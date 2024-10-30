#include "./Metric.h"

std::array<uint32_t, NUM_MSG_TYPES> MaNGOS::Metric::_Provider::IncrementalCounter::network_packet_recvCount::g_counterPerOpcode;
void MaNGOS::Metric::_Provider::IncrementalCounter::network_packet_recvCount::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd)
{
    for (size_t opcode = 0; opcode < g_counterPerOpcode.size(); ++opcode)
    {
        uint32_t& count = g_counterPerOpcode[opcode];
        if (count != 0)
        {
            uint32_t localCount = count;
            count = 0;

            OpcodeHandler const& handler = Singleton<Opcodes>::Instance()[static_cast<uint16>(opcode)];
            output << metricPathPrefix << "network.packet.recvCount." << handler.name << ' ' << localCount << timestampAndLineEnd;
        }
    }
}

std::array<uint32_t, NUM_MSG_TYPES> MaNGOS::Metric::_Provider::IncrementalCounter::network_packet_sendCount::g_counterPerOpcode;
void MaNGOS::Metric::_Provider::IncrementalCounter::network_packet_sendCount::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd)
{
    for (size_t opcode = 0; opcode < g_counterPerOpcode.size(); ++opcode)
    {
        uint32_t& count = g_counterPerOpcode[opcode];
        if (count != 0)
        {
            uint32_t localCount = count;
            count = 0;

            OpcodeHandler const& handler = Singleton<Opcodes>::Instance()[static_cast<uint16>(opcode)];
            output << metricPathPrefix << "network.packet.sendCount." << handler.name << ' ' << localCount << timestampAndLineEnd;
        }
    }
}
std::array<uint32_t, NUM_MSG_TYPES> MaNGOS::Metric::_Provider::IncrementalCounter::network_packet_recvBytes::g_counterPerOpcode;
void MaNGOS::Metric::_Provider::IncrementalCounter::network_packet_recvBytes::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd)
{
    for (size_t opcode = 0; opcode < g_counterPerOpcode.size(); ++opcode)
    {
        uint32_t& count = g_counterPerOpcode[opcode];
        if (count != 0)
        {
            uint32_t localCount = count;
            count = 0;

            OpcodeHandler const& handler = Singleton<Opcodes>::Instance()[static_cast<uint16>(opcode)];
            output << metricPathPrefix << "network.packet.recvBytes." << handler.name << ' ' << localCount << timestampAndLineEnd;
        }
    }
}

std::array<uint32_t, NUM_MSG_TYPES> MaNGOS::Metric::_Provider::IncrementalCounter::network_packet_sendBytes::g_counterPerOpcode;
void MaNGOS::Metric::_Provider::IncrementalCounter::network_packet_sendBytes::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd)
{
    for (size_t opcode = 0; opcode < g_counterPerOpcode.size(); ++opcode)
    {
        uint32_t& count = g_counterPerOpcode[opcode];
        if (count != 0)
        {
            uint32_t localCount = count;
            count = 0;

            OpcodeHandler const& handler = Singleton<Opcodes>::Instance()[static_cast<uint16>(opcode)];
            output << metricPathPrefix << "network.packet.sendBytes." << handler.name << ' ' << localCount << timestampAndLineEnd;
        }
    }
}

std::array<float, NUM_MSG_TYPES> MaNGOS::Metric::_Provider::ScopedStopwatch::network_packet_processTime::g_maxTimeMsPerOpcode;
void MaNGOS::Metric::_Provider::ScopedStopwatch::network_packet_processTime::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd)
{
    for (size_t opcode = 0; opcode < g_maxTimeMsPerOpcode.size(); ++opcode)
    {
        float& timePerOpcode = g_maxTimeMsPerOpcode[opcode];
        if (timePerOpcode != 0)
        {
            float localTime = timePerOpcode;
            timePerOpcode = 0;

            OpcodeHandler const& handler = Singleton<Opcodes>::Instance()[static_cast<uint16>(opcode)];
            output << metricPathPrefix << "network.packet.processTime." << handler.name << ' ' << localTime << timestampAndLineEnd;
        }
    }
}
