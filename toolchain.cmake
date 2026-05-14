mark_as_advanced(CMAKE_TOOLCHAIN_FILE)

# Build only release variants of vcpkg dependencies (halves install time/disk).
set(VCPKG_BUILD_TYPE release)

# Allow callers (CI, developer) to override the triplet via -DVCPKG_TARGET_TRIPLET=...
if(NOT VCPKG_TARGET_TRIPLET)
  # Use static version on Windows to prevent dll copying
  if(WIN32)
    if(DEFINED ENV{MSYSTEM} AND "$ENV{MSYSTEM}" STREQUAL "MINGW64")
      set(VCPKG_TARGET_TRIPLET "x64-mingw-static")
    else()
      set(VCPKG_TARGET_TRIPLET "x64-windows-static")
    endif()
  endif()
endif()

message("Loading vcpkg (triplet: ${VCPKG_TARGET_TRIPLET})...")
include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
message("Loading vcpkg... Complete")

set(_VMANGOS_TOOLCHAIN_USED true)
