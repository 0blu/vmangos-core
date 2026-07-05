# Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
# Copyright (C) 2009-2011 MaNGOSZero <https://github.com/mangos/zero>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# For the `classic` branch we target Windows 7 SP1 x64 with VisualStudio 2019 >=16.11
# - max C++20 support
# - bundled with cmake 3.19 (technically Windows 7 supports even newer versions)
cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(vMaNGOS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Disable non-standard C++ features

# Ensure 64bit version
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(FATAL_ERROR "Please switch to x64 build. Current pointer size is '${CMAKE_SIZEOF_VOID_P}' but should be '8'")
  return()
endif()

# Force static linking with vcpkg
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

# Allow `find_package()` to use `<PackageName>_ROOT` variable
if(${CMAKE_VERSION} VERSION_GREATER "3.11")
  cmake_policy(SET CMP0074 NEW)
endif()

include(cmake/common.cmake)
include(cmake/options.cmake)
include(cmake/platform/detect_platform.cmake)

# Force out-of-source build
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" BUILDING_IN_SOURCE)
if(BUILDING_IN_SOURCE)
  message(FATAL_ERROR
    "This project requires an out of source build. Remove the file 'CMakeCache.txt' found in this directory before continuing, create a separate build directory and run 'cmake <srcs> [options]' from there."
  )
endif()

find_package(Git)

# Set up the install-prefix
if(CMAKE_INSTALL_PREFIX STREQUAL "/usr/local")
  get_filename_component(PREFIX_ABSOLUTE "../server" ABSOLUTE)
  set(CMAKE_INSTALL_PREFIX ${PREFIX_ABSOLUTE} CACHE PATH "Install path prefix." FORCE)
endif()

if(PREFIX)
  if(!WIN32)
    string(REGEX REPLACE "^~" "$ENV{HOME}" PREFIX ${PREFIX})
  endif()
  get_filename_component(PREFIX_ABSOLUTE ${PREFIX} ABSOLUTE)
  set(CMAKE_INSTALL_PREFIX ${PREFIX} CACHE PATH "Install path prefix." FORCE)
else()
  set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} CACHE PATH "Install path prefix.")
endif()

# If win32 put it in the bin dir not lib
if(WIN32)
  set(BIN_DIR ${CMAKE_INSTALL_PREFIX})
  set(CONF_DIR ${CMAKE_INSTALL_PREFIX})
  set(LIBS_DIR ${CMAKE_INSTALL_PREFIX})
else()
  set(BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
  set(CONF_DIR ${CMAKE_INSTALL_PREFIX}/etc CACHE PATH "Set custom configuration directory")
  set(LIBS_DIR ${CMAKE_INSTALL_PREFIX}/lib)
endif()

# Set RPATH-handing (CMake parameters)
set(CMAKE_SKIP_BUILD_RPATH OFF)
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
set(CMAKE_INSTALL_RPATH ${LIBS_DIR})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

# Find needed packages and if necessary abort if something important is missing

if (ENABLE_MAILSENDER)
  set(REQUIRES_LIBCURL ON)
else()
  set(REQUIRES_LIBCURL OFF)
endif()

# Find packages via vcpkg
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

# MariaDB client library via vcpkg (lightweight, no Boost dependency).
# Provides mysql.h / errmsg.h / mysqld_error.h as a drop-in replacement
# for the MySQL C client, so the rest of the build continues to use the
# MYSQL_* variable names below.
find_package(unofficial-libmariadb CONFIG REQUIRED)
get_target_property(MYSQL_INCLUDE_DIR unofficial::libmariadb INTERFACE_INCLUDE_DIRECTORIES)
set(MYSQL_LIBRARY unofficial::libmariadb)

if(REQUIRES_LIBCURL)
  find_package(CURL REQUIRED)
endif()

# Add uninstall script and target
#configure_file(
#  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
#  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#  IMMEDIATE @ONLY
#)

#add_custom_target(uninstall
#  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#)

# Find core revision
if(NOT GIT_EXECUTABLE)
  set(rev_date "1970-01-01 00:00:00 +0000")
  set(rev_hash "unknown")
else()
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short=20 HEAD
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE rev_hash
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )

  execute_process(
    COMMAND ${GIT_EXECUTABLE} show -s --format=%ci
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE rev_date
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )

  if(NOT rev_hash)
    # No valid ways available to find/set the revision/hash, so let's force some defaults
    message(STATUS "
    Could not find a proper repository signature (hash) - you may need to pull tags with git fetch -t
    Continuing anyway - note that the versionstring will be set to \"unknown 1970-01-01 00:00:00 (Archived)\"")
    set(rev_date "1970-01-01 00:00:00 +0000")
    set(rev_hash "unknown")
  endif()

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/generators/revision.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/shared/revision.h)
endif()

set(supported_build ${SUPPORTED_CLIENT_BUILD})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/generators/Progression.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/shared/Progression.h)

# Generate migrations list
include(cmake/generators/migrations_list.h.cmake)

if(${CMAKE_VERSION} VERSION_LESS "3.16")
  if(USE_PCH AND NOT PCHSupport_FOUND)
    set(USE_PCH OFF CACHE BOOL
        "Use precompiled headers"
        FORCE)
    message(WARNING
      "No PCH for your system possible but USE_PCH was set to ON. Resetting it.\nUpdate to cmake 3.16 or newer to use PCH."
      )
  endif()
endif()

include(cmake/showoptions.cmake)

set(DEFINITIONS_RELEASE ${DEFINITIONS_RELEASE} NDEBUG)
set(DEFINITIONS_DEBUG ${DEFINITIONS_DEBUG} _DEBUG MANGOS_DEBUG)

# Some small tweaks for Visual Studio 7 and above.
if(MSVC)
  # Mark 32 bit executables large address aware so they can use > 2GB address space
  #if(PLATFORM MATCHES X86)
  #  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
  #endif()

  set(MANGOS_CXX_FLAGS "${MANGOS_CXX_FLAGS} /bigobj")
endif()

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH ${LIBS_DIR})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(XCODE)
  if(PLATFORM MATCHES X86)
    set(CMAKE_OSX_ARCHITECTURES i386)
  else()
    set(CMAKE_OSX_ARCHITECTURES x86_64)
  endif()
endif()


# Add definitions for all build types
# Don't place this above 'dep' subdirectory! Because of defines build will crash.
set(DEFINITIONS
  ${DEFINITIONS}
  DO_MYSQL
)

if(WIN32)
  set(DEFINITIONS ${DEFINITIONS} WIN32 _WIN32 _CRT_SECURE_NO_WARNINGS)
else()
  set(DEFINITIONS ${DEFINITIONS} SYSCONFDIR="${CONF_DIR}/")
endif()

if (ENABLE_MAILSENDER)
  set(DEFINITIONS ${DEFINITIONS} ENABLE_MAILSENDER)
endif()

set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS ${DEFINITIONS})
set_property(DIRECTORY APPEND PROPERTY
  COMPILE_DEFINITIONS $<$<CONFIG:Debug>:${DEFINITIONS_DEBUG}>
)
set_property(DIRECTORY APPEND PROPERTY
  COMPILE_DEFINITIONS $<$<CONFIG:Release>:${DEFINITIONS_RELEASE}>
)

message(STATUS "MaNGOS CXX flags      : ${MANGOS_CXX_FLAGS}")

add_subdirectory(src)
add_subdirectory(dep)

if (BUILD_EXTRACTORS OR BUILD_REALMMERGE)
    add_subdirectory(contrib)
endif()
