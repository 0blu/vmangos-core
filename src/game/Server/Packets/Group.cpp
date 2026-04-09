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

void WorldPackets::Group::RequestPartyMemberStats::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

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

void WorldPackets::Group::RaidReadyCheck::ReadFromWorldPacket(WorldPacket& recv_data)
{
    if (!recv_data.empty())
    {
        uint8 s;
        recv_data >> s;
        state = s;
    }
}
#endif

void WorldPackets::Group::PartyCommandResult::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << operation;
    buffer << member;
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

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
void WorldPackets::Group::RaidReadyCheckResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << senderGuid;
    buffer << state;
}
#endif

void WorldPackets::Group::WritePartyMemberStats(ByteBuffer& buffer, WorldPackets::Group::PartyMemberStatsData const& data)
{
    buffer << data.guid.WriteAsPackedClientBuildAware();
    buffer << data.mask;

    if (data.mask & GROUP_UPDATE_FLAG_STATUS)
        buffer << data.status;

    if (data.mask & GROUP_UPDATE_FLAG_CUR_HP)
        buffer << data.curHP;

    if (data.mask & GROUP_UPDATE_FLAG_MAX_HP)
        buffer << data.maxHP;

    if (data.mask & GROUP_UPDATE_FLAG_POWER_TYPE)
        buffer << data.powerType;

    if (data.mask & GROUP_UPDATE_FLAG_CUR_POWER)
        buffer << data.curPower;

    if (data.mask & GROUP_UPDATE_FLAG_MAX_POWER)
        buffer << data.maxPower;

    if (data.mask & GROUP_UPDATE_FLAG_LEVEL)
        buffer << data.level;

    if (data.mask & GROUP_UPDATE_FLAG_ZONE)
        buffer << data.zone;

    if (data.mask & GROUP_UPDATE_FLAG_POSITION)
    {
        buffer << data.posX;
        buffer << data.posY;
    }

    if (data.mask & GROUP_UPDATE_FLAG_AURAS)
    {
        buffer << data.auras.positiveAuraMask;
        for (uint32 i = 0; i < PARTY_MAX_POSITIVE_AURAS; ++i)
            if (data.auras.positiveAuraMask & (uint32(1) << i))
                buffer << data.auras.positiveAuras[i];
    }

    if (data.mask & GROUP_UPDATE_FLAG_AURAS_NEGATIVE)
    {
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
        buffer << data.auras.negativeAuraMask;
        for (uint8 i = 0; i < PARTY_MAX_NEGATIVE_AURAS; ++i)
            if (data.auras.negativeAuraMask & (1 << i))
                buffer << data.auras.negativeAuras[i];
#else
        buffer << uint8(data.auras.negativeAuraMask);
        for (uint8 i = 0; i < 8; ++i)
            if (data.auras.negativeAuraMask & (1 << i))
                buffer << data.auras.negativeAuras[i];
#endif
    }

    if (data.mask & GROUP_UPDATE_FLAG_PET_GUID)
        buffer << data.petGuid;

    if (data.mask & GROUP_UPDATE_FLAG_PET_NAME)
    {
        if (!data.petName.empty())
            buffer << data.petName;
        else
            buffer << uint8(0);
    }

    if (data.mask & GROUP_UPDATE_FLAG_PET_MODEL_ID)
        buffer << data.petModelId;

    if (data.mask & GROUP_UPDATE_FLAG_PET_CUR_HP)
        buffer << data.petCurHP;

    if (data.mask & GROUP_UPDATE_FLAG_PET_MAX_HP)
        buffer << data.petMaxHP;

    if (data.mask & GROUP_UPDATE_FLAG_PET_POWER_TYPE)
        buffer << data.petPowerType;

    if (data.mask & GROUP_UPDATE_FLAG_PET_CUR_POWER)
        buffer << data.petCurPower;

    if (data.mask & GROUP_UPDATE_FLAG_PET_MAX_POWER)
        buffer << data.petMaxPower;

    if (data.mask & GROUP_UPDATE_FLAG_PET_AURAS)
    {
        buffer << data.petAuras.positiveAuraMask;
        for (uint32 i = 0; i < PARTY_MAX_POSITIVE_AURAS; ++i)
            if (data.petAuras.positiveAuraMask & (uint32(1) << i))
                buffer << data.petAuras.positiveAuras[i];
    }

    if (data.mask & GROUP_UPDATE_FLAG_PET_AURAS_NEGATIVE)
    {
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
        buffer << data.petAuras.negativeAuraMask;
        for (uint8 i = 0; i < PARTY_MAX_NEGATIVE_AURAS; ++i)
            if (data.petAuras.negativeAuraMask & (1 << i))
                buffer << data.petAuras.negativeAuras[i];
#else
        buffer << uint8(data.petAuras.negativeAuraMask);
        for (uint8 i = 0; i < 8; ++i)
            if (data.petAuras.negativeAuraMask & (1 << i))
                buffer << data.petAuras.negativeAuras[i];
#endif
    }
}

void WorldPackets::Group::PartyMemberStatsFull::AppendBodyTo(ByteBuffer& buffer) const
{
    WritePartyMemberStats(buffer, data);
}

void WorldPackets::Group::PartyMemberStats::AppendBodyTo(ByteBuffer& buffer) const
{
    WritePartyMemberStats(buffer, data);
}
