#ifndef _MANGOS_METRIC_WAS_INCLUDED_BY_INTERNAL
#error You are somehow using a wrong include. You should use `#include "Metric/Metric.h"`
#endif

/// `_Provider` is a hidden namespace.
/// Please use `MANGOS_METRIC(IncrementalCounter::NewSocketConnection())`
/// instead of `MaNGOS::Metric::_Provider::IncrementalCounter::NewSocketConnection()`
namespace MaNGOS { namespace Metric { namespace _Provider
{

#define DEFINE_SIMPLE_METRIC_IncrementalCounter(className, dbFieldName) namespace IncrementalCounter { \
    class className : public MaNGOS::Policies::NoCopyNoMove \
    { \
    public: \
        explicit className() { g_counter++; } \
        static void WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd); \
    private: \
        static constexpr char const* g_dbFieldName = dbFieldName; \
        static uint64_t g_counter; \
    };}
#define DEFINE_SIMPLE_METRIC_IncrementalCounter_impl(className) uint64_t MaNGOS::Metric::_Provider::IncrementalCounter::className::g_counter; \
    void MaNGOS::Metric::_Provider::IncrementalCounter::className::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd) \
    { \
        uint64_t counter = g_counter; \
        g_counter = 0; \
        output << metricPathPrefix << g_dbFieldName << ' ' << counter << timestampAndLineEnd; \
    }

#define DEFINE_SIMPLE_METRIC_MaximalCounter(className, dbFieldName) namespace MaximalCounter { \
    class className : public MaNGOS::Policies::NoCopyNoMove \
    { \
    public: \
        explicit className(uint64_t currentCount) { if (currentCount > g_counter) g_counter = currentCount;  } \
        static void WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd); \
    private: \
        static constexpr char const* g_dbFieldName = dbFieldName; \
        static uint64_t g_counter; \
    };}
#define DEFINE_SIMPLE_METRIC_MaximalCounter_impl(className) uint64_t MaNGOS::Metric::_Provider::MaximalCounter::className::g_counter; \
    void MaNGOS::Metric::_Provider::MaximalCounter::className::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd) \
    { \
        uint64_t counter = g_counter; \
        g_counter = 0; \
        output << metricPathPrefix << g_dbFieldName << ' ' << counter << timestampAndLineEnd; \
    }

#define DEFINE_SIMPLE_METRIC_ScopedStopwatch(className, dbFieldName) namespace ScopedStopwatch { \
    class className : public MaNGOS::Policies::NoCopyNoMove \
    { \
    public: \
        explicit className() : m_startTime(std::chrono::high_resolution_clock::now()) { } \
        ~className() { \
            auto endTime = std::chrono::high_resolution_clock::now(); \
            auto duration = endTime - m_startTime; \
            float timeInMs = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(duration).count()) / 1000.0f; \
            if (timeInMs > g_maxTimeMs) \
                g_maxTimeMs = timeInMs; \
        } \
        static void WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd); \
    private: \
        std::chrono::high_resolution_clock::time_point m_startTime; \
        static constexpr char const* g_dbFieldName = dbFieldName; \
        static float g_maxTimeMs; \
    };}
#define DEFINE_SIMPLE_METRIC_ScopedStopwatch_impl(className) float MaNGOS::Metric::_Provider::ScopedStopwatch::className::g_maxTimeMs; \
    void MaNGOS::Metric::_Provider::ScopedStopwatch::className::WriteGraphiteLinesToBufferAndResetStats(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd) \
    { \
        float maxTimeMs = g_maxTimeMs; \
        g_maxTimeMs = 0; \
        if (maxTimeMs != 0) \
            output << metricPathPrefix << g_dbFieldName << ' ' << maxTimeMs << timestampAndLineEnd; \
    }

    DEFINE_SIMPLE_METRIC_IncrementalCounter (network_socketEvent_newConnection,         "network.socketEvent.newConnection");
    DEFINE_SIMPLE_METRIC_IncrementalCounter (network_socketEvent_closeConnection,       "network.socketEvent.closeConnection");
    DEFINE_SIMPLE_METRIC_IncrementalCounter (world_update_completeAsyncLoops,           "world.update.completeAsyncLoops");

    DEFINE_SIMPLE_METRIC_MaximalCounter     (server_memAllocatedBytes,                  "server.memAllocatedBytes");
    DEFINE_SIMPLE_METRIC_MaximalCounter     (player_sessionCount,                       "player.sessionCount");
    DEFINE_SIMPLE_METRIC_MaximalCounter     (world_loadedMaps,                          "world.loadedMaps");

    DEFINE_SIMPLE_METRIC_ScopedStopwatch    (world_updateTime_total,                    "world.updateTime.total");
    DEFINE_SIMPLE_METRIC_ScopedStopwatch    (world_updateTime_auctionHouse,             "world.updateTime.auctionHouse");
    DEFINE_SIMPLE_METRIC_ScopedStopwatch    (world_updateTime_updateSessions,           "world.updateTime.updateSessions");
    DEFINE_SIMPLE_METRIC_ScopedStopwatch    (world_updateTime_mapMgrUpdate,             "world.updateTime.mapMgrUpdate");
    DEFINE_SIMPLE_METRIC_ScopedStopwatch    (world_updateTime_battleGroundMgrUpdate,    "world.updateTime.battleGroundMgrUpdate");
    DEFINE_SIMPLE_METRIC_ScopedStopwatch    (world_updateTime_guardMgrUpdate,           "world.updateTime.guardMgrUpdate");
    DEFINE_SIMPLE_METRIC_ScopedStopwatch    (world_updateTime_zoneScriptMgr,            "world.updateTime.zoneScriptMgr");
    DEFINE_SIMPLE_METRIC_ScopedStopwatch    (world_updateTime_asyncQueries,             "world.updateTime.asyncQueries");
    DEFINE_SIMPLE_METRIC_ScopedStopwatch    (world_updateTime_playerBotMgrUpdate,       "world.updateTime.playerBotMgrUpdate");

}}}
