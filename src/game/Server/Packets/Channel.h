#ifndef MANGOS_PACKETS_CHANNEL_H
#define MANGOS_PACKETS_CHANNEL_H

#include "Packet.h"

namespace WorldPackets { namespace Channel
{
    class JoinChannel final : public ClientPacket
    {
    public:
        std::string channelName;
        std::string channelPassword;

        explicit JoinChannel() : ClientPacket(CMSG_JOIN_CHANNEL) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Channel

#endif // MANGOS_PACKETS_CHANNEL_H
