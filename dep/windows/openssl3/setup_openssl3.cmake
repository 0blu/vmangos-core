set(BUNDLED_OPENSSL3_VERSION "openssl3-3.0.19")
set(BUNDLED_OPENSSL3_BUNDLE "bundle_${BUNDLED_OPENSSL3_VERSION}.zip")
set(BUNDLED_OPENSSL3_VERSION_FILE "${CMAKE_CURRENT_LIST_DIR}/extracted_bundle/version.txt")
set(BUNDLED_OPENSSL3_NEEDS_EXTRACT FALSE)

if(NOT EXISTS "${BUNDLED_OPENSSL3_VERSION_FILE}")
    set(BUNDLED_OPENSSL3_NEEDS_EXTRACT TRUE)
    message(STATUS "Current bundled OpenSSL3 version : <none>")
    message(STATUS "Required bundled OpenSSL3 version: ${BUNDLED_OPENSSL3_VERSION}")
else()
    file(READ "${BUNDLED_OPENSSL3_VERSION_FILE}" BUNDLED_OPENSSL3_CURRENT_VERSION)
    string(STRIP "${BUNDLED_OPENSSL3_CURRENT_VERSION}" BUNDLED_OPENSSL3_CURRENT_VERSION)
    if(NOT BUNDLED_OPENSSL3_CURRENT_VERSION STREQUAL "${BUNDLED_OPENSSL3_VERSION}")
        message(STATUS "Current bundled OpenSSL3 version : ${BUNDLED_OPENSSL3_CURRENT_VERSION}")
        message(STATUS "Required bundled OpenSSL3 version: ${BUNDLED_OPENSSL3_VERSION}")
        set(BUNDLED_OPENSSL3_NEEDS_EXTRACT TRUE)
    else()
        message(STATUS "Bundled OpenSSL3 is up to date (version ${BUNDLED_OPENSSL3_CURRENT_VERSION}). No extraction needed.")
    endif()
endif()

if(BUNDLED_OPENSSL3_NEEDS_EXTRACT)
    if(CMAKE_VERSION VERSION_LESS "3.18")
        message(FATAL_ERROR
            "CMake 3.18+ is required to auto-extract ${BUNDLED_OPENSSL3_BUNDLE}. "
            "Please manually extract ${BUNDLED_OPENSSL3_BUNDLE} into "
            "${CMAKE_CURRENT_LIST_DIR}/extracted_bundle"
            "or upgrade CMake to 3.18+.")
    endif()
    message(STATUS "Extracting ${BUNDLED_OPENSSL3_BUNDLE}...")
    file(REMOVE_RECURSE "${CMAKE_CURRENT_LIST_DIR}/extracted_bundle")
    file(ARCHIVE_EXTRACT INPUT "${CMAKE_CURRENT_LIST_DIR}/${BUNDLED_OPENSSL3_BUNDLE}"
         DESTINATION "${CMAKE_CURRENT_LIST_DIR}/extracted_bundle")
    message(STATUS "Extracted ${BUNDLED_OPENSSL3_BUNDLE} successfully.")
endif()

set(OPENSSL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/extracted_bundle/include)
set(OPENSSL_LIBRARIES ${CMAKE_CURRENT_LIST_DIR}/extracted_bundle/lib/${DEP_ARCH}/libcrypto_static.lib ${CMAKE_CURRENT_LIST_DIR}/extracted_bundle/lib/${DEP_ARCH}/libssl_static.lib)
set(OPENSSL_DEBUG_LIBRARIES ${CMAKE_CURRENT_LIST_DIR}/extracted_bundle/lib/${DEP_ARCH}/libcrypto_static.lib ${CMAKE_CURRENT_LIST_DIR}/extracted_bundle/lib/${DEP_ARCH}/libssl_static.lib)
