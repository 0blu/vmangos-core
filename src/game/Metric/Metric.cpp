#include "Metric.h"

#include <future>
#include <sstream>
#include <thread>
#include <utility>

#include "Opcodes.h"
#include "IO/Multithreading/CreateThread.h"
#include "IO/Networking/AsyncSocketAcceptor.h"
#include "IO/Networking/SocketConnector.h"
#include "Policies/Singleton.h"

auto constexpr g_metricSendTime = std::chrono::seconds(1); // send new metric values every second

class MetricServiceInstance
{
public:
    explicit MetricServiceInstance(MaNGOS::Metric::MetricService::InfluxDbCredentials credentials, std::string realmName)
        : m_credentials{std::move(credentials)}, m_realmName{std::move(realmName)}
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
        m_senderThread = IO::Multithreading::CreateThreadPtr("Metric", [this]{ ThreadBody(); });
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
        ScopedStopwatch::PacketProcessTime::WriteInfluxLinesToBufferAndResetStats,
        ScopedStopwatch::TotalWorldUpdateTime::WriteInfluxLinesToBufferAndResetStats,
    };

    std::string realmTag = ",realmName=" + m_realmName;
    std::future<void> stopFlag = m_metricSenderThreadStopFlag.get_future();

    while (stopFlag.wait_for(std::chrono::seconds(1)) == std::future_status::timeout)
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

bool MaNGOS::Metric::MetricService::Initialize(InfluxDbCredentials const& credentials, std::string const& realmName)
{
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

// -- IncrementalCounter

uint32_t MaNGOS::Metric::_Provider::IncrementalCounter::NewSocketConnection::g_counter;
void MaNGOS::Metric::_Provider::IncrementalCounter::NewSocketConnection::WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName)
{
    uint32_t counter = g_counter;
    g_counter = 0;

    output << "NewSocketConnection" << afterName << " count=" << counter << '\n';
}

std::array<uint32_t, NUM_MSG_TYPES> MaNGOS::Metric::_Provider::IncrementalCounter::ReceivedPacket::g_recvOpcodeCount;
void MaNGOS::Metric::_Provider::IncrementalCounter::ReceivedPacket::WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName)
{
    for (size_t opcode = 0; opcode < g_recvOpcodeCount.size(); ++opcode)
    {
        uint32_t& count = g_recvOpcodeCount[opcode];
        if (count != 0)
        {
            uint32_t localCount = count;
            count = 0;

            OpcodeHandler const& handler = Singleton<Opcodes>::Instance()[static_cast<uint16>(opcode)];
            output << "ReceivedPacket" << afterName << ",opcode=" << handler.name << " count=" << localCount << '\n';
        }
    }
}

std::array<uint32_t, NUM_MSG_TYPES> MaNGOS::Metric::_Provider::IncrementalCounter::SentPacket::g_sentOpcodeCount;
void MaNGOS::Metric::_Provider::IncrementalCounter::SentPacket::WriteInfluxLinesToBufferAndResetStats(std::stringstream& output, std::string const& afterName)
{
    for (size_t opcode = 0; opcode < g_sentOpcodeCount.size(); ++opcode)
    {
        uint32_t& count = g_sentOpcodeCount[opcode];
        if (count != 0)
        {
            uint32_t localCount = count;
            count = 0;

            OpcodeHandler const& handler = Singleton<Opcodes>::Instance()[static_cast<uint16>(opcode)];
            output << "SentPacket" << afterName << ",opcode=" << handler.name << " count=" << localCount << '\n';
        }
    }
}

// -- ScopedStopwatch

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
