#include "Group.h"
#include "Group/Group.h"
#include "Player.h"

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
    void AppendPartyMemberStats(ByteBuffer& buffer, ::Player const* player, uint32 mask, bool sendAllAuras)
    {
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
        buffer << player->GetPackGUID();
#else
        buffer << player->GetGUID();
#endif
        buffer << mask;

        if (mask & GROUP_UPDATE_FLAG_STATUS)
            buffer << uint8(GetGroupMemberStatus(player));

        if (mask & GROUP_UPDATE_FLAG_CUR_HP)
            buffer << uint16(player->GetHealth());

        if (mask & GROUP_UPDATE_FLAG_MAX_HP)
            buffer << uint16(player->GetMaxHealth());

        Powers powerType = player->GetPowerType();
        if (mask & GROUP_UPDATE_FLAG_POWER_TYPE)
            buffer << uint8(powerType);

        if (mask & GROUP_UPDATE_FLAG_CUR_POWER)
            buffer << uint16(player->GetPower(powerType));

        if (mask & GROUP_UPDATE_FLAG_MAX_POWER)
            buffer << uint16(player->GetMaxPower(powerType));

        if (mask & GROUP_UPDATE_FLAG_LEVEL)
            buffer << uint16(player->GetLevel());

        if (mask & GROUP_UPDATE_FLAG_ZONE)
            buffer << uint16(player->GetCachedZoneId());

        if (mask & GROUP_UPDATE_FLAG_POSITION)
            buffer << int16(player->GetPositionX()) << int16(player->GetPositionY());

        if (mask & GROUP_UPDATE_FLAG_AURAS)
        {
            uint64 auramask = sendAllAuras ? player->GetAuraApplicationMask() : player->GetAuraUpdateMask();
            buffer << uint32(auramask);
            for (uint32 i = 0; i < MAX_POSITIVE_AURAS; ++i)
                if (auramask & (uint64(1) << i))
                    buffer << uint16(player->GetUInt32Value(UNIT_FIELD_AURA + i));
        }

        if (mask & GROUP_UPDATE_FLAG_AURAS_NEGATIVE)
        {
            uint64 auramask = sendAllAuras ? player->GetNegativeAuraApplicationMask() : player->GetAuraUpdateMask();
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
            uint16 maskForClient = uint16(auramask >> 32);
#else
            uint8 maskForClient = uint8(auramask >> 32);
#endif
            buffer << maskForClient;
            for (uint64 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; ++i)
                if (auramask & (uint64(1) << i))
                    buffer << uint16(player->GetUInt32Value(UNIT_FIELD_AURA + i));
        }

        Pet* pet = player->GetPet();
        if (mask & GROUP_UPDATE_FLAG_PET_GUID)
            buffer << (pet ? pet->GetObjectGuid() : ObjectGuid());

        if (mask & GROUP_UPDATE_FLAG_PET_NAME)
        {
            if (pet)
                buffer << pet->GetName();
            else
                buffer << uint8(0);
        }

        if (mask & GROUP_UPDATE_FLAG_PET_MODEL_ID)
            buffer << uint16(pet ? pet->GetDisplayId() : 0);

        if (mask & GROUP_UPDATE_FLAG_PET_CUR_HP)
            buffer << uint16(pet ? pet->GetHealth() : 0);

        if (mask & GROUP_UPDATE_FLAG_PET_MAX_HP)
            buffer << uint16(pet ? pet->GetMaxHealth() : 0);

        if (mask & GROUP_UPDATE_FLAG_PET_POWER_TYPE)
            buffer << uint8(pet ? pet->GetPowerType() : 0);

        if (mask & GROUP_UPDATE_FLAG_PET_CUR_POWER)
            buffer << uint16(pet ? pet->GetPower(pet->GetPowerType()) : 0);

        if (mask & GROUP_UPDATE_FLAG_PET_MAX_POWER)
            buffer << uint16(pet ? pet->GetMaxPower(pet->GetPowerType()) : 0);

        if (mask & GROUP_UPDATE_FLAG_PET_AURAS)
        {
            if (pet)
            {
                uint64 auramask = sendAllAuras ? pet->GetAuraApplicationMask() : pet->GetAuraUpdateMask();
                buffer << uint32(auramask);
                for (uint32 i = 0; i < MAX_POSITIVE_AURAS; ++i)
                    if (auramask & (uint64(1) << i))
                        buffer << uint16(pet->GetUInt32Value(UNIT_FIELD_AURA + i));
            }
            else
                buffer << uint32(0);
        }

        if (mask & GROUP_UPDATE_FLAG_PET_AURAS_NEGATIVE)
        {
            if (pet)
            {
                uint64 auramask = sendAllAuras ? pet->GetNegativeAuraApplicationMask() : pet->GetAuraUpdateMask();
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
                uint16 maskForClient = uint16(auramask >> 32);
#else
                uint8 maskForClient = uint8(auramask >> 32);
#endif
                buffer << maskForClient;
                for (uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; ++i)
                    if (auramask & (uint64(1) << i))
                        buffer << uint16(pet->GetUInt32Value(UNIT_FIELD_AURA + i));
            }
            else
                buffer << uint16(0);
        }
    }
}

void WorldPackets::Group::PartyMemberStats::AppendBodyTo(ByteBuffer& buffer) const
{
    AppendPartyMemberStats(buffer, player, updateMask, sendAllAuras);
}

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_5_1
void WorldPackets::Group::PartyMemberStatsFull::AppendBodyTo(ByteBuffer& buffer) const
{
    AppendPartyMemberStats(buffer, player, updateMask, sendAllAuras);
}
#endif
#endif
