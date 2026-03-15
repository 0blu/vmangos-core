#include "ZLib.h"

#include <zlib.h>

nonstd::optional<std::vector<uint8>> Compression::ZLib::Decompress(std::vector<uint8> const& input, uint32 decompressedSize)
{
    // Round up to the next block boundary to avoid Z_BUF_ERROR (-5) when the
    // actual uncompressed size slightly exceeds the reported decompressedSize.
    constexpr uLongf BlockSize = 1024;
    uLongf bufferSize = ((static_cast<uLongf>(decompressedSize) + BlockSize - 1) / BlockSize) * BlockSize;
    std::vector<uint8> output(bufferSize);
    int result = uncompress(output.data(), &bufferSize, input.data(), static_cast<uLong>(input.size()));
    if (result != Z_OK)
        return nonstd::nullopt;
    output.resize(bufferSize);
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
