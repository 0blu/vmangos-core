#ifndef MANGOS_PACKETS_GROUP_H
#define MANGOS_PACKETS_GROUP_H

#include "Packet.h"
#include "ObjectGuid.h"
#include "nonstd/optional.hpp"

#include <string>
#include <vector>

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

    class MinimapPing final : public ClientPacket
    {
    public:
        float x = 0.0f;
        float y = 0.0f;

        explicit MinimapPing() : ClientPacket(MSG_MINIMAP_PING) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class RandomRoll final : public ClientPacket
    {
    public:
        uint32 minimum = 0;
        uint32 maximum = 0;

        explicit RandomRoll() : ClientPacket(MSG_RANDOM_ROLL) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class GroupChangeSubGroup final : public ClientPacket
    {
    public:
        std::string name;
        uint8 groupNr = 0;

        explicit GroupChangeSubGroup() : ClientPacket(CMSG_GROUP_CHANGE_SUB_GROUP) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class GroupSwapSubGroup final : public ClientPacket
    {
    public:
        std::string name;
        std::string nameSwapWith;

        explicit GroupSwapSubGroup() : ClientPacket(CMSG_GROUP_SWAP_SUB_GROUP) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class GroupSetLeader final : public ClientPacket
    {
    public:
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_11_2
        ObjectGuid guid;
#else
        std::string name;
#endif

        explicit GroupSetLeader() : ClientPacket(CMSG_GROUP_SET_LEADER) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class GroupAssistantLeader final : public ClientPacket
    {
    public:
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_11_2
        ObjectGuid guid;
#else
        std::string name;
#endif
        uint8 flag = 0;

        explicit GroupAssistantLeader() : ClientPacket(CMSG_GROUP_ASSISTANT_LEADER) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
    class RaidTargetUpdate final : public ClientPacket
    {
    public:
        uint8      iconId = 0;
        ObjectGuid guid; // only valid when iconId != 0xFF (icon update, not request)

        explicit RaidTargetUpdate() : ClientPacket(MSG_RAID_TARGET_UPDATE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class RaidReadyCheck final : public ClientPacket
    {
    public:
        // packet can be used in two ways: request: `hasValue() = false`, response: `hasValue() = true`
        nonstd::optional<uint8> state;

        explicit RaidReadyCheck() : ClientPacket(MSG_RAID_READY_CHECK) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
#endif

    // --- Server Packets ---

    static constexpr uint8 PARTY_MAX_POSITIVE_AURAS = 32;  // MAX_POSITIVE_AURAS
    static constexpr uint8 PARTY_MAX_NEGATIVE_AURAS = 16;  // MAX_AURAS - MAX_POSITIVE_AURAS

    struct PartyMemberAuraData
    {
        uint32 positiveAuraMask = 0;
        uint16 positiveAuras[PARTY_MAX_POSITIVE_AURAS] = {};
        uint16 negativeAuraMask = 0;
        uint16 negativeAuras[PARTY_MAX_NEGATIVE_AURAS] = {};
    };

    struct PartyMemberStatsData
    {
        ObjectGuid guid;
        uint32 mask = 0;

        // Player fields
        uint8  status = 0;
        uint16 curHP = 0;
        uint16 maxHP = 0;
        uint8  powerType = 0;
        uint16 curPower = 0;
        uint16 maxPower = 0;
        uint16 level = 0;
        uint16 zone = 0;
        int16  posX = 0;
        int16  posY = 0;
        PartyMemberAuraData auras;

        // Pet fields
        ObjectGuid petGuid;
        std::string petName;
        uint16 petModelId = 0;
        uint16 petCurHP = 0;
        uint16 petMaxHP = 0;
        uint8  petPowerType = 0;
        uint16 petCurPower = 0;
        uint16 petMaxPower = 0;
        PartyMemberAuraData petAuras;
    };

    void WritePartyMemberStats(ByteBuffer& buffer, PartyMemberStatsData const& data);

    class PartyCommandResult final : public ServerPacket
    {
    public:
        uint32 operation = 0;
        std::string member; // max len 48
        uint32 result = 0;

        explicit PartyCommandResult() : ServerPacket(SMSG_PARTY_COMMAND_RESULT) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class GroupInviteNotification final : public ServerPacket
    {
    public:
        std::string inviterName;

        explicit GroupInviteNotification() : ServerPacket(SMSG_GROUP_INVITE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class GroupDeclineNotification final : public ServerPacket
    {
    public:
        std::string playerName;

        explicit GroupDeclineNotification() : ServerPacket(SMSG_GROUP_DECLINE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class GroupUninviteNotification final : public ServerPacket
    {
    public:
        explicit GroupUninviteNotification() : ServerPacket(SMSG_GROUP_UNINVITE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class GroupDestroyed final : public ServerPacket
    {
    public:
        explicit GroupDestroyed() : ServerPacket(SMSG_GROUP_DESTROYED) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class GroupListEmpty final : public ServerPacket
    {
    public:
        explicit GroupListEmpty() : ServerPacket(SMSG_GROUP_LIST) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    struct GroupMemberEntry
    {
        std::string name;
        ObjectGuid guid;
        uint8 onlineStatus = 0;
        uint8 groupAndFlags = 0; // (groupId | (assistant ? 0x80 : 0))
    };

    class GroupListFull final : public ServerPacket
    {
    public:
        uint8 groupType = 0;
        uint8 ownGroupAndFlags = 0; // own (groupId | (assistant ? 0x80 : 0))
        std::vector<GroupMemberEntry> members;
        ObjectGuid leaderGuid;
        // Loot settings - only present when members is not empty
        uint8 lootMethod = 0;
        ObjectGuid masterLooterGuid;
        uint8 lootThreshold = 0;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
        uint8 dungeonDifficulty = 0;
#endif

        explicit GroupListFull() : ServerPacket(SMSG_GROUP_LIST) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
    class RaidReadyCheckResponse final : public ServerPacket
    {
    public:
        ObjectGuid senderGuid;  // guid of the player who answered
        uint8 state = 0;        // ready state

        explicit RaidReadyCheckResponse() : ServerPacket(MSG_RAID_READY_CHECK) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    struct RaidTargetIconEntry
    {
        uint8 iconId = 0;
        ObjectGuid guid;
    };

    // Delta update: a single icon was changed (mode byte = 0)
    class RaidTargetUpdateDelta final : public ServerPacket
    {
    public:
        uint8 iconId = 0;
        ObjectGuid targetGuid;

        explicit RaidTargetUpdateDelta() : ServerPacket(MSG_RAID_TARGET_UPDATE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    // Full icon list sent to one session (mode byte = 1)
    class RaidTargetUpdateFull final : public ServerPacket
    {
    public:
        std::vector<RaidTargetIconEntry> icons;

        explicit RaidTargetUpdateFull() : ServerPacket(MSG_RAID_TARGET_UPDATE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };
#endif

    class PartyMemberStatsFull final : public ServerPacket
    {
    public:
        PartyMemberStatsData data;

        explicit PartyMemberStatsFull() : ServerPacket(
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_5_1
            SMSG_PARTY_MEMBER_STATS_FULL
#else
            SMSG_PARTY_MEMBER_STATS
#endif
        ) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class PartyMemberStats final : public ServerPacket
    {
    public:
        PartyMemberStatsData data;

        explicit PartyMemberStats() : ServerPacket(SMSG_PARTY_MEMBER_STATS) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

}} // namespace WorldPackets::Group

#endif // MANGOS_PACKETS_GROUP_H
