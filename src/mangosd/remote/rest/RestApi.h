#ifndef MANGOS_RESTAPI_H
#define MANGOS_RESTAPI_H

#include "IO/Networking/Http/HttpServer.h"

#include <memory>
#include <string>
#include <cstdint>

namespace IO { class IoContext; }

/// Starts the built-in REST API HTTP server.
/// Returns nullptr on failure (e.g. port already in use).
std::unique_ptr<IO::Networking::Http::HttpServer> StartRestApi(
    IO::IoContext* ioCtx,
    std::string const& bindIp,
    uint16_t bindPort);

#endif // MANGOS_RESTAPI_H
