#ifndef _MANGOS_METRIC_H
#define _MANGOS_METRIC_H

#include <string>
#include <chrono>
#include <Opcodes.h>
#include <unordered_map>

#include "Policies/ObjectConstructorTraits.h"

// Welcome to vMangos Metric implementation!
// This implementation is different to the one you see in cMaNGOS or TrinityCore.
//
// We try to be as non-inverse as possible to the instrumented parts of the code.
// (instrumented = part where MANGOS_METRIC() is inserted)
//
// Most variables are pre-allocated in static storage.
// The conversion to a string, which ultimately gets sent to the database, is performed in a different thread.
//
// Some measurements allow the use of scoped timers.
// It's the same principle as `std::unique_lock<std::mutex> lock(myMutex);`
// as soon as you leave the scope, the timer is stopped and committed to the measurements.
//
// Since we want to have a uniform interface for all measurements, all of them are inlined constructors.
// Tests have shown that all the unnecessary stuff gets optimized out.
// The usage is held simple with `MANGOS_METRIC()` but it's a bit awkward if you want to add new measurements.

/// Namespace holding all the internal metric stuff.
/// Use the macros below if you want to log values.
namespace MaNGOS { namespace Metric
{
    /// global static functions related to metrics
    namespace MetricService {
        struct InfluxDbCredentials
        {
            std::string username;
            std::string password;
            std::string database;
        };
        /// Initializes metric variables and test the connection
        /// \returns true if successful
        bool Initialize(InfluxDbCredentials const& credentials, std::string const& realmName);

        /// Start the sender thread which will repeatably send new metrics to the database
        void StartSenderThread();

        /// Stops and waits for the sender thread
        void Finalize();
    }
}} // MaNGOS::Metric

#if 0

#define MANGOS_METRIC(metricSetter) do {} while(0)

#else

#define _MANGOS_METRIC_VARIABLE_UNIQUE_NAME_DO_CONCAT(a, b) a ## b
#define _MANGOS_METRIC_VARIABLE_UNIQUE_NAME_CONCAT(a, b) _MANGOS_METRIC_VARIABLE_UNIQUE_NAME_DO_CONCAT(a, b)
/// Creates a unique variable name (based on the current line number)
#define _MANGOS_METRIC_VARIABLE_UNIQUE_NAME_UNIQUE_NAME(name) _MANGOS_METRIC_VARIABLE_UNIQUE_NAME_CONCAT(name, __LINE__)

#define MANGOS_METRIC(metricSetter) auto const& _MANGOS_METRIC_VARIABLE_UNIQUE_NAME_UNIQUE_NAME(_mangos_metric_scope) = ::MaNGOS::Metric::_Provider:: metricSetter

/// `_Provider` is a hidden namespace.
/// Please use `MANGOS_METRIC(IncrementalCounter::NewSocketConnection())`
/// instead of `MaNGOS::Metric::_Provider::IncrementalCounter::NewSocketConnection()`
namespace MaNGOS { namespace Metric { namespace _Provider
{
    /// Stuff that counts +1
    namespace IncrementalCounter
    {
        class NewSocketConnection : public MaNGOS::Policies::NoCopyNoMove
        {
        public:
            explicit NewSocketConnection()
            {
                g_counter++;
            }

            // Static storage
        public:
            static void WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName);
        private:
            static uint32_t g_counter;
        };

        class ReceivedPacket : public MaNGOS::Policies::NoCopyNoMove
        {
        public:
            explicit ReceivedPacket(uint16_t opcode)
            {
                g_recvOpcodeCount[opcode]++;
            }

            // Static storage
        public:
            static void WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName);
        private:
            static std::array<uint32_t, NUM_MSG_TYPES> g_recvOpcodeCount;
        };

        class SentPacket : public MaNGOS::Policies::NoCopyNoMove
        {
        public:
            explicit SentPacket(uint16_t opcode)
            {
                g_sentOpcodeCount[opcode]++;
            }

            // Static storage
        public:
            static void WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName);
        private:
            static std::array<uint32_t, NUM_MSG_TYPES> g_sentOpcodeCount;
        };
    }

    /// Stuff that takes the maximum of a value
    namespace MaximalCounter
    {
        class PlayerCount : public MaNGOS::Policies::NoCopyNoMove
        {
        public:
            // TODO: Add this commit later https://github.com/cmangos/mangos-tbc/commit/24f4a2c86273d9d06f69a43bb73b9adce44b230b
            explicit PlayerCount(uint32 sessionCount)
            {
                if (sessionCount > g_stats.sessionCount)
                    g_stats.sessionCount = sessionCount;
            }

            // Static storage
        public:
            static void WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName);
        private:
            struct MaxPlayerCountStats
            {
                uint32_t sessionCount;
            };

            static MaxPlayerCountStats g_stats;
        };

        class LoadedMaps : public MaNGOS::Policies::NoCopyNoMove
        {
        public:
            explicit LoadedMaps(uint32 loadedMaps)
            {
                if (loadedMaps > g_loadedMaps)
                    g_loadedMaps = loadedMaps;
            }

            // Static storage
        public:
            static void WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName);
        private:
            static uint32 g_loadedMaps;
        };

        class OpenGmTickets : public MaNGOS::Policies::NoCopyNoMove
        {
        public:
            explicit OpenGmTickets(uint32 gmTicketCount)
            {
                if (gmTicketCount > g_gmTicketCount)
                    g_gmTicketCount = gmTicketCount;
            }

            // Static storage
        public:
            static void WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName);
        private:
            static uint32 g_gmTicketCount;
        };
    }

    /// Metrics that can automatically measure the time of the current scope
    namespace ScopedStopwatch
    {
        class PacketProcessTime : public MaNGOS::Policies::NoCopyNoMove
        {
        public:
            explicit PacketProcessTime(uint16_t opcode) : m_startTime(std::chrono::high_resolution_clock::now()), m_opcode(opcode)
            {

            }
            ~PacketProcessTime()
            {
                auto endTime = std::chrono::high_resolution_clock::now();
                auto duration = endTime - m_startTime;
                float timeInMs = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(duration).count()) / 1000.0f;
                float& maxTimeInMs = g_maxTimeMsPerOpcode[m_opcode];
                if (timeInMs > maxTimeInMs)
                {
                    maxTimeInMs = timeInMs;
                }
            }
        private:
            std::chrono::high_resolution_clock::time_point m_startTime;
            uint16_t m_opcode;

        // Static storage
        public:
            static void WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName);
        private:
            static std::array<float, NUM_MSG_TYPES> g_maxTimeMsPerOpcode;
        };

        class TotalWorldUpdateTime : public MaNGOS::Policies::NoCopyNoMove
        {
        public:
            explicit TotalWorldUpdateTime() : m_startTime(std::chrono::high_resolution_clock::now())
            {

            }
            ~TotalWorldUpdateTime()
            {
                auto endTime = std::chrono::high_resolution_clock::now();
                auto duration = endTime - m_startTime;
                float timeInMs = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(duration).count()) / 1000.0f;
                if (timeInMs > g_maxUpdateTimeMs)
                {
                    g_maxUpdateTimeMs = timeInMs;
                }
            }
        private:
            std::chrono::high_resolution_clock::time_point m_startTime;

            // Static storage
        public:
            static void WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName);
        private:
            static float g_maxUpdateTimeMs;
        };
    }
}}} // namespace MaNGOS::Metric::_Provider

#endif

#endif // _MANGOS_METRIC_H
