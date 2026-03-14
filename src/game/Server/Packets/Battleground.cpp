#include "Battleground.h"

void WorldPackets::Battleground::BattlefieldListRequest::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> mapId;
}

void WorldPackets::Battleground::AreaSpiritHealerQuery::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Battleground::AreaSpiritHealerQueue::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Battleground::BattlemasterHello::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}
