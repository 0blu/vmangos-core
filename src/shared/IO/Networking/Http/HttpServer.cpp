#include "IO/Networking/Http/HttpServer.h"
#include "IO/Networking/AsyncSocket.h"
#include "IO/Timer/AsyncSystemTimer.h"
#include "IO/ReadableBuffer.h"
#include "Log.h"

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>

namespace IO { namespace Networking { namespace Http {

namespace
{
    static constexpr size_t MAX_HEADER_SIZE = 8192;

    void SendRawErrorAndClose(
        std::shared_ptr<IO::Networking::AsyncSocket> const& socket,
        char const* statusLine,
        std::string const& body)
    {
        std::string response;
        response.reserve(256 + body.size());
        response += statusLine;
        response += "\r\nContent-Type: text/plain\r\nContent-Length: ";
        response += std::to_string(body.size());
        response += "\r\nConnection: close\r\n\r\n";
        response += body;

        std::vector<char> data(response.begin(), response.end());
        socket->Write(IO::ReadableBuffer(std::move(data)), [socket](IO::NetworkError const&)
        {
            socket->CloseSocket();
        });
    }

    std::shared_ptr<HttpRequest> ParseHttpRequest(
        std::shared_ptr<IO::Networking::AsyncSocket> socket,
        std::string const& headerSection,
        std::vector<char> prereadBodyBytes)
    {
        // Parse request line: "METHOD PATH HTTP/VERSION"
        size_t firstLineEnd = headerSection.find("\r\n");
        if (firstLineEnd == std::string::npos)
            firstLineEnd = headerSection.size();

        std::string requestLine = headerSection.substr(0, firstLineEnd);

        size_t sp1 = requestLine.find(' ');
        if (sp1 == std::string::npos)
            return nullptr;

        size_t sp2 = requestLine.find(' ', sp1 + 1);
        if (sp2 == std::string::npos)
            return nullptr;

        std::string method = requestLine.substr(0, sp1);
        std::string path = requestLine.substr(sp1 + 1, sp2 - sp1 - 1);
        std::string httpVersion = requestLine.substr(sp2 + 1);

        // Parse headers
        std::unordered_map<std::string, std::string> headers;
        size_t pos = firstLineEnd + 2; // skip \r\n after request line
        while (pos < headerSection.size())
        {
            size_t lineEnd = headerSection.find("\r\n", pos);
            if (lineEnd == std::string::npos)
                lineEnd = headerSection.size();

            if (lineEnd == pos)
                break; // empty line

            std::string line = headerSection.substr(pos, lineEnd - pos);
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos)
            {
                std::string key = line.substr(0, colonPos);
                std::string value = line.substr(colonPos + 1);

                // Trim leading whitespace from value
                size_t valueStart = value.find_first_not_of(' ');
                if (valueStart != std::string::npos && valueStart > 0)
                    value = value.substr(valueStart);

                // Lowercase the key for case-insensitive lookup
                std::transform(key.begin(), key.end(), key.begin(),
                    [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

                headers[key] = value;
            }

            pos = lineEnd + 2;
        }

        return std::make_shared<HttpRequest>(
            std::move(socket),
            std::move(method),
            std::move(path),
            std::move(httpVersion),
            std::move(headers),
            std::move(prereadBodyBytes));
    }

    void ContinueReadingHeaders(
        std::shared_ptr<IO::Networking::AsyncSocket> socket,
        std::shared_ptr<std::vector<char>> buffer,
        size_t totalRead,
        std::shared_ptr<IO::Timer::TimerHandle> timeout,
        HttpServer::RequestHandler handler)
    {
        socket->ReadSome(
            buffer->data() + totalRead,
            buffer->size() - totalRead,
            [socket, buffer, totalRead, timeout, handler](IO::NetworkError const& error, size_t bytesRead)
        {
            if (error)
                return; // connection lost, timer will fire harmlessly

            size_t newTotal = totalRead + bytesRead;

            // Search for the end-of-headers marker
            std::string data(buffer->data(), newTotal);
            size_t headerEnd = data.find("\r\n\r\n");

            if (headerEnd != std::string::npos)
            {
                timeout->Cancel();

                // Parse the request
                std::string headerSection = data.substr(0, headerEnd);
                size_t bodyStart = headerEnd + 4;
                std::vector<char> prereadBody;
                if (bodyStart < newTotal)
                    prereadBody.assign(buffer->data() + bodyStart, buffer->data() + newTotal);

                auto request = ParseHttpRequest(socket, headerSection, std::move(prereadBody));
                if (request)
                {
                    handler(request);
                }
                else
                {
                    SendRawErrorAndClose(socket, "HTTP/1.1 400 Bad Request", "Malformed HTTP request");
                }
            }
            else if (newTotal >= buffer->size())
            {
                SendRawErrorAndClose(socket, "HTTP/1.1 431 Request Header Fields Too Large",
                    "Request headers exceeded maximum allowed size");
            }
            else
            {
                // Continue reading more header data
                ContinueReadingHeaders(socket, buffer, newTotal, timeout, handler);
            }
        });
    }
} // anonymous namespace

// --- HttpServer ---

HttpServer::HttpServer(IO::IoContext* ctx, std::unique_ptr<IO::Networking::AsyncSocketAcceptor> acceptor)
    : m_ctx(ctx)
    , m_acceptor(std::move(acceptor))
{
}

HttpServer::~HttpServer()
{
    Shutdown();
}

std::unique_ptr<HttpServer> HttpServer::Create(IO::IoContext* ctx, std::string const& bindIp, uint16_t port)
{
    auto acceptor = IO::Networking::AsyncSocketAcceptor::CreateAndBindServer(ctx, bindIp, port);
    if (!acceptor)
        return nullptr;

    return std::unique_ptr<HttpServer>(new HttpServer(ctx, std::move(acceptor)));
}

void HttpServer::OnRequest(RequestHandler handler)
{
    m_handler = std::move(handler);

    m_acceptor->AutoAcceptSocketsUntilClose([this](IO::Networking::SocketDescriptor socketDescriptor)
    {
        auto socket = std::make_shared<IO::Networking::AsyncSocket>(m_ctx, std::move(socketDescriptor));

        if (IO::NetworkError initError = socket->InitializeAndFixateMemoryLocation())
        {
            sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "[%s] HTTP socket init error: %s", socket->GetRemoteIpString().c_str(), initError.ToString().c_str());
            return;
        }

        if (!m_handler)
        {
            SendRawErrorAndClose(socket, "HTTP/1.1 503 Service Unavailable", "No request handler configured");
            return;
        }

        auto buffer = std::make_shared<std::vector<char>>(MAX_HEADER_SIZE, '\0');

        auto timeout = sAsyncSystemTimer.ScheduleFunctionOnce(HEADER_READ_TIMEOUT, [socket]()
        {
            sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "[%s] HTTP header read timed out", socket->GetRemoteIpString().c_str());
            socket->CloseSocket();
        });

        ContinueReadingHeaders(socket, buffer, 0, timeout, m_handler);
    });
}

void HttpServer::Shutdown()
{
    if (m_acceptor)
        m_acceptor->ClosePortAndStopAcceptingNewConnections();
}

}}} // namespace IO::Networking::Http
