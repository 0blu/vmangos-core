#include "Misc.h"

void WorldPackets::Misc::WorldTeleport::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> timeMs;
    recv_data >> location.mapId;
    recv_data >> location.x;
    recv_data >> location.y;
    recv_data >> location.z;
    recv_data >> location.o;
}

void WorldPackets::Misc::AddFriend::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> friendName;
}

void WorldPackets::Misc::DelFriend::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> friendGuid;
}

void WorldPackets::Misc::AddIgnore::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> ignoreName;
}

void WorldPackets::Misc::DelIgnore::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> ignoreGuid;
}
