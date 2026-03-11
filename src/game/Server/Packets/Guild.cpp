#include "Guild.h"

void WorldPackets::Guild::GuildCreate::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> desiredGuildName;
}

void WorldPackets::Guild::GuildQuery::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guildId;
}

void WorldPackets::Guild::GuildInvite::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> invitedName;
}

void WorldPackets::Guild::GuildRemove::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> playerName;
}

void WorldPackets::Guild::GuildPromote::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> playerName;
}

void WorldPackets::Guild::GuildDemote::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> playerName;
}

void WorldPackets::Guild::GuildLeader::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> playerName;
}

void WorldPackets::Guild::GuildMOTD::ReadFromWorldPacket(WorldPacket& recv_data)
{
    if (!recv_data.empty())
        recv_data >> motd;
}

void WorldPackets::Guild::GuildChangeInfoText::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> infoText;
}
