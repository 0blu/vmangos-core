#include "Trade.h"

void WorldPackets::Trade::InitiateTrade::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> tradeTargetGuid;
}

void WorldPackets::Trade::SetTradeGold::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> gold;
}

void WorldPackets::Trade::SetTradeItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> tradeSlot;
    recv_data >> bag;
    recv_data >> slot;
}

void WorldPackets::Trade::ClearTradeItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> tradeSlot;
}

void WorldPackets::Trade::AcceptTrade::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> unknown;
}
