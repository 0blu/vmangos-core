#ifndef _MANGOS_METRIC_H
#define _MANGOS_METRIC_H

#include <string>
#include <chrono>

#include "IO/Context/IoContext.h"

// Welcome to vMangos' Metric implementation!
// This implementation is different to the one you see in cMaNGOS or TrinityCore.
//
// We try to be as non-inverse as possible to the instrumented parts of the code.
// (instrumented = part where MANGOS_METRIC() is inserted)
//
// Most variables are pre-allocated in static storage and take 1 (!!!) assembly instruction.
// The conversion to a string, which ultimately gets sent to the database, is performed in a different thread.
//
// Some measurements allow the use of scoped timers.
// It's the same principle as `std::unique_lock<std::mutex> lock(myMutex);`
// as soon as you leave the scope, the timer is stopped and committed to the measurements.
//
// Since we want to have a uniform interface for all measurements, all of them are inlined constructors.
// Tests have shown that all the unnecessary stuff gets optimized out.
// The usage is held simple with `MANGOS_METRIC()`,
// but it's a bit awkward and boilerplaty if you want to add new measurements.
//
// We opted for GraphiteDB due to the uncertain future of free InfluxDB.
// GraphiteDB is also simpler in terms of dependencies, making it easier to create a docker compose setup from scratch.
// Additionally, it includes built-in data downsampling for maintaining a longer history.

/// Namespace holding all the internal metric stuff.
/// Use the macros below if you want to log values.
namespace MaNGOS { namespace Metric
{
    /// global static functions related to metrics
    namespace MetricService {
        struct GraphiteDbClientConfig
        {
            std::string address;
            uint16_t port;
        };
        /// Initializes metric variables and test the connection
        /// \returns true if successful
        bool Initialize(GraphiteDbClientConfig const& config, std::string const& metricPrefix, IO::IoContext* ioContext, std::chrono::seconds sendingInterval);

        /// Start the sender thread which will repeatably send new metrics to the database
        void StartSenderThread();

        /// Stops and waits for the sender thread
        void Finalize();
    }
}} // MaNGOS::Metric

#ifndef ENABLE_METRICS

/// NOOP metric handler
#define MANGOS_METRIC(...) do {} while(0)

#else

#define _MANGOS_METRIC_VARIABLE_UNIQUE_NAME_DO_CONCAT(a, b) a ## b
#define _MANGOS_METRIC_VARIABLE_UNIQUE_NAME_CONCAT(a, b) _MANGOS_METRIC_VARIABLE_UNIQUE_NAME_DO_CONCAT(a, b)
/// Creates a unique variable name (based on the current line number)
#define _MANGOS_METRIC_VARIABLE_UNIQUE_NAME_UNIQUE_NAME(name) _MANGOS_METRIC_VARIABLE_UNIQUE_NAME_CONCAT(name, __LINE__)

/// Example usage `MANGOS_METRIC(ScopedStopwatch::TotalWorldUpdateTime{});`
#define MANGOS_METRIC(...) auto const& _MANGOS_METRIC_VARIABLE_UNIQUE_NAME_UNIQUE_NAME(_mangos_metric_scope) = ::MaNGOS::Metric::_Provider:: __VA_ARGS__

#include "Policies/ObjectConstructorTraits.h"
#define _MANGOS_METRIC_WAS_INCLUDED_BY_INTERNAL
#include "./Metric_Definitions_Simple.h"
#include "./Metric_Definitions_Complex.h"
#undef _MANGOS_METRIC_WAS_INCLUDED_BY_INTERNAL

#endif

#endif // _MANGOS_METRIC_H
