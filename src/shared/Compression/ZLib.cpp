#include "ZLib.h"

#include <zlib.h>

nonstd::optional<std::vector<uint8>> Compression::ZLib::Decompress(std::vector<uint8> const& input, uint32 decompressedSize)
{
    std::vector<uint8> output(decompressedSize);
    uLongf realSize = decompressedSize;
    int result = uncompress(output.data(), &realSize, input.data(), static_cast<uLong>(input.size()));
    if (result != Z_OK)
        return nonstd::nullopt;
    output.resize(realSize);
    return output;
}

nonstd::optional<std::vector<uint8>> Compression::ZLib::Compress(uint8 const* data, size_t length)
{
    uLongf destSize = compressBound(static_cast<uLong>(length));
    std::vector<uint8> output(destSize);
    int result = compress(output.data(), &destSize, data, static_cast<uLong>(length));
    if (result != Z_OK)
        return nonstd::nullopt;
    output.resize(destSize);
    return output;
}

nonstd::optional<std::vector<uint8>> Compression::ZLib::Compress(std::vector<uint8> const& data)
{
    return Compress(data.data(), data.size());
}
