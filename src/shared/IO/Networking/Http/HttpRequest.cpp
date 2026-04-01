#include "IO/Networking/Http/HttpRequest.h"
#include "IO/Networking/Http/HttpResponse.h"
#include "IO/Timer/AsyncSystemTimer.h"
#include "Errors.h"
#include "Log.h"

#include <algorithm>
#include <cstring>

namespace IO { namespace Networking { namespace Http {

HttpRequest::HttpRequest(
    std::shared_ptr<IO::Networking::AsyncSocket> socket,
    std::string method,
    std::string path,
    std::string httpVersion,
    std::unordered_map<std::string, std::string> headers,
    std::vector<char> prereadBodyBytes
    )
    : method(std::move(method))
    , path(std::move(path))
    , httpVersion(std::move(httpVersion))
    , headers(std::move(headers))
    , m_socket(std::move(socket))
    , m_prereadBodyBytes(std::move(prereadBodyBytes))
{
}

std::string HttpRequest::GetHeader(std::string const& name) const
{
    std::string lowerName = name;
    std::transform(
        lowerName.begin(), lowerName.end(), lowerName.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); }
    );
    auto it = headers.find(lowerName);
    if (it != headers.end())
        return it->second;
    return {};
}

std::string const& HttpRequest::GetRemoteIpString() const
{
    return m_socket->GetRemoteIpString();
}

void HttpRequest::ReadBody(char* target, size_t size, std::function<void(IO::NetworkError const&, size_t bytesRead)> const& callback)
{
    // Copy any bytes already received during header parsing
    size_t prereadSize = std::min(m_prereadBodyBytes.size(), size);
    if (prereadSize > 0)
        std::memcpy(target, m_prereadBodyBytes.data(), prereadSize);

    if (prereadSize >= size)
    {
        IO::NetworkError noError(IO::NetworkError::ErrorType::NoError);
        callback(noError, size);
        return;
    }

    size_t remaining = size - prereadSize;
    auto self = shared_from_this();

    auto timeoutHandle = sAsyncSystemTimer.ScheduleFunctionOnce(BODY_READ_TIMEOUT, [self]()
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "[%s] HTTP body read timed out", self->GetRemoteIpString().c_str());
        self->m_socket->CloseSocket();
    });

    // Wrap the user callback to cancel the timeout on completion
    auto callbackWithTimeout = [callback, timeoutHandle](IO::NetworkError const& err, size_t bytesRead)
    {
        timeoutHandle->Cancel();
        callback(err, bytesRead);
    };

    // Handle "Expect: 100-continue" — tell the client to proceed sending the body
    std::string expect = GetHeader("expect");
    if (expect == "100-continue")
    {
        std::string continueStr = "HTTP/1.1 100 Continue\r\n\r\n";
        std::vector<char> continueData(continueStr.begin(), continueStr.end());

        m_socket->Write(IO::ReadableBuffer(std::move(continueData)), [self, target, prereadSize, remaining, size, callbackWithTimeout](IO::NetworkError const& writeErr)
        {
            if (writeErr)
            {
                callbackWithTimeout(writeErr, 0);
                return;
            }
            self->m_socket->Read(target + prereadSize, remaining, [size, callbackWithTimeout](IO::NetworkError const& readErr, size_t)
            {
                callbackWithTimeout(readErr, readErr ? 0 : size);
            });
        });
    }
    else
    {
        m_socket->Read(target + prereadSize, remaining, [size, callbackWithTimeout](IO::NetworkError const& readErr, size_t)
        {
            callbackWithTimeout(readErr, readErr ? 0 : size);
        });
    }
}

void HttpRequest::SendResponse(HttpResponse const& response)
{
    std::string head = SerializeResponseHead(response, 0);

    std::vector<char> responseData(head.begin(), head.end());
    SendRawResponse(std::move(responseData));
}

void HttpRequest::SendResponse(HttpResponse const& response, std::string const& body)
{
    std::string head = SerializeResponseHead(response, body.size());

    std::vector<char> responseData;
    responseData.reserve(head.size() + body.size());
    responseData.insert(responseData.end(), head.begin(), head.end());
    responseData.insert(responseData.end(), body.begin(), body.end());
    SendRawResponse(std::move(responseData));
}

void HttpRequest::SendResponse(HttpResponse const& response, IO::ReadableBuffer body)
{
    std::string head = SerializeResponseHead(response, body.GetSize());

    std::vector<char> responseData;
    responseData.reserve(head.size() + body.GetSize());
    responseData.insert(responseData.end(), head.begin(), head.end());
    responseData.insert(responseData.end(), body.GetPtr(), body.GetPtr() + body.GetSize());
    SendRawResponse(std::move(responseData));
}

void HttpRequest::SendRawResponse(std::vector<char> responseData)
{
    auto self = shared_from_this();
    m_socket->Write(IO::ReadableBuffer(std::move(responseData)), [self](IO::NetworkError const& err)
    {
        if (err)
            sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "[%s] HTTP write error: %s",
                self->GetRemoteIpString().c_str(), err.ToString().c_str());
        self->m_socket->CloseSocket();
    });
}

std::string HttpRequest::SerializeResponseHead(HttpResponse const& response, size_t contentLength)
{
    char const* reasonPhrase = HttpResponse::GetReasonPhrase(response.statusCode);
    uint16_t code = static_cast<uint16_t>(response.statusCode);

    std::string head;
    head.reserve(256);
    head += "HTTP/1.1 ";
    head += std::to_string(code);
    head += ' ';
    head += reasonPhrase;
    head += "\r\n";

    for (auto const& header : response.headers)
    {
        head += header.first;
        head += ": ";
        head += header.second;
        head += "\r\n";
    }

    head += "Content-Length: ";
    head += std::to_string(contentLength);
    head += "\r\nConnection: close\r\n\r\n";

    return head;
}

}}} // namespace IO::Networking::Http
