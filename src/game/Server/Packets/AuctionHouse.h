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

    class AuctionListBidderItem final : public ClientPacket
    {
    public:
        ObjectGuid          auctioneerGuid;          // NPC guid
        uint32              pagingElementStartIndex; // Page of auctions. (Should be a multiple of 50)
        std::vector<uint32> bidAuctionIdsToRefresh;  // Sent when the client needs specific auction id information. (Should be an auction where player previously bid on)

        explicit AuctionListBidderItem() : ClientPacket(CMSG_AUCTION_LIST_BIDDER_ITEMS), pagingElementStartIndex(0) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::AuctionHouse

#endif // MANGOS_PACKETS_AUCTIONHOUSE_H
