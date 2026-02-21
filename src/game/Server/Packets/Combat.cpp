#include "Combat.h"

void WorldPackets::Combat::AttackSwing::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> targetGuid;
}
