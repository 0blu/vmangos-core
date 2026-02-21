#include "AuctionHouse.h"

void WorldPackets::AuctionHouse::AuctionHello::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> auctioneerGuid;
}
