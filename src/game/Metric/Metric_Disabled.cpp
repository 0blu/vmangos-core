#include "./Metric.h"
#include "Log.h"

void MaNGOS::Metric::MetricService::Finalize()
{
}

bool MaNGOS::Metric::MetricService::Initialize(InfluxDbCredentials const& credentials, std::string const& realmName)
{
    sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "Metrics are disabled in this build. Please build with -DENABLE_METRICS=On to enable them");
    return false;
}

void MaNGOS::Metric::MetricService::StartSenderThread()
{
}
