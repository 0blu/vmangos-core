#ifndef MANGOS_PACKETS_AUCTIONHOUSE_H
#define MANGOS_PACKETS_AUCTIONHOUSE_H

#include "Packet.h"
#include "ObjectGuid.h"

namespace WorldPackets { namespace AuctionHouse
{
    class AuctionHello final : public ClientPacket
    {
    public:
        ObjectGuid auctioneerGuid; // NPC guid

        explicit AuctionHello() : ClientPacket(MSG_AUCTION_HELLO) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::AuctionHouse

#endif // MANGOS_PACKETS_AUCTIONHOUSE_H
