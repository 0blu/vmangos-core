# Windows Dependencies

This folder includes pre-compiled windows dependencies for easy compilation.

## OpenSSL Version
Version: `OpenSSL 3.0.15 LTS`  
Git commit `openssl-3.0.15-0-gc523121f90`

[From Shining Light Productions](https://slproweb.com/products/Win32OpenSSL.html)  
(officially linked in the [OpenSSLWiki](https://wiki.openssl.org/index.php/Binaries))  
(and gives us files which are compatible with Windows XP)

Win64 OpenSSL v3.0.15 [https://slproweb.com/download/Win64OpenSSL-3_0_15.msi](https://web.archive.org/web/20250211193825if_/https://slproweb.com/download/Win64OpenSSL-3_0_15.msi)  
`SHA256: 45fe89ed631f29a5d2178b68defd9f72a3ec81ae48a0f70cf9a4716392c0b7e9`

Win32 OpenSSL v3.0.15 [https://slproweb.com/download/Win32OpenSSL-3_0_15.msi](https://web.archive.org/web/20250211194259if_/https://slproweb.com/download/Win32OpenSSL-3_0_15.msi)  
`SHA256: 5559576257388df2f6fa694aa41397ec016c7486a581311fcb09d474be9d073e`

^ using archive.org links since the site is removing old installers :/

## Copied files
```
# Include files
<dir>/include/                            -> include/

# 32 bit debug
<dir>/lib/VC/x86/MDd/libcrypto_static.lib -> lib/win32_debug/libcrypto_static.lib
<dir>/lib/VC/x86/MDd/libssl_static.lib    -> lib/win32_debug/libssl_static.lib

# 32 bit release
<dir>/lib/VC/x86/MD/libcrypto_static.lib  -> lib/win32_release/libcrypto_static.lib
<dir>/lib/VC/x86/MD/libssl_static.lib     -> lib/win32_release/libssl_static.lib

# 64bit debug
<dir>/lib/VC/x64/MDd/libcrypto_static.lib -> lib/x64_debug/libcrypto_static.lib
<dir>/lib/VC/x64/MDd/libssl_static.lib    -> lib/x64_debug/libssl_static.lib

# 64bit release
<dir>/lib/VC/x64/MD/libcrypto_static.lib  -> lib/x64_release/libcrypto_static.lib
<dir>/lib/VC/x64/MD/libssl_static.lib     -> lib/x64_release/libssl_static.lib
```
