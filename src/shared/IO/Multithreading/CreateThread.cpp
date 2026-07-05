#include "CreateThread.h"

#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#include <pthread.h>
#endif

std::unique_ptr<std::thread> IO::Multithreading::CreateThreadPtr(std::string const& name, std::function<void()> entryFunction)
{
    return std::make_unique<std::thread>([name, entryFunction = std::move(entryFunction)]()
    {
       IO::Multithreading::RenameCurrentThread(name);
       entryFunction();
    });
}

std::thread IO::Multithreading::CreateThread(std::string const& name, std::function<void()> entryFunction)
{
    return std::thread([name, entryFunction = std::move(entryFunction)]()
    {
        IO::Multithreading::RenameCurrentThread(name);
        entryFunction();
    });
}

#if defined(WIN32) && !defined(__MINGW32__)
// SEH must live in its own function — MSVC C2712 forbids __try in functions
// that have C++ objects requiring stack unwinding (e.g. std::wstring).
static void RenameCurrentThreadSEH(char const* name)
{
    const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)
    typedef struct tagTHREADNAME_INFO
    {
        DWORD dwType;       // Must be 0x1000.
        LPCSTR szName;      // Pointer to name (in user addr space).
        DWORD dwThreadID;   // Thread ID (-1=caller thread).
        DWORD dwFlags;      // Reserved for future use, must be zero.
    } THREADNAME_INFO;
#pragma pack(pop)

    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = name;
    info.dwThreadID = GetCurrentThreadId();
    info.dwFlags = 0;

    __try
    {
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
    }
}
#endif

void IO::Multithreading::RenameCurrentThread(std::string const& name)
{
#if defined(WIN32)
    // Prefer SetThreadDescription (Win10+): stored in the OS, visible in any debugger and crash dumps.
    // Load dynamically so the binary still runs on older Windows.
    typedef HRESULT (WINAPI *PFN_SetThreadDescription)(HANDLE, PCWSTR);
    static auto const pSetThreadDescription = reinterpret_cast<PFN_SetThreadDescription>(
        reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA("kernel32.dll"), "SetThreadDescription")));
    if (pSetThreadDescription)
    {
        int const wlen = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, nullptr, 0);
        if (wlen > 0)
        {
            std::wstring wname(wlen - 1, L'\0');
            MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, wname.data(), wlen);
            pSetThreadDescription(GetCurrentThread(), wname.c_str());
        }
    }
#if !defined(__MINGW32__)
    else
    {
        // Fallback for pre-Win10: raise a named exception that Visual Studio's debugger intercepts.
        // Thread naming is a debugging convenience only; skipping it has no runtime impact.
        RenameCurrentThreadSEH(name.c_str());
    }
#endif // !defined(__MINGW32__)
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    ::pthread_setname_np(pthread_self(), name.c_str());
#elif defined(__APPLE__)
    ::pthread_setname_np(name.c_str());
#else
    // It's not too serisous if we cant rename a thread
    #warning "IO::Multithreading::_renameThisThread not supported on your platform"
#endif
}
