#include "Group.h"

void WorldPackets::Group::GroupInvite::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> memberName;
}
