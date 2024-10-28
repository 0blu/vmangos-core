# mySQL
include(${CMAKE_CURRENT_LIST_DIR}/openssl3/setup_mysql.cmake)

# zlib
include(${CMAKE_CURRENT_LIST_DIR}/openssl3/setup_zlib.cmake)

# openssl3
include(${CMAKE_CURRENT_LIST_DIR}/openssl3/setup_openssl3.cmake)

# curl
if (REQUIRES_LIBCURL)
  set(CURL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/optional_dependencies/curl/include)
  set(CURL_LIBRARY     ${CMAKE_CURRENT_LIST_DIR}/optional_dependencies/curl/lib/libcurl.lib)
endif()
