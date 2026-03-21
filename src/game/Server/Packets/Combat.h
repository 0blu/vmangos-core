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

    class SetSheathed final : public ClientPacket
    {
    public:
        uint32 sheathed = 0;

        explicit SetSheathed() : ClientPacket(CMSG_SETSHEATHED) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
    class AttackStop final : public ServerPacket
    {
    public:
        ObjectGuid attackerGuid;
        ObjectGuid victimGuid;
        uint32 nowDead = 0;

        explicit AttackStop() : ServerPacket(SMSG_ATTACKSTOP) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };
}} // namespace WorldPackets::Combat

#endif // MANGOS_PACKETS_COMBAT_H
