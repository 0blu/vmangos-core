#include "RestApi.h"

#include "IO/Networking/Http/HttpRequest.h"
#include "IO/Networking/Http/HttpResponse.h"
#include "Crypto/Encoding/Base64.h"

#include "World.h"
#include "AccountMgr.h"
#include "Log.h"
#include "Config/Config.h"
#include "Util.h"
#include "json.hpp"

#include <functional>
#include <future>
#include <string>
#include <vector>

using namespace IO::Networking::Http;
using json = nlohmann::json;

namespace
{
    // ---------- helpers ----------

    void SendJson(std::shared_ptr<HttpRequest> const& request, HttpStatusCode status, json const& body)
    {
        HttpResponse response(status);
        response.SetContentType("application/json");
        request->SendResponse(response, body.dump());
    }

    void SendError(std::shared_ptr<HttpRequest> const& request, HttpStatusCode status, std::string const& message)
    {
        json body;
        body["error"] = message;
        SendJson(request, status, body);
    }

    // Basic-auth helper.  Returns true when the caller is authenticated
    // at SEC_ADMINISTRATOR or above.  Sends an HTTP 401 / 403 on failure.
    bool AuthenticateRequest(std::shared_ptr<HttpRequest> const& request)
    {
        std::string authHeader = request->GetHeader("authorization");
        if (authHeader.empty())
        {
            HttpResponse resp(HttpStatusCode::Unauthorized);
            resp.SetHeader("WWW-Authenticate", "Basic realm=\"MaNGOS REST API\"");
            resp.SetContentType("application/json");
            request->SendResponse(resp, R"({"error":"Authentication required"})");
            return false;
        }

        // Expect "Basic <base64(user:pass)>"
        if (authHeader.size() < 7 || authHeader.substr(0, 6) != "Basic ")
        {
            SendError(request, HttpStatusCode::BadRequest, "Unsupported authentication scheme");
            return false;
        }

        std::string encoded = authHeader.substr(6);
        static constexpr uint32 MAX_CREDENTIALS_LENGTH = 512;
        auto decodedBytes = Crypto::Encoding::Base64::Decode(encoded, MAX_CREDENTIALS_LENGTH);
        if (!decodedBytes)
        {
            SendError(request, HttpStatusCode::BadRequest, "Malformed base64 in Authorization header");
            return false;
        }

        std::string decoded(decodedBytes->begin(), decodedBytes->end());
        size_t colonPos = decoded.find(':');
        if (colonPos == std::string::npos)
        {
            SendError(request, HttpStatusCode::BadRequest, "Invalid credentials format");
            return false;
        }

        std::string username = decoded.substr(0, colonPos);
        std::string password = decoded.substr(colonPos + 1);

        uint32 accountId = sAccountMgr.GetId(username);
        if (!accountId)
        {
            sLog.Out(LOG_RA, LOG_LVL_DETAIL, "REST API: invalid username '%s' from %s",
                      username.c_str(), request->GetRemoteIpString().c_str());
            SendError(request, HttpStatusCode::Unauthorized, "Invalid credentials");
            return false;
        }

        if (!sAccountMgr.CheckPassword(accountId, password))
        {
            sLog.Out(LOG_RA, LOG_LVL_DETAIL, "REST API: wrong password for account '%s' from %s",
                      username.c_str(), request->GetRemoteIpString().c_str());
            SendError(request, HttpStatusCode::Unauthorized, "Invalid credentials");
            return false;
        }

        if (sAccountMgr.GetSecurity(accountId) < SEC_ADMINISTRATOR)
        {
            sLog.Out(LOG_RA, LOG_LVL_DETAIL, "REST API: account '%s' (gmlevel too low) from %s",
                      username.c_str(), request->GetRemoteIpString().c_str());
            SendError(request, HttpStatusCode::Forbidden, "Insufficient privileges");
            return false;
        }

        return true;
    }

    // ---------- route: GET /api/v1/server/status ----------
    void HandleServerStatus(std::shared_ptr<HttpRequest> request)
    {
        json body;
        body["online"]              = !World::IsStopped();
        body["uptime"]              = sWorld.GetUptime();
        body["activeSessions"]      = sWorld.GetActiveSessionCount();
        body["queuedSessions"]      = sWorld.GetQueuedSessionCount();
        body["maxActiveSessions"]   = sWorld.GetMaxActiveSessionCount();
        body["motd"]                = sWorld.GetMotd();
        body["realmName"]           = realmName;

        SendJson(request, HttpStatusCode::OK, body);
    }

    // ---------- route: POST /api/v1/server/command ----------
    // Body: { "command": "server info" }
    void HandleCommand(std::shared_ptr<HttpRequest> request)
    {
        std::string contentLengthStr = request->GetHeader("content-length");
        if (contentLengthStr.empty())
        {
            SendError(request, HttpStatusCode::BadRequest, "Missing Content-Length header");
            return;
        }

        auto parsedLength = TryParseUint64(contentLengthStr);
        if (!parsedLength)
        {
            SendError(request, HttpStatusCode::BadRequest, "Invalid Content-Length");
            return;
        }
        size_t contentLength = static_cast<size_t>(*parsedLength);

        static constexpr size_t MAX_COMMAND_BODY = 4096;
        if (contentLength > MAX_COMMAND_BODY)
        {
            SendError(request, HttpStatusCode::PayloadTooLarge, "Request body too large");
            return;
        }

        // Read the body asynchronously, then execute the command
        auto bodyBuf = std::make_shared<std::vector<char>>(contentLength);
        request->ReadBody(bodyBuf->data(), contentLength,
            [request, bodyBuf, contentLength](IO::NetworkError const& err, size_t /*bytesRead*/)
        {
            if (err)
            {
                SendError(request, HttpStatusCode::BadRequest, "Failed to read request body");
                return;
            }

            std::string rawBody(bodyBuf->data(), contentLength);
            json parsed;
            try { parsed = json::parse(rawBody); }
            catch (json::parse_error const&)
            {
                SendError(request, HttpStatusCode::BadRequest, "Invalid JSON");
                return;
            }

            if (parsed.find("command") == parsed.end() || !parsed["command"].is_string())
            {
                SendError(request, HttpStatusCode::BadRequest, "Missing 'command' field");
                return;
            }

            std::string command = parsed["command"].get<std::string>();
            if (command.empty())
            {
                SendError(request, HttpStatusCode::BadRequest, "Command cannot be empty");
                return;
            }

            sLog.Out(LOG_RA, LOG_LVL_BASIC, "REST API: executing command '%s' from %s",
                      command.c_str(), request->GetRemoteIpString().c_str());

            // Execute on the world thread, mirroring the SOAP approach with a shared promise.
            struct CommandContext
            {
                std::promise<bool> successPromise;
                std::string output;

                static void OnPrint(void* ctx, char const* msg)
                {
                    static_cast<CommandContext*>(ctx)->output += msg;
                }
                static void OnFinished(void* ctx, bool success)
                {
                    static_cast<CommandContext*>(ctx)->successPromise.set_value(success);
                }
            };

            auto cmdCtx = std::make_shared<CommandContext>();
            {
                // Note: CliCommandHolder is deleted by the world thread after execution
                sWorld.QueueCliCommand(new CliCommandHolder(
                    0, SEC_CONSOLE, cmdCtx.get(), command.c_str(),
                    &CommandContext::OnPrint, &CommandContext::OnFinished));
            }

            // Block on the IO thread until the command completes.
            // HttpServer uses its own async IO context.
            bool success = cmdCtx->successPromise.get_future().get();

            json result;
            result["success"] = success;
            result["output"]  = cmdCtx->output;

            SendJson(request, success ? HttpStatusCode::OK : HttpStatusCode::BadRequest, result);
        });
    }

    // ---------- route dispatcher ----------
    void DispatchRequest(std::shared_ptr<HttpRequest> request)
    {
        std::string const& path = request->path;
        std::string const& method = request->method;

        sLog.Out(LOG_RA, LOG_LVL_DEBUG, "REST API: %s %s from %s",
                  method.c_str(), path.c_str(), request->GetRemoteIpString().c_str());

        // --- public endpoint (no auth) ---
        if (method == "GET" && path == "/api/v1/server/status")
        {
            HandleServerStatus(request);
            return;
        }

        // --- authenticated endpoints ---
        if (!AuthenticateRequest(request))
            return;

        if (method == "POST" && path == "/api/v1/server/command")
        {
            HandleCommand(request);
            return;
        }

        // fallthrough
        SendError(request, HttpStatusCode::NotFound, "Not found");
    }

} // anonymous namespace

std::unique_ptr<IO::Networking::Http::HttpServer> StartRestApi(
    IO::IoContext* ioCtx,
    std::string const& bindIp,
    uint16_t bindPort)
{
    auto server = IO::Networking::Http::HttpServer::Create(ioCtx, bindIp, bindPort);
    if (!server)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "REST API: failed to bind to %s:%u", bindIp.c_str(), bindPort);
        return nullptr;
    }

    server->OnRequest(&DispatchRequest);

    return server;
}
