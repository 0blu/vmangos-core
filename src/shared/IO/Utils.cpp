#include "Utils.h"

#if defined(WIN32)
#include <Windows.h>
#include <psapi.h>
#else
#include <unistd.h>
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
#ifdef WIN32
    PROCESS_MEMORY_COUNTERS_EX pmc;
    ::GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc));
    return pmc.PrivateUsage;
#else
    #error TODO cache static: `fd = open("/proc/self/statm", O_RDONLY);`
#endif
}
