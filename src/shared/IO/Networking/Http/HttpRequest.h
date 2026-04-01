#ifndef MANGOS_IO_NETWORKING_HTTP_HTTPREQUEST_H
#define MANGOS_IO_NETWORKING_HTTP_HTTPREQUEST_H

#include "IO/Networking/AsyncSocket.h"
#include "IO/Networking/NetworkError.h"
#include "IO/Networking/Http/HttpResponse.h"
#include "IO/ReadableBuffer.h"

#include <chrono>
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <vector>
#include <cstdint>

namespace IO { namespace Networking { namespace Http {

static constexpr std::chrono::seconds HEADER_READ_TIMEOUT{30};
static constexpr std::chrono::seconds BODY_READ_TIMEOUT{30};

/// Represents a parsed HTTP request and provides methods to read the body and send a response.
/// Passed to the user's handler as a shared_ptr; must be kept alive during async operations.
class HttpRequest final : public MaNGOS::Policies::NoCopyButAllowMove, public std::enable_shared_from_this<HttpRequest>
{
public:
    /// HTTP method (e.g. "GET", "POST", "PUT")
    std::string const method;
    /// Request path (e.g. "/", "/api/users")
    std::string const path;
    /// HTTP version string (e.g. "HTTP/1.1")
    std::string const httpVersion;
    /// Request headers. Keys are lowercased for case-insensitive lookup.
    std::unordered_map<std::string, std::string> const headers;

public:
    HttpRequest(
        std::shared_ptr<IO::Networking::AsyncSocket> socket,
        std::string method,
        std::string path,
        std::string httpVersion,
        std::unordered_map<std::string, std::string> headers,
        std::vector<char> prereadBodyBytes);

    /// Returns the header value for the given name (case-insensitive).
    /// Returns an empty string if the header is not present.
    std::string GetHeader(std::string const& name) const;

    /// Returns the remote IP address as a string
    std::string const& GetRemoteIpString() const;

    /// Reads the request body into a user-provided buffer.
    /// Any bytes already received during header parsing are copied into `target` first,
    /// then the remaining bytes are read from the socket.
    /// If the client sent "Expect: 100-continue", a 100 Continue response is sent automatically.
    /// The caller is responsible for parsing Content-Length and allocating an appropriately sized buffer.
    /// If the body is not fully received within the timeout, the socket is closed
    /// and the callback receives an error.
    void ReadBody(char* target, size_t size, std::function<void(IO::NetworkError const&, size_t bytesRead)> const& callback);

    /// Sends an HTTP response with no body and closes the connection.
    void SendResponse(HttpResponse const& response);

    /// Sends an HTTP response with a string body and closes the connection.
    void SendResponse(HttpResponse const& response, std::string const& body);

    /// Sends an HTTP response with a ReadableBuffer body and closes the connection.
    void SendResponse(HttpResponse const& response, IO::ReadableBuffer body);

private:
    std::shared_ptr<IO::Networking::AsyncSocket> m_socket;
    /// Since we are using ReadSome, we might have already received some bytes of the body while reading headers.
    std::vector<char> m_prereadBodyBytes;

    void SendRawResponse(std::vector<char> responseData);
    static std::string SerializeResponseHead(HttpResponse const& response, size_t contentLength);
};

}}} // namespace IO::Networking::Http

#endif // MANGOS_IO_NETWORKING_HTTP_HTTPREQUEST_H
