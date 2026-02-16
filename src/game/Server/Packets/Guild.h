#ifndef MANGOS_PACKETS_GUILD_H
#define MANGOS_PACKETS_GUILD_H

#include "Packet.h"

namespace WorldPackets { namespace Guild
{
    class GuildCreate final : public ClientPacket
    {
    public:
        std::string desiredGuildName;

        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Guild

#endif // MANGOS_PACKETS_GUILD_H
