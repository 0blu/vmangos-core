# Windows Dependencies

This folder includes pre-compiled windows dependencies for easy compilation.

## OpenSSL Version
Version: `OpenSSL 3.0.19 LTS`  
Git tag [`openssl-3.0.19`](https://github.com/openssl/openssl/releases/tag/openssl-3.0.19)

Win32 OpenSSL v3.0.19 [https://slproweb.com/download/Win32OpenSSL-3_0_19.msi](https://web.archive.org/web/20260219074231if_/https://slproweb.com/download/Win32OpenSSL-3_0_19.msi)  
`SHA256: 4105c3b7b8a1fbfab35b1427ab692abbddb26edf779ec8cb786e9adbaacd7ba8`

Win64 OpenSSL v3.0.19 [https://slproweb.com/download/Win64OpenSSL-3_0_19.msi](https://web.archive.org/web/20260201081127if_/https://slproweb.com/download/Win64OpenSSL-3_0_19.msi)  
`SHA256: e41c9f2f1d22a2fc38dcf0cf641fd2f8ac11e77da51a8fb60b5f8acf2745115d`

^ using archive.org links since the site is removing old installers. :/  
[From Shining Light Productions](https://slproweb.com/products/Win32OpenSSL.html)  
(officially linked in the [OpenSSLWiki](https://wiki.openssl.org/index.php/Binaries))  
(and gives us files which are compatible with Windows XP)

## Copied files
```
# Include files
<dir>/include/                            -> include/

# 32 bit release
<dir>/lib/VC/x86/MT/libcrypto_static.lib  -> lib/win32/libcrypto_static.lib
<dir>/lib/VC/x86/MT/libssl_static.lib     -> lib/win32/libssl_static.lib

# 64 bit release
<dir>/lib/VC/x64/MT/libcrypto_static.lib  -> lib/x64/libcrypto_static.lib
<dir>/lib/VC/x64/MT/libssl_static.lib     -> lib/x64/libssl_static.lib
```
