#ifndef MANGOS_PACKETS_GROUP_H
#define MANGOS_PACKETS_GROUP_H

#include "Packet.h"
#include "ObjectGuid.h"

namespace WorldPackets { namespace Group
{
    class GroupInvite final : public ClientPacket
    {
    public:
        std::string memberName;

        explicit GroupInvite() : ClientPacket(CMSG_GROUP_INVITE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class GroupUninvite final : public ClientPacket
    {
    public:
        std::string memberName;

        explicit GroupUninvite() : ClientPacket(CMSG_GROUP_UNINVITE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class GroupUninviteGuid final : public ClientPacket
    {
    public:
        ObjectGuid guid;

        explicit GroupUninviteGuid() : ClientPacket(CMSG_GROUP_UNINVITE_GUID) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class RequestPartyMemberStats final : public ClientPacket
    {
    public:
        ObjectGuid guid;

        explicit RequestPartyMemberStats() : ClientPacket(CMSG_REQUEST_PARTY_MEMBER_STATS) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class LootMethod final : public ClientPacket
    {
    public:
        uint32 lootMethod = 0;
        ObjectGuid lootMaster;
        uint32 lootThreshold = 0;

        explicit LootMethod() : ClientPacket(CMSG_LOOT_METHOD) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Group

#endif // MANGOS_PACKETS_GROUP_H
