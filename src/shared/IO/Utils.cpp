#include "Utils.h"

#if defined(WIN32)
#include <Windows.h>
#include <psapi.h>
#elif defined(__linux__)
#include <unistd.h>
#include <sys/resource.h>
#endif

uint64_t IO::Utils::GetCurrentProcessId()
{
#ifdef WIN32
    return ::GetCurrentProcessId();
#else
    return ::getpid();
#endif
}

uint64_t IO::Utils::GetCurrentProcessAmountOfAllocatedBytes()
{
#if defined(WIN32)
    PROCESS_MEMORY_COUNTERS_EX pmc;
    ::GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc));
    return pmc.PrivateUsage;
#elif defined(__linux__)
    rusage usage;
    if (::getrusage(RUSAGE_SELF, &usage) != 0)
        return 0;

    return usage.ru_maxrss * 1024; // Convert from kilobytes to bytes
#elif defined(__APPLE__)
    #error "TODO"
#else
#error "IO::Utils::GetCurrentProcessAmountOfAllocatedBytes() not supported on your platform"
#endif
}
