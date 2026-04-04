#include "Combat.h"

void WorldPackets::Combat::AttackSwing::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> targetGuid;
}

void WorldPackets::Combat::SetSheathed::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> sheathed;
}

// --- Server Packets ---

void WorldPackets::Combat::AttackSwingNotInRange::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

void WorldPackets::Combat::AttackSwingNotStanding::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

void WorldPackets::Combat::AttackSwingDeadTarget::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

void WorldPackets::Combat::AttackSwingCantAttack::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

void WorldPackets::Combat::CancelCombat::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

void WorldPackets::Combat::AttackSwingBadFacing::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

void WorldPackets::Combat::AttackStop::AppendBodyTo(ByteBuffer& buffer) const
{
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    buffer << PackedGuid(attackerGuid);
    buffer << PackedGuid(enemyGuid);
#else
    buffer << attackerGuid;
    buffer << enemyGuid;
#endif
    buffer << unk;
}
