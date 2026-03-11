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

void WorldPackets::Misc::StandStateChange::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> animState;
}

void WorldPackets::Misc::AreaTrigger::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> triggerId;
}

void WorldPackets::Misc::Inspect::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Misc::GameObjectUse::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Misc::Emote::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> emote;
}

void WorldPackets::Misc::TextEmote::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> textEmote;
    recv_data >> emoteNum;
    recv_data >> guid;
}
