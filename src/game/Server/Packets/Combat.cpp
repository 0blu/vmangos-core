#include "Combat.h"

void WorldPackets::Combat::AttackSwing::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> targetGuid;
}

void WorldPackets::Combat::SetSheathed::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> sheathed;
}

void WorldPackets::Combat::AttackStop::AppendBodyTo(ByteBuffer& buffer) const
{
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    buffer << attackerGuid.WriteAsPacked();
    buffer << victimGuid.WriteAsPacked();
#else
    buffer << attackerGuid.GetRawValue();
    buffer << victimGuid.GetRawValue();
#endif
    buffer << uint32(nowDead);
}
