#include "./Metric.h"

MaNGOS::Metric::_Provider::MaximalCounter::SessionCount::MaxPlayerCountStats MaNGOS::Metric::_Provider::MaximalCounter::SessionCount::g_stats;
void MaNGOS::Metric::_Provider::MaximalCounter::SessionCount::WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName)
{
    uint32_t sessionCount = g_stats.sessionCount;
    g_stats.sessionCount = 0;

    output << "SessionCount" << afterName << " session_count=" << sessionCount << '\n';
}

uint32_t MaNGOS::Metric::_Provider::MaximalCounter::LoadedMaps::g_loadedMaps;
void MaNGOS::Metric::_Provider::MaximalCounter::LoadedMaps::WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName)
{
    uint32_t loadedMaps = g_loadedMaps;
    g_loadedMaps = 0;

    output << "LoadedMaps" << afterName << " loaded_maps=" << loadedMaps << '\n';
}

uint32_t MaNGOS::Metric::_Provider::MaximalCounter::OpenGmTickets::g_gmTicketCount;
void MaNGOS::Metric::_Provider::MaximalCounter::OpenGmTickets::WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName)
{
    uint32_t gmTicketCount = g_gmTicketCount;
    g_gmTicketCount = 0;

    output << "OpenGmTickets" << afterName << " open_gm_ticket=" << gmTicketCount << '\n';
}
