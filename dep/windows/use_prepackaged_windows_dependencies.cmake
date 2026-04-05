# mySQL
include(${CMAKE_CURRENT_LIST_DIR}/mysql/setup_mysql.cmake)

# zlib
include(${CMAKE_CURRENT_LIST_DIR}/zlib/setup_zlib.cmake)

# openssl3
include(${CMAKE_CURRENT_LIST_DIR}/openssl3/setup_openssl3.cmake)

# curl
if (REQUIRES_LIBCURL)
  include(${CMAKE_CURRENT_LIST_DIR}/curl/setup_curl.cmake)
endif()
