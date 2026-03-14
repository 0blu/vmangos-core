#include "Group.h"

void WorldPackets::Group::GroupInvite::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> memberName;
}

void WorldPackets::Group::GroupUninvite::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> memberName;
}

void WorldPackets::Group::GroupUninviteGuid::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Group::RequestPartyMemberStats::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Group::LootMethod::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> lootMethod;
    recv_data >> lootMaster;
    recv_data >> lootThreshold;
}

void WorldPackets::Group::MinimapPing::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> x;
    recv_data >> y;
}

void WorldPackets::Group::RandomRoll::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> minimum;
    recv_data >> maximum;
}

void WorldPackets::Group::GroupChangeSubGroup::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> name;
    recv_data >> groupNr;
}

void WorldPackets::Group::GroupSwapSubGroup::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> name;
    recv_data >> nameSwapWith;
}
