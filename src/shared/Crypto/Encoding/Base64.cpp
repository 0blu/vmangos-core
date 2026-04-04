#include "Base64.h"

namespace
{
    static constexpr char kEncodeTable[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    // Maps ASCII 0-127 to 6-bit values; -1 means invalid.
    static constexpr int8 kDecodeTable[128] = {
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
        52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
        15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
        -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
        41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,
    };
} // anonymous namespace

namespace Crypto { namespace Encoding { namespace Base64
{

std::string Encode(std::vector<uint8> const& data)
{
    std::string out;
    out.reserve(((data.size() + 2) / 3) * 4);

    size_t i = 0;
    while (i + 2 < data.size())
    {
        out.push_back(kEncodeTable[(data[i] >> 2) & 0x3F]);
        out.push_back(kEncodeTable[((data[i] & 0x03) << 4) | (data[i + 1] >> 4)]);
        out.push_back(kEncodeTable[((data[i + 1] & 0x0F) << 2) | (data[i + 2] >> 6)]);
        out.push_back(kEncodeTable[data[i + 2] & 0x3F]);
        i += 3;
    }

    if (i < data.size())
    {
        out.push_back(kEncodeTable[(data[i] >> 2) & 0x3F]);
        if (i + 1 < data.size())
        {
            out.push_back(kEncodeTable[((data[i] & 0x03) << 4) | (data[i + 1] >> 4)]);
            out.push_back(kEncodeTable[(data[i + 1] & 0x0F) << 2]);
        }
        else
        {
            out.push_back(kEncodeTable[(data[i] & 0x03) << 4]);
            out.push_back('=');
        }
        out.push_back('=');
    }

    return out;
}

nonstd::optional<std::vector<uint8>> Decode(std::string const& input, uint32 max_binary_output_size)
{
    // Upper-bound for decoded length
    if (input.empty())
        return std::vector<uint8>{};

    size_t estimatedSize = (input.size() / 4) * 3 + 3;
    if (estimatedSize > max_binary_output_size)
        return nonstd::nullopt;

    std::vector<uint8> out;
    out.reserve(estimatedSize);

    uint32 buf = 0;
    int bits = 0;

    for (char c : input)
    {
        if (c == '=' || c == '\r' || c == '\n')
            continue;

        uint8 uc = static_cast<uint8>(c);
        if (uc >= 128 || kDecodeTable[uc] < 0)
            return nonstd::nullopt; // invalid character

        buf = (buf << 6) | static_cast<uint32>(kDecodeTable[uc]);
        bits += 6;

        if (bits >= 8)
        {
            bits -= 8;
            out.push_back(static_cast<uint8>((buf >> bits) & 0xFF));

            if (out.size() > max_binary_output_size)
                return nonstd::nullopt;
        }
    }

    return out;
}

}}} // namespace Crypto::Encoding::Base64
