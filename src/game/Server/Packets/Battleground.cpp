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

void WorldPackets::Battleground::BattleFieldPort::ReadFromWorldPacket(WorldPacket& recv_data)
{
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    recv_data >> mapId >> action;
#else
    recv_data >> action;
#endif
}

void WorldPackets::Battleground::LeaveBattlefield::ReadFromWorldPacket(WorldPacket& recv_data)
{
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    recv_data >> mapId;
#endif
}
