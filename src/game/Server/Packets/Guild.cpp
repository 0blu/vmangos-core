#include "Guild.h"

void WorldPackets::Guild::GuildCreate::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> desiredGuildName;
}
