#include "AuctionHouse.h"

void WorldPackets::AuctionHouse::AuctionHello::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> auctioneerGuid;
}

void WorldPackets::AuctionHouse::AuctionListBidderItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> auctioneerGuid;
    recv_data >> pagingElementStartIndex;

    uint32 idsToRefresh;
    recv_data >> idsToRefresh;
    for (int i = 0; i < idsToRefresh; i++)
    {
        uint32 auctionId;
        recv_data >> auctionId;
        bidAuctionIdsToRefresh.push_back(auctionId);
    }
}

void WorldPackets::AuctionHouse::AuctionListOwnerItems::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> auctioneerGuid;
    recv_data >> listfrom;
}

void WorldPackets::AuctionHouse::AuctionPlaceBid::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> auctioneerGuid;
    recv_data >> auctionId >> price;
}

void WorldPackets::AuctionHouse::AuctionRemoveItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> auctioneerGuid;
    recv_data >> auctionId;
}

