#include "Warden.h"

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_5_1
void WorldPackets::Warden::WardenData::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer.append(encryptedData);
}
#endif
