#ifndef MANGOS_IO_NETWORKING_HTTP_HTTPSERVER_H
#define MANGOS_IO_NETWORKING_HTTP_HTTPSERVER_H

#include "IO/Context/IoContext.h"
#include "IO/Networking/AsyncSocketAcceptor.h"
#include "IO/Networking/Http/HttpRequest.h"

#include <memory>
#include <string>
#include <functional>

namespace IO { namespace Networking { namespace Http {

/// A simple HTTP/1.1 server built on top of IO::Networking::AsyncSocketAcceptor.
/// Parses incoming HTTP request headers and dispatches them to a user-provided handler.
class HttpServer
{
public:
    using RequestHandler = std::function<void(std::shared_ptr<HttpRequest> request)>;

    /// Creates an HTTP server bound to the given address and port.
    /// Returns nullptr on failure (e.g. port already in use).
    static std::unique_ptr<HttpServer> Create(IO::IoContext* ctx, std::string const& bindIp, uint16_t port);

    /// Sets the handler that will be called for each incoming HTTP request
    /// and starts accepting connections. Must be called before the IoContext starts running.
    void OnRequest(RequestHandler handler);

    /// Stops accepting new connections.
    void Shutdown();

    ~HttpServer();

    HttpServer(HttpServer const&) = delete;
    HttpServer& operator=(HttpServer const&) = delete;

private:
    HttpServer(IO::IoContext* ctx, std::unique_ptr<IO::Networking::AsyncSocketAcceptor> acceptor);

    IO::IoContext* m_ctx;
    std::unique_ptr<IO::Networking::AsyncSocketAcceptor> m_acceptor;
    RequestHandler m_handler;
};

}}} // namespace IO::Networking::Http

#endif // MANGOS_IO_NETWORKING_HTTP_HTTPSERVER_H
