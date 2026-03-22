#include "TaxiMask.h"

#include <Errors.h>
#include "Util.h"

nonstd::optional<TaxiMask> TaxiMask::FromSerializedString(std::string const& str)
{
    std::vector<std::string> chunks = SplitStringByDelimiter(str, ' ');
    if (chunks.size() != 8)
        return nonstd::nullopt; // Corrupted DB data! It should contain 8 numbers separated by space

    TaxiMask result;
    uint32 index = 0;
    for (std::string const& numberStr : chunks)
    {
        uint32 const number = std::stoi(numberStr);
        for (uint32 innerIdx = 0; innerIdx < 32; innerIdx++)
        {
            bool isSet = number & (1 << innerIdx);
            uint32 entry = index + 1;
            result.SetEntry(entry, isSet);

            index++;
        }
    }

    MANGOS_DEBUG_ASSERT(index == TaxiMask::BIT_COUNT);
    return result;
}

std::string TaxiMask::ToSerializedString() const
{
    uint32 constexpr SERIALIZED_CHUNK_COUNT = TaxiMask::BIT_COUNT / 32; // should be 8

    std::string result;
    for (uint32 chunkIdx = 0; chunkIdx < SERIALIZED_CHUNK_COUNT; chunkIdx++)
    {
        uint32 number = 0;
        for (uint32 innerIdx = 0; innerIdx < 32; innerIdx++)
        {
            uint32 entry = (chunkIdx * 32) + innerIdx + 1;
            if (HasEntry(entry))
            {
                number |= (1 << innerIdx);
            }
        }
        result += std::to_string(number);
        result += " "; // For backwards compatibility we always insert a space at the end (even for the last element)
    }
    return result;
}

std::vector<uint32> TaxiMask::GetEntryList() const
{
    std::vector<uint32> result;
    for (uint32 idx = 0; idx < TaxiMask::BIT_COUNT; idx++)
    {
        uint32 entry = idx + 1;
        if (HasEntry(entry))
        {
            result.push_back(entry);
        }
    }
    return result;
}
