#ifndef _MANGOS_CRYPTO_ENCODING_BASE64_H
#define _MANGOS_CRYPTO_ENCODING_BASE64_H

#include "Platform/Define.h"
#include "nonstd/optional.hpp"

#include <string>
#include <vector>

namespace Crypto { namespace Encoding { namespace Base64
{
    std::string Encode(std::vector<uint8> const& binary_source_data);
    nonstd::optional<std::vector<uint8>> Decode(std::string const& base64_source_string, uint32 max_binary_output_size);
}}} // namespace Crypto::Encoding::Base64

#endif // _MANGOS_CRYPTO_ENCODING_BASE64_H
