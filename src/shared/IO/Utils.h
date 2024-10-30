#ifndef MANGOS_IO_UTILS_H
#define MANGOS_IO_UTILS_H

#include <cstdint>

namespace IO { namespace Utils {
    uint64_t GetCurrentProcessId();
    uint64_t GetCurrentProcessAmountOfAllocatedBytes();
}} // namespace IO::Utils

#endif // MANGOS_IO_UTILS_H
