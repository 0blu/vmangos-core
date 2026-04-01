#ifndef MANGOS_IO_NETWORKING_HTTP_HTTPRESPONSE_H
#define MANGOS_IO_NETWORKING_HTTP_HTTPRESPONSE_H

#include <string>
#include <vector>
#include <utility>
#include <cstdint>

namespace IO { namespace Networking { namespace Http {

enum class HttpStatusCode : uint16_t
{
    Continue                  = 100,
    OK                        = 200,
    Created                   = 201,
    NoContent                 = 204,
    MovedPermanently          = 301,
    Found                     = 302,
    NotModified               = 304,
    BadRequest                = 400,
    Unauthorized              = 401,
    Forbidden                 = 403,
    NotFound                  = 404,
    MethodNotAllowed          = 405,
    PayloadTooLarge           = 413,
    RequestHeaderFieldsTooLarge = 431,
    InternalServerError       = 500,
    BadGateway                = 502,
    ServiceUnavailable        = 503,
};

/// Represents an HTTP response with status code and headers.
/// Use builder-style methods to set headers before passing to SendResponse.
struct HttpResponse
{
    HttpStatusCode statusCode;
    std::vector<std::pair<std::string, std::string>> headers;

    explicit HttpResponse(HttpStatusCode statusCode);

    /// Sets an arbitrary header. If the header already exists, it is replaced.
    HttpResponse& SetHeader(std::string const& name, std::string const& value);

    /// Convenience shorthand for SetHeader("Content-Type", contentType).
    HttpResponse& SetContentType(std::string const& contentType);

    /// Returns the reason phrase for the given status code (e.g. "OK", "Not Found").
    static char const* GetReasonPhrase(HttpStatusCode statusCode);
};

}}} // namespace IO::Networking::Http

#endif // MANGOS_IO_NETWORKING_HTTP_HTTPRESPONSE_H
