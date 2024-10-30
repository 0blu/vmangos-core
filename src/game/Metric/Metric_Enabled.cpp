#include "./Metric.h"

#include <future>
#include <sstream>
#include <thread>
#include <utility>
#include <curl/curl.h>

#include "Opcodes.h"
#include "Policies/Singleton.h"

std::chrono::seconds g_metricSendingInterval(1);

std::string toInfluxSafeRealmName(std::string realmName)
{
    std::replace(realmName.begin(), realmName.end(), '\'', '_');
    return realmName;
}

class MetricServiceInstance
{
public:
    explicit MetricServiceInstance(MaNGOS::Metric::MetricService::InfluxDbCredentials credentials, std::string realmName)
        : m_credentials{std::move(credentials)}, m_realmName{toInfluxSafeRealmName(realmName)}
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
    void StartSendThread()
    {
        // TODO: Name Thread "MetricSender"
        m_senderThread = std::make_unique<std::thread>([this]{ ThreadBody(); });
    }

private:
    void SendLinesToDatabase(const std::string& str);
    void ThreadBody();

    MaNGOS::Metric::MetricService::InfluxDbCredentials m_credentials;
    std::string m_realmName;
    std::promise<void> m_metricSenderThreadStopFlag;
    std::unique_ptr<std::thread> m_senderThread;
};

bool MetricServiceInstance::TestConnection()
{
    return false;
}

void MetricServiceInstance::SendLinesToDatabase(std::string const& lines)
{
    sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "%s", lines.c_str());
    CURL *curl = curl_easy_init();
    if (curl)
    {
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: text/plain");
        headers = curl_slist_append(headers, "Authorization: Token V6MaPyZ_kpNecXAAMHKeR79aiIRAcsJCvDnoWa6kutdnAlBiSUJCX2AruyzVDMCqAZn6xgZghWzrtf8mO5yJzw==");

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8086/api/v2/write?org=vmangos&bucket=metrics&precision=s");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, lines.c_str());

        CURLcode res = curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

void MetricServiceInstance::ThreadBody()
{
    // This function will continuously write to InfluxDB via plaintext LineProtocol (https://docs.influxdata.com/influxdb/cloud/reference/syntax/line-protocol/)
    using namespace MaNGOS::Metric::_Provider;

    typedef void (*WriterFunc)(std::stringstream& output, std::string const& afterName);
    WriterFunc constexpr InfluxWriters[] =
    {
        IncrementalCounter::NewSocketConnection::WriteInfluxLinesToBufferAndResetStats,
        IncrementalCounter::ReceivedPacket::WriteInfluxLinesToBufferAndResetStats,
        IncrementalCounter::SentPacket::WriteInfluxLinesToBufferAndResetStats,
        MaximalCounter::SessionCount::WriteInfluxLinesToBufferAndResetStats,
        MaximalCounter::LoadedMaps::WriteInfluxLinesToBufferAndResetStats,
        ScopedStopwatch::PacketProcessTime::WriteInfluxLinesToBufferAndResetStats,
        ScopedStopwatch::TotalWorldUpdateTime::WriteInfluxLinesToBufferAndResetStats,
    };

    std::string realmTag = "";//",realmName=\"" + m_realmName + '\"';
    std::future<void> stopFlag = m_metricSenderThreadStopFlag.get_future();

    while (stopFlag.wait_for(g_metricSendingInterval) == std::future_status::timeout)
    {
        std::stringstream batchedData;

        for (auto func : InfluxWriters)
        {
            func(batchedData, realmTag);
        }

        SendLinesToDatabase(batchedData.str());
    }
}

std::unique_ptr<MetricServiceInstance> g_metricServiceInstance;

void MaNGOS::Metric::MetricService::Finalize()
{
    g_metricServiceInstance.reset();
}

bool MaNGOS::Metric::MetricService::Initialize(InfluxDbCredentials const& credentials, std::chrono::seconds sendingInterval, std::string const& realmName)
{
    g_metricSendingInterval = sendingInterval;
    g_metricServiceInstance = std::make_unique<MetricServiceInstance>(credentials, realmName);
    bool selfTestWasSuccessful = g_metricServiceInstance->TestConnection();
    return selfTestWasSuccessful;
}

void MaNGOS::Metric::MetricService::StartSenderThread()
{
    if (g_metricServiceInstance)
    {
        // we start the thread even if there was an self test error
        // maybe the connection will magically fix itself while the server is running
        g_metricServiceInstance->StartSendThread();
    }
}
