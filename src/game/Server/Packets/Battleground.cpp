#include "Battleground.h"

void WorldPackets::Battleground::BattlefieldListRequest::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> mapId;
}
