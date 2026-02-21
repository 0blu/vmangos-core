#ifndef MANGOS_PACKETS_MISC_H
#define MANGOS_PACKETS_MISC_H

#include "Packet.h"
#include "SharedDefines.h"

namespace WorldPackets { namespace Misc
{
    class WorldTeleport final : public ClientPacket
    {
    public:
        uint32 timeMs;
        WorldLocation location;

        explicit WorldTeleport() : ClientPacket(CMSG_WORLD_TELEPORT), timeMs(0) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Misc

#endif // MANGOS_PACKETS_MISC_H
