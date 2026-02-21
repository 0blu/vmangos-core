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
}} // namespace WorldPackets::Guild

#endif // MANGOS_PACKETS_GUILD_H
