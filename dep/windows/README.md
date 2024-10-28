# Windows Dependencies

This folder includes pre-compiled windows dependencies for easy compilation.

# Used Version

## OpenSSL
Version: OpenSSL 3.0.15 LTS
Git commit openssl-3.0.15-0-gc523121f90

[From Shining Light Productions](https://slproweb.com/products/Win32OpenSSL.html)
(officially linked in the [OpenSSLWiki](https://wiki.openssl.org/index.php/Binaries))
(and gives us DLLs which are compatible with Windows XP)

Win64 OpenSSL v3.0.15 https://slproweb.com/download/Win64OpenSSL-3_0_15.msi  
`SHA256: 45fe89ed631f29a5d2178b68defd9f72a3ec81ae48a0f70cf9a4716392c0b7e9`

Win32 OpenSSL v3.0.15 https://slproweb.com/download/Win32OpenSSL-3_0_15.msi  
`SHA256: 5559576257388df2f6fa694aa41397ec016c7486a581311fcb09d474be9d073e`

Copied files:
```
include/                         -> include/

# 32 bit debug
lib/VC/x86/MDd/libcrypto_static.lib  -> lib/win32_debug/libcrypto_static.lib
lib/VC/x86/MDd/libssl_static.lib     -> lib/win32_debug/libssl_static.lib
bin/legacy.dll                       -> lib/win32_debug/legacy.dll

# 32 bit release
lib/VC/x86/MD/libcrypto_static.lib   -> lib/win32_release/libcrypto_static.lib
lib/VC/x86/MD/libssl_static.lib      -> lib/win32_release/libssl_static.lib
bin/legacy.dll                       -> lib/win32_release/legacy.dll

# 64bit debug
lib/VC/x64/MDd/libcrypto_static.lib  -> lib/x64_debug/libcrypto_static.lib
lib/VC/x64/MDd/libssl_static.lib     -> lib/x64_debug/libssl_static.lib
bin/legacy.dll                       -> lib/x64_debug/legacy.dll

# 64bit release
lib/VC/x64/MD/libcrypto_static.lib   -> lib/x64_release/libcrypto_static.lib
lib/VC/x64/MD/libssl_static.lib      -> lib/x64_release/libssl_static.lib
bin/legacy.dll                       -> lib/x64_release/legacy.dll
```
