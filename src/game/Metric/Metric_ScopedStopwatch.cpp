#include "./Metric.h"

std::array<float, NUM_MSG_TYPES> MaNGOS::Metric::_Provider::ScopedStopwatch::PacketProcessTime::g_maxTimeMsPerOpcode;
void MaNGOS::Metric::_Provider::ScopedStopwatch::PacketProcessTime::WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName)
{
    for (size_t opcode = 0; opcode < g_maxTimeMsPerOpcode.size(); ++opcode)
    {
        float& timePerOpcode = g_maxTimeMsPerOpcode[opcode];
        if (timePerOpcode != 0)
        {
            float localTime = timePerOpcode;
            timePerOpcode = 0;

            OpcodeHandler const& handler = Singleton<Opcodes>::Instance()[static_cast<uint16>(opcode)];
            output << "PacketProcessTime" << afterName << ",opcode=" << handler.name << " max_time=" << localTime << '\n';
        }
    }
}

float MaNGOS::Metric::_Provider::ScopedStopwatch::TotalWorldUpdateTime::g_maxUpdateTimeMs;
void MaNGOS::Metric::_Provider::ScopedStopwatch::TotalWorldUpdateTime::WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName)
{
    float updateTimeMs = g_maxUpdateTimeMs;
    g_maxUpdateTimeMs = 0;

    output << "TotalWorldUpdateTime" << afterName << " max_time_ms=" << updateTimeMs << '\n';
}
