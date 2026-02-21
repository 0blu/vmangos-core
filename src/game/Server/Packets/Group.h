#ifndef MANGOS_PACKETS_GROUP_H
#define MANGOS_PACKETS_GROUP_H

#include "Packet.h"

namespace WorldPackets { namespace Group
{
    class GroupInvite final : public ClientPacket
    {
    public:
        std::string memberName;

        explicit GroupInvite() : ClientPacket(CMSG_GROUP_INVITE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Group

#endif // MANGOS_PACKETS_GROUP_H
