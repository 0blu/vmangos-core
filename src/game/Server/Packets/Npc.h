#ifndef MANGOS_PACKETS_NPC_H
#define MANGOS_PACKETS_NPC_H

#include "Packet.h"
#include "ObjectGuid.h"

namespace WorldPackets { namespace Npc
{
    class GossipHello final : public ClientPacket
    {
    public:
        ObjectGuid npcGuid;

        explicit GossipHello() : ClientPacket(CMSG_GOSSIP_HELLO) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Npc

#endif // MANGOS_PACKETS_NPC_H
