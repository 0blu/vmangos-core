#ifndef MANGOS_PACKETS_LOOT_H
#define MANGOS_PACKETS_LOOT_H

#include "Packet.h"
#include "SharedDefines.h"

namespace WorldPackets { namespace Loot
{
    class AutoStoreLootItem final : public ClientPacket
    {
    public:
        uint8 lootSlot;

        explicit AutoStoreLootItem() : ClientPacket(CMSG_AUTOSTORE_LOOT_ITEM), lootSlot(0) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Loot

#endif // MANGOS_PACKETS_LOOT_H
