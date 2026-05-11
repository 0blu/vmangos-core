#include "Duel.h"

void WorldPackets::Duel::DuelAccepted::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> playerGuid;
}

void WorldPackets::Duel::DuelCancelled::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> playerGuid;
}

// --- Server Packets ---

void WorldPackets::Duel::DuelOutOfBounds::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

void WorldPackets::Duel::DuelInBounds::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

void WorldPackets::Duel::DuelComplete::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint8(started ? 1 : 0);
}

void WorldPackets::Duel::DuelWinner::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint8(fled ? 1 : 0); // 0 = just won; 1 = fled
    buffer << winnerName;
    buffer << loserName;
}

void WorldPackets::Duel::DuelCountdown::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << countdown; // seconds
}
