#ifndef MANGOS_PACKETS_GUILD_H
#define MANGOS_PACKETS_GUILD_H

#include "Packet.h"

namespace WorldPackets { namespace Guild
{
    class GuildCreate final : public ClientPacket
    {
    public:
        std::string desiredGuildName;

        explicit GuildCreate() : ClientPacket(CMSG_GUILD_CREATE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class GuildQuery final : public ClientPacket
    {
    public:
        uint32 guildId = 0;

        explicit GuildQuery() : ClientPacket(CMSG_GUILD_QUERY) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class GuildInvite final : public ClientPacket
    {
    public:
        std::string invitedName;

        explicit GuildInvite() : ClientPacket(CMSG_GUILD_INVITE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class GuildRemove final : public ClientPacket
    {
    public:
        std::string playerName;

        explicit GuildRemove() : ClientPacket(CMSG_GUILD_REMOVE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class GuildPromote final : public ClientPacket
    {
    public:
        std::string playerName;

        explicit GuildPromote() : ClientPacket(CMSG_GUILD_PROMOTE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class GuildDemote final : public ClientPacket
    {
    public:
        std::string playerName;

        explicit GuildDemote() : ClientPacket(CMSG_GUILD_DEMOTE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class GuildLeader final : public ClientPacket
    {
    public:
        std::string playerName;

        explicit GuildLeader() : ClientPacket(CMSG_GUILD_LEADER) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class GuildMOTD final : public ClientPacket
    {
    public:
        std::string motd;

        explicit GuildMOTD() : ClientPacket(CMSG_GUILD_MOTD) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Guild

#endif // MANGOS_PACKETS_GUILD_H
