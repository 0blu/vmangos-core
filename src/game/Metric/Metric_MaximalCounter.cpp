#include "./Metric.h"

MaNGOS::Metric::_Provider::MaximalCounter::SessionCount::MaxPlayerCountStats MaNGOS::Metric::_Provider::MaximalCounter::SessionCount::g_stats;
void MaNGOS::Metric::_Provider::MaximalCounter::SessionCount::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd)
{
    uint32_t sessionCount = g_stats.sessionCount;
    g_stats.sessionCount = 0;

    output << metricPathPrefix << "SessionCount " << sessionCount << timestampAndLineEnd;
}

uint32_t MaNGOS::Metric::_Provider::MaximalCounter::LoadedMaps::g_loadedMaps;
void MaNGOS::Metric::_Provider::MaximalCounter::LoadedMaps::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd)
{
    uint32_t loadedMaps = g_loadedMaps;
    g_loadedMaps = 0;

    output << metricPathPrefix << "LoadedMaps " << loadedMaps << timestampAndLineEnd;
}
