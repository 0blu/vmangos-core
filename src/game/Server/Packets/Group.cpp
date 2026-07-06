#include "Group.h"
#include "Group/Group.h"

void WorldPackets::Group::GroupInvite::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> memberName;
}

void WorldPackets::Group::GroupUninvite::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> memberName;
}

void WorldPackets::Group::GroupUninviteGuid::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_4_2
void WorldPackets::Group::RequestPartyMemberStats::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}
#endif

void WorldPackets::Group::LootMethod::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> lootMethod;
    recv_data >> lootMaster;
    recv_data >> lootThreshold;
}

void WorldPackets::Group::MinimapPing::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> x;
    recv_data >> y;
}

void WorldPackets::Group::RandomRoll::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> minimum;
    recv_data >> maximum;
}

void WorldPackets::Group::GroupChangeSubGroup::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> name;
    recv_data >> groupNr;
}

void WorldPackets::Group::GroupSwapSubGroup::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> name;
    recv_data >> nameSwapWith;
}

void WorldPackets::Group::GroupSetLeader::ReadFromWorldPacket(WorldPacket& recv_data)
{
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_11_2
    recv_data >> guid;
#else
    recv_data >> name;
#endif
}

void WorldPackets::Group::GroupAssistantLeader::ReadFromWorldPacket(WorldPacket& recv_data)
{
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_11_2
    recv_data >> guid;
#else
    recv_data >> name;
#endif
    recv_data >> flag;
}

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
void WorldPackets::Group::RaidTargetUpdate::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> iconId;
    if (iconId != 0xFF) // target icon update (not a request)
        recv_data >> guid;
}

void WorldPackets::Group::RaidReadyCheckFromClient::ReadFromWorldPacket(WorldPacket& recv_data)
{
    if (!recv_data.empty())
    {
        uint8 s;
        recv_data >> s;
        state = s;
    }
}

void WorldPackets::Group::RaidReadyCheckFromServer_Request::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

#endif

void WorldPackets::Group::PartyCommandResult::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << operation;
    buffer << memberName;
    buffer << result;
}

void WorldPackets::Group::GroupInviteNotification::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << inviterName;
}

void WorldPackets::Group::GroupDeclineNotification::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << playerName;
}

void WorldPackets::Group::GroupUninviteNotification::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

void WorldPackets::Group::GroupDestroyed::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
void WorldPackets::Group::RaidReadyCheckFromServer_Response::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << senderGuid;
    buffer << state;
}

void WorldPackets::Group::RaidTargetUpdateDelta::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint8(0); // 0 = delta update
    buffer << iconId;
    buffer << targetGuid;
}

void WorldPackets::Group::RaidTargetUpdateAll::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint8(1); // 1 - full icon list, 0 - delta update
    for (auto const& icon : icons)
    {
        buffer << icon.iconId;
        buffer << icon.targetGuid;
    }
}
#endif

void WorldPackets::Group::GroupSetLeaderNotification::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << leaderName;
}

void WorldPackets::Group::GroupList::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << groupType;
    buffer << ownGroupAndAssistantFlag;

    buffer << uint32(members.size());
    for (auto const& member : members)
    {
        buffer << member.name;
        buffer << member.guid;
        buffer << member.onlineStatus;
        buffer << member.groupAndAssistantFlag;
    }

    buffer << leaderGuid;
    if (!members.empty())
    {
        buffer << lootMethod;
        buffer << looterGuid;              // master looter guid (MASTER_LOOT) or 0
        buffer << lootThreshold;

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
        buffer << dungeonDifficulty;
#endif
    }
}

void WorldPackets::Group::LootMasterList::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint8(eligibleLooters.size());
    for (auto const& guid : eligibleLooters)
        buffer << guid;
}

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_4_2
namespace
{
    template <typename Packet>
    void AppendPartyMemberStatsBody(ByteBuffer& buffer, Packet const& p)
    {
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
        buffer << p.playerGuid.WriteAsPacked();
#else
        buffer << p.playerGuid;
#endif
        buffer << p.updateMask;

        if (p.updateMask & GROUP_UPDATE_FLAG_STATUS)
            buffer << p.memberStatus;

        if (p.updateMask & GROUP_UPDATE_FLAG_CUR_HP)
            buffer << p.currentHp;

        if (p.updateMask & GROUP_UPDATE_FLAG_MAX_HP)
            buffer << p.maxHp;

        if (p.updateMask & GROUP_UPDATE_FLAG_POWER_TYPE)
            buffer << p.powerType;

        if (p.updateMask & GROUP_UPDATE_FLAG_CUR_POWER)
            buffer << p.currentPower;

        if (p.updateMask & GROUP_UPDATE_FLAG_MAX_POWER)
            buffer << p.maxPower;

        if (p.updateMask & GROUP_UPDATE_FLAG_LEVEL)
            buffer << p.level;

        if (p.updateMask & GROUP_UPDATE_FLAG_ZONE)
            buffer << p.zone;

        if (p.updateMask & GROUP_UPDATE_FLAG_POSITION)
            buffer << p.posX << p.posY;

        if (p.updateMask & GROUP_UPDATE_FLAG_AURAS)
        {
            buffer << p.positiveAurasMask;
            for (uint32 i = 0; i < MAX_POSITIVE_AURAS; ++i)
                if (p.positiveAurasMask & (uint32(1) << i))
                    buffer << p.positiveAuraSpellIds[i];
        }

        if (p.updateMask & GROUP_UPDATE_FLAG_AURAS_NEGATIVE)
        {
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
            buffer << uint16(p.negativeAurasMask >> 32);
#else
            buffer << uint8(p.negativeAurasMask >> 32);
#endif
            for (uint32 i = 0; i < MAX_AURAS - MAX_POSITIVE_AURAS; ++i)
                if (p.negativeAurasMask & (uint64(1) << (i + MAX_POSITIVE_AURAS)))
                    buffer << p.negativeAuraSpellIds[i];
        }

        if (p.updateMask & GROUP_UPDATE_FLAG_PET_GUID)
            buffer << p.petGuid;

        if (p.updateMask & GROUP_UPDATE_FLAG_PET_NAME)
        {
            if (!p.petName.empty())
                buffer << p.petName;
            else
                buffer << uint8(0);
        }

        if (p.updateMask & GROUP_UPDATE_FLAG_PET_MODEL_ID)
            buffer << p.petModelId;

        if (p.updateMask & GROUP_UPDATE_FLAG_PET_CUR_HP)
            buffer << p.petCurrentHp;

        if (p.updateMask & GROUP_UPDATE_FLAG_PET_MAX_HP)
            buffer << p.petMaxHp;

        if (p.updateMask & GROUP_UPDATE_FLAG_PET_POWER_TYPE)
            buffer << p.petPowerType;

        if (p.updateMask & GROUP_UPDATE_FLAG_PET_CUR_POWER)
            buffer << p.petCurrentPower;

        if (p.updateMask & GROUP_UPDATE_FLAG_PET_MAX_POWER)
            buffer << p.petMaxPower;

        if (p.updateMask & GROUP_UPDATE_FLAG_PET_AURAS)
        {
            buffer << p.petPositiveAurasMask;
            for (uint32 i = 0; i < MAX_POSITIVE_AURAS; ++i)
                if (p.petPositiveAurasMask & (uint32(1) << i))
                    buffer << p.petPositiveAuraSpellIds[i];
        }

        if (p.updateMask & GROUP_UPDATE_FLAG_PET_AURAS_NEGATIVE)
        {
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
            buffer << uint16(p.petNegativeAurasMask >> 32);
#else
            buffer << uint8(p.petNegativeAurasMask >> 32);
#endif
            for (uint32 i = 0; i < MAX_AURAS - MAX_POSITIVE_AURAS; ++i)
                if (p.petNegativeAurasMask & (uint64(1) << (i + MAX_POSITIVE_AURAS)))
                    buffer << p.petNegativeAuraSpellIds[i];
        }
    }
}

void WorldPackets::Group::PartyMemberStats::AppendBodyTo(ByteBuffer& buffer) const
{
    AppendPartyMemberStatsBody(buffer, *this);
}

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_5_1
void WorldPackets::Group::PartyMemberStatsFull::AppendBodyTo(ByteBuffer& buffer) const
{
    AppendPartyMemberStatsBody(buffer, *this);
}
#endif
#endif
