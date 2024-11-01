#include "./Metric.h"

#include <future>
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

std::string toCarboneSafeRealmName(std::string const& realmName)
{
    // Replace every non-alphanumeric character with _
    static const std::regex invalidChars{ R"([^A-Za-z0-9_])" };
    return std::regex_replace(realmName, invalidChars, "_");
}

class MetricServiceInstance
{
public:
    explicit MetricServiceInstance(MaNGOS::Metric::MetricService::GraphiteDbClientConfig const& config, uint32 realmId)
        : m_metricLinePrefix("vmangos_metric." + std::to_string(realmId) + ".")
    {
        m_dbAddress = ACE_INET_Addr(config.address.c_str(), config.port);
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
    nonstd::optional<ACE_SOCK_Stream>& GetOrReconnectSocket();

    ACE_INET_Addr m_dbAddress;
    nonstd::optional<ACE_SOCK_Stream> m_currentDbSocket;

    std::string const m_metricLinePrefix;
    std::promise<void> m_metricSenderThreadStopFlag;
    std::unique_ptr<std::thread> m_senderThread;

};

bool MetricServiceInstance::TestConnection()
{
    return GetOrReconnectSocket().has_value();
}

void MetricServiceInstance::SendLinesToDatabase(std::string const& lines)
{
    nonstd::optional<ACE_SOCK_Stream>& socket = GetOrReconnectSocket();
    if (!socket.has_value())
    {
        sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Failed to send metric. Unable to connect to db.");
        return;
    }

    ACE_Time_Value timeout(5);
    if (socket->send_n(lines.c_str(), lines.size(), &timeout) == -1)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Failed to send metric. Error while sending.");
        socket.reset(); // Reset, so it can reconnect next time...
        return;
    }

    // Success...
}

void MetricServiceInstance::ThreadBody()
{
    // This function will continuously write to InfluxDB via plaintext LineProtocol (https://docs.influxdata.com/influxdb/cloud/reference/syntax/line-protocol/)
    using namespace MaNGOS::Metric::_Provider;

    typedef void (*WriterFunc)(std::stringstream& output, std::string const& metricPathPrefix, std::string const& timestampAndLineEnd);
    WriterFunc constexpr InfluxWriters[] =
    {
        IncrementalCounter::NewSocketConnection::WriteGraphiteLinesToBufferAndResetStats,
        IncrementalCounter::ReceivedPacket::WriteGraphiteLinesToBufferAndResetStats,
        IncrementalCounter::SentPacket::WriteGraphiteLinesToBufferAndResetStats,
        MaximalCounter::SessionCount::WriteGraphiteLinesToBufferAndResetStats,
        MaximalCounter::LoadedMaps::WriteGraphiteLinesToBufferAndResetStats,
        ScopedStopwatch::PacketProcessTime::WriteGraphiteLinesToBufferAndResetStats,
        ScopedStopwatch::TotalWorldUpdateTime::WriteGraphiteLinesToBufferAndResetStats,
    };

    std::future<void> stopFlag = m_metricSenderThreadStopFlag.get_future();

    while (stopFlag.wait_for(g_metricSendingInterval) == std::future_status::timeout)
    {
        std::string timestampAndLineEnd = " -1\n";

        std::stringstream batchedData;

        for (WriterFunc const& func : InfluxWriters)
        {
            func(batchedData, m_metricLinePrefix, timestampAndLineEnd);
        }

        SendLinesToDatabase(batchedData.str());
    }

    if (m_currentDbSocket.has_value())
        m_currentDbSocket->close();
}

nonstd::optional<ACE_SOCK_Stream>& MetricServiceInstance::GetOrReconnectSocket()
{
    if (m_currentDbSocket.has_value())
        return m_currentDbSocket;

    ACE_INET_Addr myAddr("127.0.0.1:2003");

    ACE_SOCK_Connector connector;
    m_currentDbSocket = nonstd::make_optional<ACE_SOCK_Stream>({});

    ACE_Time_Value timeout(5); // Set timeout to 5 seconds
    if (connector.connect(m_currentDbSocket.value(), myAddr, &timeout) == -1)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "Metric: Fetch failed: connector.connect(...) Error: %d", ACE_OS::last_error());
        m_currentDbSocket.reset();
    }

    return m_currentDbSocket;
}

std::unique_ptr<MetricServiceInstance> g_metricServiceInstance;

void MaNGOS::Metric::MetricService::Finalize()
{
    g_metricServiceInstance.reset();
}

bool MaNGOS::Metric::MetricService::Initialize(GraphiteDbClientConfig const& config, std::chrono::seconds sendingInterval, uint32_t realmId)
{
    g_metricSendingInterval = sendingInterval;
    g_metricServiceInstance = std::make_unique<MetricServiceInstance>(config, realmId);
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
