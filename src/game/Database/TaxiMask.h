#ifndef MANGOS_TAXIMASK_H
#define MANGOS_TAXIMASK_H

#include "Platform/Define.h"
#include "nonstd/optional.hpp"
#include <Errors.h>

#include <bitset>
#include <vector>
#include <string>

class TaxiMask
{
    static size_t constexpr BIT_COUNT = 256;
private:
    std::bitset<BIT_COUNT> bits;

    explicit TaxiMask(std::bitset<BIT_COUNT> const& bits) : bits(bits) {}
public:
    explicit TaxiMask() {};
    static nonstd::optional<TaxiMask> FromSerializedString(std::string const& str);
    std::string ToSerializedString() const;
    std::vector<uint32> GetEntryList() const;

    uint32 MaxValidIndex = BIT_COUNT - 1;

    bool HasEntry(uint32 entry) const
    {
        MANGOS_DEBUG_ASSERT(entry > 0);
        return bits.test(entry - 1);
    }

    void SetEntry(uint32 entry, bool value)
    {
        MANGOS_DEBUG_ASSERT(entry > 0);
        bits.set(entry - 1, value);
    }

    void Clear()
    {
        bits.reset();
    }
};


#endif // MANGOS_TAXIMASK_H
