#include "./Metric.h"

#include <future>
#include <iomanip>
#include <regex>
#include <sstream>
#include <thread>
#include <utility>
#include <nonstd/optional.hpp>

#include "Opcodes.h"
#include "Policies/Singleton.h"

#include <ace/INET_Addr.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>

std::chrono::seconds g_metricSendingInterval(1);

class MetricServiceInstance
{
public:
    explicit MetricServiceInstance(MaNGOS::Metric::MetricService::GraphiteDbClientConfig const& config, std::string const& metricPrefix)
        : m_config(config), m_metricLinePrefix(metricPrefix)
    {
    }
    ~MetricServiceInstance()
    {
        m_metricSenderThreadStopFlag.set_value();
        if (m_senderThread)
        {
            m_senderThread->join();
            m_senderThread.reset();
        }
    }
    bool TestConnection();
    void StartMetricSender()
    {
        m_senderThread = std::make_unique<std::thread>([this]{ ThreadBody(); });
    }

private:
    void SendLinesToDatabase(std::string const& lines);
    void ThreadBody();
    /// This function will write to GraphiteDB via plaintext protocol
    /// (https://graphite.readthedocs.io/en/latest/feeding-carbon.html#the-plaintext-protocol)
    void CollectMetricsAndSendOnce();

    std::unique_ptr<ACE_SOCK_Stream>& GetOrReconnectSocketPtrRef();

    MaNGOS::Metric::MetricService::GraphiteDbClientConfig m_config;
    std::unique_ptr<ACE_SOCK_Stream> m_currentDbSocket;

    std::string const m_metricLinePrefix;
    std::promise<void> m_metricSenderThreadStopFlag;
    std::unique_ptr<std::thread> m_senderThread;

};

bool MetricServiceInstance::TestConnection()
{
    return GetOrReconnectSocketPtrRef() != nullptr;
}

void MetricServiceInstance::SendLinesToDatabase(std::string const& lines)
{
    std::unique_ptr<ACE_SOCK_Stream>& socket = GetOrReconnectSocketPtrRef();
    if (socket == nullptr)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Metric: Failed to send metric. Unable to connect to db.");
        return;
    }

    ACE_Time_Value timeout(5);
    if (socket->send_n(lines.c_str(), lines.size(), &timeout) == -1)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Metric: Failed to send metric. Error while sending.");
        socket.reset(); // Reset, so it can reconnect next time...
        return;
    }

    // Success...
}

void MetricServiceInstance::ThreadBody()
{
    std::future<void> stopFlag = m_metricSenderThreadStopFlag.get_future();

    std::chrono::time_point<std::chrono::steady_clock> nextTick = std::chrono::steady_clock::now();
    while (stopFlag.wait_until(nextTick) == std::future_status::timeout)
    {
        CollectMetricsAndSendOnce();

        nextTick += g_metricSendingInterval;
        auto now = std::chrono::steady_clock::now();
        if (nextTick < now)
        {
            sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Metric: Loop is out of sync... Skipping wait time");

            // Align to the next sync by calculating the offset
            auto missedIntervals = (now - nextTick) / g_metricSendingInterval;
            auto adjustmentOffset = (missedIntervals + 1) * g_metricSendingInterval;
            nextTick += adjustmentOffset;
        }
    }

    if (m_currentDbSocket != nullptr)
        m_currentDbSocket->close();
}

void MetricServiceInstance::CollectMetricsAndSendOnce()
{
    using namespace MaNGOS::Metric::_Provider;

    typedef void (*WriterFunc)(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd);
    WriterFunc constexpr InfluxWriters[] =
    {
        // complex
        IncrementalCounter::network_packet_recvBytes::WriteGraphiteLinesToBufferAndResetStats,
        IncrementalCounter::network_packet_recvCount::WriteGraphiteLinesToBufferAndResetStats,
        IncrementalCounter::network_packet_sendBytes::WriteGraphiteLinesToBufferAndResetStats,
        IncrementalCounter::network_packet_sendCount::WriteGraphiteLinesToBufferAndResetStats,

        ScopedStopwatch::network_packet_processTime::WriteGraphiteLinesToBufferAndResetStats,

        // simple
        IncrementalCounter::network_socketEvent_newConnection::WriteGraphiteLinesToBufferAndResetStats,
        IncrementalCounter::network_socketEvent_closeConnection::WriteGraphiteLinesToBufferAndResetStats,
        IncrementalCounter::world_update_completeAsyncLoops::WriteGraphiteLinesToBufferAndResetStats,

        MaximalCounter::server_memAllocatedBytes::WriteGraphiteLinesToBufferAndResetStats,
        MaximalCounter::player_sessionCount::WriteGraphiteLinesToBufferAndResetStats,
        MaximalCounter::world_loadedMaps::WriteGraphiteLinesToBufferAndResetStats,

        ScopedStopwatch::world_updateTime_total::WriteGraphiteLinesToBufferAndResetStats,
        ScopedStopwatch::world_updateTime_auctionHouse::WriteGraphiteLinesToBufferAndResetStats,
        ScopedStopwatch::world_updateTime_updateSessions::WriteGraphiteLinesToBufferAndResetStats,
        ScopedStopwatch::world_updateTime_mapMgrUpdate::WriteGraphiteLinesToBufferAndResetStats,
        ScopedStopwatch::world_updateTime_battleGroundMgrUpdate::WriteGraphiteLinesToBufferAndResetStats,
        ScopedStopwatch::world_updateTime_guardMgrUpdate::WriteGraphiteLinesToBufferAndResetStats,
        ScopedStopwatch::world_updateTime_zoneScriptMgr::WriteGraphiteLinesToBufferAndResetStats,
        ScopedStopwatch::world_updateTime_playerBotMgrUpdate::WriteGraphiteLinesToBufferAndResetStats,
    };

    std::string timestampAndLineEnd = " -1\n"; // -1 means current time of database

    std::stringstream batchedData;

    for (WriterFunc const& func : InfluxWriters)
    {
        func(batchedData, m_metricLinePrefix, timestampAndLineEnd);
    }

    SendLinesToDatabase(batchedData.str());
}

std::unique_ptr<ACE_SOCK_Stream>& MetricServiceInstance::GetOrReconnectSocketPtrRef()
{
    if (m_currentDbSocket != nullptr)
        return m_currentDbSocket;

    ACE_SOCK_Connector connector;
    m_currentDbSocket = std::make_unique<ACE_SOCK_Stream>();

    ACE_INET_Addr targetAddress((m_config.address + ":" + std::to_string(m_config.port)).c_str());

    ACE_Time_Value timeout(5); // Set timeout to 5 seconds
    if (connector.connect(*m_currentDbSocket, targetAddress, &timeout) == -1)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "Metric: Failed to connect to database. Connect error: Error: %d", ACE_OS::last_error());
        m_currentDbSocket.reset();
    }

    return m_currentDbSocket;
}

std::unique_ptr<MetricServiceInstance> g_metricServiceInstance;

void MaNGOS::Metric::MetricService::Finalize()
{
    g_metricServiceInstance.reset();
}

bool MaNGOS::Metric::MetricService::Initialize(GraphiteDbClientConfig const& config, std::string const& metricPrefix, std::chrono::seconds sendingInterval)
{
    g_metricSendingInterval = sendingInterval;
    g_metricServiceInstance = std::make_unique<MetricServiceInstance>(config, metricPrefix);
    bool selfTestWasSuccessful = g_metricServiceInstance->TestConnection();
    return selfTestWasSuccessful;
}

void MaNGOS::Metric::MetricService::StartSenderThread()
{
    if (g_metricServiceInstance)
    {
        // we start the thread even if there was an self test error
        // maybe the connection will magically fix itself while the server is running
        g_metricServiceInstance->StartMetricSender();
    }
}
