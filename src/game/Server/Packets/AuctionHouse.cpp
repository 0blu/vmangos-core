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

