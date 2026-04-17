#ifndef MANGOS_PACKETS_WARDEN_H
#define MANGOS_PACKETS_WARDEN_H

#include "Packet.h"
#include "ByteBuffer.h"

namespace WorldPackets { namespace Warden
{
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_5_1
    class WardenData final : public ServerPacket
    {
    public:
        ByteBuffer encryptedData; // already-encrypted warden payload

        explicit WardenData() : ServerPacket(SMSG_WARDEN_DATA) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };
#endif

}} // namespace WorldPackets::Warden

#endif // MANGOS_PACKETS_WARDEN_H
