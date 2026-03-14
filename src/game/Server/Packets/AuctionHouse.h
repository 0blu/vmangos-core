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

    class AuctionListOwnerItems final : public ClientPacket
    {
    public:
        ObjectGuid auctioneerGuid;
        uint32     listfrom = 0;

        explicit AuctionListOwnerItems() : ClientPacket(CMSG_AUCTION_LIST_OWNER_ITEMS) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class AuctionPlaceBid final : public ClientPacket
    {
    public:
        ObjectGuid auctioneerGuid;
        uint32     auctionId = 0;
        uint32     price = 0;

        explicit AuctionPlaceBid() : ClientPacket(CMSG_AUCTION_PLACE_BID) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class AuctionRemoveItem final : public ClientPacket
    {
    public:
        ObjectGuid auctioneerGuid;
        uint32     auctionId = 0;

        explicit AuctionRemoveItem() : ClientPacket(CMSG_AUCTION_REMOVE_ITEM) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::AuctionHouse

#endif // MANGOS_PACKETS_AUCTIONHOUSE_H
