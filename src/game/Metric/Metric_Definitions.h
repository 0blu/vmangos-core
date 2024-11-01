#ifndef _MANGOS_METRIC_DEFINITIONS_H
#define _MANGOS_METRIC_DEFINITIONS_H

#ifndef _MANGOS_METRIC_WAS_INCLUDED_BY_INTERNAL
#error You are somehow using a wrong include. You should use `#include "Metric/Metric.h"`
#endif

#include <array>
#include <Opcodes.h>

/// `_Provider` is a hidden namespace.
/// Please use `MANGOS_METRIC(IncrementalCounter::NewSocketConnection())`
/// instead of `MaNGOS::Metric::_Provider::IncrementalCounter::NewSocketConnection()`
namespace MaNGOS { namespace Metric { namespace _Provider
{

    DEFINE_METRIC(IncrementalCounter, network_newSocketConnections, "network.newSocketConnections");

    MANGOS_METRIC(IncrementalCounter::network_newSocketConnections);

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
            static void WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd);
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
            static void WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd);
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
            static void WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd);
        private:
            static std::array<uint32_t, NUM_MSG_TYPES> g_sentOpcodeCount;
        };
    }

    /// Stuff that takes the maximum of a value
    namespace MaximalCounter
    {
        class SessionCount : public MaNGOS::Policies::NoCopyNoMove
        {
        public:
            // TODO: Add this commit later https://github.com/cmangos/mangos-tbc/commit/24f4a2c86273d9d06f69a43bb73b9adce44b230b
            explicit SessionCount(uint32 sessionCount)
            {
                if (sessionCount > g_stats.sessionCount)
                    g_stats.sessionCount = sessionCount;
            }

            // Static storage
        public:
            static void WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd);
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
            static void WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd);
        private:
            static uint32 g_loadedMaps;
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
            static void WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd);
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
            static void WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd);
        private:
            static float g_maxUpdateTimeMs;
        };
    }
}}} // namespace MaNGOS::Metric::_Provider

#endif //_MANGOS_METRIC_DEFINITIONS_H
