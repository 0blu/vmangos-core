#ifndef MANGOS_PACKETS_ITEM_H
#define MANGOS_PACKETS_ITEM_H

#include "Packet.h"
#include "ObjectGuid.h"

namespace WorldPackets { namespace Item
{
    class QueryItem final : public ClientPacket
    {
    public:
        uint32 itemEntry; // db entry id
        ObjectGuid itemGuid;

        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Item

#endif // MANGOS_PACKETS_ITEM_H
