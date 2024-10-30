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
    /// Stuff that counts +1
    namespace IncrementalCounter
    {
        class network_packet_recvCount : public MaNGOS::Policies::NoCopyNoMove
        {
        public:
            explicit network_packet_recvCount(uint16_t opcode)
            {
                g_counterPerOpcode[opcode]++;
            }
            static void WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd);
        private:
            static std::array<uint32_t, NUM_MSG_TYPES> g_counterPerOpcode;
        };

        class network_packet_sendCount : public MaNGOS::Policies::NoCopyNoMove
        {
        public:
            explicit network_packet_sendCount(uint16_t opcode)
            {
                g_counterPerOpcode[opcode]++;
            }
            static void WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd);
        private:
            static std::array<uint32_t, NUM_MSG_TYPES> g_counterPerOpcode;
        };

        class network_packet_sendBytes : public MaNGOS::Policies::NoCopyNoMove
        {
        public:
            explicit network_packet_sendBytes(uint16_t opcode, size_t size)
            {
                g_counterPerOpcode[opcode] += size;
            }
            static void WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd);
        private:
            static std::array<uint32_t, NUM_MSG_TYPES> g_counterPerOpcode;
        };

        class network_packet_recvBytes : public MaNGOS::Policies::NoCopyNoMove
        {
        public:
            explicit network_packet_recvBytes(uint16_t opcode, size_t size)
            {
                g_counterPerOpcode[opcode] += size;
            }
            static void WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd);
        private:
            static std::array<uint32_t, NUM_MSG_TYPES> g_counterPerOpcode;
        };
    }

    /// Stuff that takes the maximum of a value
    namespace MaximalCounter
    {
    }

    /// Metrics that can automatically measure the time of the current scope
    namespace ScopedStopwatch
    {
        class network_packet_processTime : public MaNGOS::Policies::NoCopyNoMove
        {
        public:
            explicit network_packet_processTime(uint16_t opcode) : m_startTime(std::chrono::high_resolution_clock::now()), m_opcode(opcode)
            {

            }
            ~network_packet_processTime()
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
    }
}}} // namespace MaNGOS::Metric::_Provider
