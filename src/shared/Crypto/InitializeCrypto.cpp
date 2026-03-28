#include "InitializeCrypto.h"

#include "Authentication/SRP6.h"
#include "Platform/CompilerDefs.h"
#include "Log.h"

#include <openssl/crypto.h>

#include "IO/Filesystem/FileHandle.h"
#include "IO/Filesystem/FileSystem.h"
#if defined(OPENSSL_VERSION_MAJOR) && (OPENSSL_VERSION_MAJOR >= 3)
#include <openssl/provider.h>
#endif

bool Crypto::InitializeCryptoAndPrintVersion()
{
    sLog.Out(LOG_BASIC, LOG_LVL_DETAIL, "%s (Library: %s)", OPENSSL_VERSION_TEXT, SSLeay_version(SSLEAY_VERSION));
    return true;

#if defined(OPENSSL_VERSION_MAJOR) && (OPENSSL_VERSION_MAJOR >= 3)
    if (!OSSL_PROVIDER_available(nullptr, "legacy"))
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "OpenSSL: legacy provide is not available. Trying to load it...");
        if (!OSSL_PROVIDER_load(nullptr, "legacy"))
        {
            sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "OpenSSL: legacy provide load failed. Retrying after setting CWD...");
            std::string cwd = IO::Filesystem::ToAbsolutePath(".");

            if (!OSSL_PROVIDER_set_default_search_path(nullptr, cwd.c_str()))
            {
                sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "OpenSSL: failed to set provider default search path to CWD");
            }

            if (!OSSL_PROVIDER_load(nullptr, "legacy"))
            {
                sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "OpenSSL: Failed to load openssl legacy provider.");
                sLog.WaitBeforeContinueIfNeed();
                return false;
            }
        }
    }
#endif

    return true;
}
