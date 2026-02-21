#ifndef MANGOS_PACKETS_COMBAT_H
#define MANGOS_PACKETS_COMBAT_H

#include "Packet.h"
#include "ObjectGuid.h"

namespace WorldPackets { namespace Combat
{
    class AttackSwing final : public ClientPacket
    {
    public:
        ObjectGuid targetGuid;

        explicit AttackSwing() : ClientPacket(CMSG_ATTACKSWING) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Combat

#endif // MANGOS_PACKETS_COMBAT_H
