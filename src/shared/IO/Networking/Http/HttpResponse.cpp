#include "IO/Networking/Http/HttpResponse.h"
#include "Errors.h"

namespace IO { namespace Networking { namespace Http {

HttpResponse::HttpResponse(HttpStatusCode statusCode)
    : statusCode(statusCode)
{
}

HttpResponse& HttpResponse::SetHeader(std::string const& name, std::string const& value)
{
    for (auto& header : headers)
    {
        if (header.first == name)
        {
            header.second = value;
            return *this;
        }
    }
    headers.emplace_back(name, value);
    return *this;
}

HttpResponse& HttpResponse::SetContentType(std::string const& contentType)
{
    return SetHeader("Content-Type", contentType);
}

char const* HttpResponse::GetReasonPhrase(HttpStatusCode statusCode)
{
    switch (statusCode)
    {
        case HttpStatusCode::Continue:                    return "Continue";
        case HttpStatusCode::OK:                          return "OK";
        case HttpStatusCode::Created:                     return "Created";
        case HttpStatusCode::NoContent:                   return "No Content";
        case HttpStatusCode::MovedPermanently:            return "Moved Permanently";
        case HttpStatusCode::Found:                       return "Found";
        case HttpStatusCode::NotModified:                 return "Not Modified";
        case HttpStatusCode::BadRequest:                  return "Bad Request";
        case HttpStatusCode::Unauthorized:                return "Unauthorized";
        case HttpStatusCode::Forbidden:                   return "Forbidden";
        case HttpStatusCode::NotFound:                    return "Not Found";
        case HttpStatusCode::MethodNotAllowed:            return "Method Not Allowed";
        case HttpStatusCode::PayloadTooLarge:             return "Payload Too Large";
        case HttpStatusCode::RequestHeaderFieldsTooLarge: return "Request Header Fields Too Large";
        case HttpStatusCode::InternalServerError:         return "Internal Server Error";
        case HttpStatusCode::BadGateway:                  return "Bad Gateway";
        case HttpStatusCode::ServiceUnavailable:          return "Service Unavailable";
    }
    MANGOS_ASSERT(false && "Unhandled HttpStatusCode in GetReasonPhrase");
    return "Unknown";
}

}}} // namespace IO::Networking::Http
