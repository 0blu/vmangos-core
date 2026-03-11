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

void WorldPackets::Misc::SetSelection::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Misc::FarSight::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> op;
}

void WorldPackets::Misc::TutorialFlag::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> iFlag;
}

void WorldPackets::Misc::SetActionButton::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> button;
    recv_data >> packetData;
}

void WorldPackets::Misc::InspectHonorStats::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Misc::SetActiveMover::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Misc::SetFactionAtWar::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> repListId;
    recv_data >> flag;
}

void WorldPackets::Misc::SetFactionInactive::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> replistid;
    recv_data >> inactive;
}

void WorldPackets::Misc::ZoneUpdate::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> newZone;
}

void WorldPackets::Misc::ReclaimCorpse::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Misc::ChatIgnored::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}
