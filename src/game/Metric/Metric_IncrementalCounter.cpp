#include "./Metric.h"

uint32_t MaNGOS::Metric::_Provider::IncrementalCounter::NewSocketConnection::g_counter;
void MaNGOS::Metric::_Provider::IncrementalCounter::NewSocketConnection::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd)
{
    uint32_t counter = g_counter;
    g_counter = 0;

    output << metricPathPrefix << "NewSocketConnection " << counter << timestampAndLineEnd;
}

std::array<uint32_t, NUM_MSG_TYPES> MaNGOS::Metric::_Provider::IncrementalCounter::ReceivedPacket::g_recvOpcodeCount;
void MaNGOS::Metric::_Provider::IncrementalCounter::ReceivedPacket::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd)
{
    for (size_t opcode = 0; opcode < g_recvOpcodeCount.size(); ++opcode)
    {
        uint32_t& count = g_recvOpcodeCount[opcode];
        if (count != 0)
        {
            uint32_t localCount = count;
            count = 0;

            OpcodeHandler const& handler = Singleton<Opcodes>::Instance()[static_cast<uint16>(opcode)];
            output << metricPathPrefix << "ReceivedPacket." << handler.name << ' ' << localCount << timestampAndLineEnd;
        }
    }
}

std::array<uint32_t, NUM_MSG_TYPES> MaNGOS::Metric::_Provider::IncrementalCounter::SentPacket::g_sentOpcodeCount;
void MaNGOS::Metric::_Provider::IncrementalCounter::SentPacket::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd)
{
    for (size_t opcode = 0; opcode < g_sentOpcodeCount.size(); ++opcode)
    {
        uint32_t& count = g_sentOpcodeCount[opcode];
        if (count != 0)
        {
            uint32_t localCount = count;
            count = 0;

            OpcodeHandler const& handler = Singleton<Opcodes>::Instance()[static_cast<uint16>(opcode)];
            output << metricPathPrefix << "SentPacket." << handler.name << ' ' << localCount << timestampAndLineEnd;
        }
    }
}
