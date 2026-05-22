// Shim: for legacy code that still uses nonstd::optional, but we want to use std::optional in modern C++
#pragma once
#include <optional>

namespace nonstd {
    using std::optional;
    using std::nullopt_t;
    using std::nullopt;
    using std::make_optional;
    using std::bad_optional_access;
} // namespace nonstd
