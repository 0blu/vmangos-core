#include "./Metric.h"

std::array<float, NUM_MSG_TYPES> MaNGOS::Metric::_Provider::ScopedStopwatch::PacketProcessTime::g_maxTimeMsPerOpcode;
void MaNGOS::Metric::_Provider::ScopedStopwatch::PacketProcessTime::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd)
{
    for (size_t opcode = 0; opcode < g_maxTimeMsPerOpcode.size(); ++opcode)
    {
        float& timePerOpcode = g_maxTimeMsPerOpcode[opcode];
        if (timePerOpcode != 0)
        {
            float localTime = timePerOpcode;
            timePerOpcode = 0;

            OpcodeHandler const& handler = Singleton<Opcodes>::Instance()[static_cast<uint16>(opcode)];
            output << metricPathPrefix << "PacketProcessTime." << handler.name << ' ' << localTime << timestampAndLineEnd;
        }
    }
}

float MaNGOS::Metric::_Provider::ScopedStopwatch::TotalWorldUpdateTime::g_maxUpdateTimeMs;
void MaNGOS::Metric::_Provider::ScopedStopwatch::TotalWorldUpdateTime::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd)
{
    float updateTimeMs = g_maxUpdateTimeMs;
    g_maxUpdateTimeMs = 0;

    output << metricPathPrefix << "TotalWorldUpdateTime" << ' ' << updateTimeMs << timestampAndLineEnd;
}
