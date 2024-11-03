#include "./Metric.h"
#include "Log.h"

void MaNGOS::Metric::MetricService::Finalize()
{
}

bool MaNGOS::Metric::MetricService::Initialize(GraphiteDbClientConfig const& config, std::string const& metricPrefix, std::chrono::seconds sendingInterval)
{
    sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "Metrics are disabled in this build. Please build with -DENABLE_METRICS=On to enable them");
    return false;
}

void MaNGOS::Metric::MetricService::StartSenderThread()
{
}
