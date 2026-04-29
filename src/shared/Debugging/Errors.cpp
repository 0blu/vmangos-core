#include "Debugging/Errors.h"
#include "../Log.h"

#include "Platform/CompilerDefs.h"

#ifdef ENABLE_CPPTRACE
#include <cpptrace/cpptrace.hpp>
#endif

#if PLATFORM == PLATFORM_WINDOWS
#include <intrin.h>
#include <Windows.h>

#define CUSTOM_EXCEPTION_ASSERTION_FAILURE 0xC0000420L // Keep in sync with WheatyExceptionReport.cpp

[[noreturn]]
static void PerformCrash(std::string const& message) {
    ULONG_PTR exceptionArgs[] = {
        reinterpret_cast<ULONG_PTR>(_strdup(message.c_str())),
        reinterpret_cast<ULONG_PTR>(_ReturnAddress())
    };
    RaiseException(CUSTOM_EXCEPTION_ASSERTION_FAILURE, 0, 2, exceptionArgs);
    exit(1);
}
#else
// should be easily accessible in gdb
extern "C" { char const* vMangosAssertionFailedMessage = nullptr; }
[[noreturn]]
static void PerformCrash(std::string const& message) {
    vMangosAssertionFailedMessage = strdup(message.c_str());
    *((volatile int*)nullptr) = 0;
    exit(1);
}
#endif

#if PLATFORM == PLATFORM_WINDOWS
#include "Windows/WheatyExceptionReport.h"
void MaNGOS::Errors::RegisterCrashHandler()
{
    g_WheatyExceptionReport.EnsureRegistration(); // Ensures the usage of this variable
}
#else
void MaNGOS::Errors::RegisterCrashHandler()
{
    /* Currently not supported */
}
#endif

void MaNGOS::Errors::PrintStacktrace()
{
    PrintStacktrace(1, 64);
}

void MaNGOS::Errors::PrintStacktrace(int skipFrames, int maxFrames)
{
#ifndef ENABLE_CPPTRACE
    sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Stack traces using cpptrace are disabled. Set ENABLE_CPPTRACE=ON (default) during CMake configuration to enable them.");
#else
    cpptrace::stacktrace st = cpptrace::generate_trace(
        std::size_t(skipFrames) + 1, // we want to skip our own frame
        std::size_t(maxFrames)
    );

    bool hasStacktraceInfo = false;
    for (size_t i = 0; i < st.frames.size(); ++i)
    {
        cpptrace::stacktrace_frame const& trace = st.frames[i];
        if (trace.line.has_value())
        {
            sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL,
                "#%zu [0x%" PRIXPTR "] %s %s:%u",
                i,
                trace.object_address,
                trace.symbol.c_str(),
                trace.filename.c_str(),
                trace.line.value()
            );
        }
        else
        {
            // without line
            sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL,
                "#%zu [0x%" PRIXPTR "] %s %s",
                i,
                trace.object_address,
                trace.symbol.c_str(),
                trace.filename.c_str()
            );
        }

        if (!hasStacktraceInfo && trace.line.has_value() && !trace.symbol.empty())
        {
            // we assume there are symbols if at least one frame was parsed successfully
            hasStacktraceInfo = true;
        }
    }

    if (!hasStacktraceInfo)
    {
        // without line
#if PLATFORM == PLATFORM_WINDOWS
        sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "Missing debug symbols. Place an up-to-date PDB file next to executable and/or build with debug symbols.");
#else
        sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "Missing debug symbols. Please build with debug symbols.");
#endif
    }
#endif
}

[[noreturn]]
void MaNGOS::Errors::PrintStacktraceAndThrow(char const* filename, int line, char const* functionName, char const* failedExpression, char const* message)
{
    if (message)
        sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "%s:%i Error: Assertion in %s: %s (%s)", filename, line, functionName, failedExpression, message);
    else
        sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "%s:%i Error: Assertion in %s: %s", filename, line, functionName, failedExpression);

    MaNGOS::Errors::PrintStacktrace(1, 32);

    std::string completeMessage = failedExpression;
    if (message)
        completeMessage += std::string(" Message: ") + message;

    PerformCrash(completeMessage);
}
