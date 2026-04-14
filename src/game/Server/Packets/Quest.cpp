#include "Quest.h"

#include "ObjectMgr.h"
#include "QuestDef.h"

void WorldPackets::Quest::QueryQuest::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> questEntry;
}

void WorldPackets::Quest::QuestgiverStatusQuery::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Quest::QuestgiverHello::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Quest::QuestgiverAcceptQuest::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
    recv_data >> quest;
}

void WorldPackets::Quest::QuestgiverQueryQuest::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
    recv_data >> quest;
}

void WorldPackets::Quest::QuestgiverChooseReward::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
    recv_data >> quest;
    recv_data >> reward;
}

void WorldPackets::Quest::QuestgiverRequestReward::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
    recv_data >> quest;
}

void WorldPackets::Quest::QuestgiverCompleteQuest::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
    recv_data >> quest;
}

void WorldPackets::Quest::QuestLogSwapQuest::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> slot1;
    recv_data >> slot2;
}

void WorldPackets::Quest::QuestLogRemoveQuest::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> slot;
}

void WorldPackets::Quest::QuestConfirmAccept::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> questId;
}

void WorldPackets::Quest::PushQuestToParty::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> questId;
}

void WorldPackets::Quest::QuestPushResult::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
    recv_data >> msg;
}

// --- Server Packets ---

void WorldPackets::Quest::QuestPushResultResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << senderGuid;
    buffer << msg;
}

void WorldPackets::Quest::QuestLogFull::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

void WorldPackets::Quest::QuestUpdateComplete::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << questId;
}

void WorldPackets::Quest::QuestGiverQuestComplete::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << questId;
    buffer << unknown;
    buffer << xp;
    buffer << money;
    buffer << static_cast<uint32>(rewardItems.size());
    for (auto const& item : rewardItems)
    {
        buffer << item.itemId;
        buffer << item.itemCount;
    }
}

void WorldPackets::Quest::QuestGiverQuestFailed::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << questId;
    buffer << reason;
}

void WorldPackets::Quest::QuestUpdateFailed::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << questId;
}

void WorldPackets::Quest::QuestUpdateFailedTimer::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << questId;
}

void WorldPackets::Quest::QuestGiverQuestInvalid::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << msg;
}

void WorldPackets::Quest::QuestConfirmAcceptResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << questId;
    buffer << questTitle;
    buffer << senderGuid;
}

void WorldPackets::Quest::QuestUpdateAddItem::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << itemId;
    buffer << count;
}

void WorldPackets::Quest::QuestUpdateAddKill::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << questId;
    buffer << entry;
    buffer << count;
    buffer << required;
    buffer << guid;
}

void WorldPackets::Quest::QuestGiverStatus::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << npcGuid;
    buffer << status;
}

void WorldPackets::Quest::QuestGiverQuestList::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << npcGuid;
    buffer << greetingText;
    buffer << emoteDelay;
    buffer << emote;
    buffer << static_cast<uint8>(quests.size());
    for (auto const& quest : quests)
    {
        buffer << quest.questId;
        buffer << quest.icon;
        buffer << quest.questLevel;
        buffer << quest.title;
    }
}

void WorldPackets::Quest::QuestGiverQuestDetails::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << npcGuid;
    buffer << questId;
    buffer << title;
    buffer << details;
    buffer << objectives;
    buffer << autoFinish;

    if (hiddenRewards)
    {
        buffer << uint32(0); // Rewarded chosen items hidden
        buffer << uint32(0); // Rewarded items hidden
        buffer << uint32(0); // Rewarded money hidden
    }
    else
    {
        buffer << static_cast<uint32>(rewardChoiceItems.size());
        for (auto const& item : rewardChoiceItems)
        {
            buffer << item.itemId;
            buffer << item.itemCount;
            buffer << item.displayInfoId;
        }

        buffer << static_cast<uint32>(rewardItems.size());
        for (auto const& item : rewardItems)
        {
            buffer << item.itemId;
            buffer << item.itemCount;
            buffer << item.displayInfoId;
        }

        buffer << rewMoney;
    }

    buffer << rewSpell;
    buffer << static_cast<uint32>(emotes.size());
    for (auto const& emote : emotes)
    {
        buffer << emote.emoteId;
        buffer << emote.emoteDelay;
    }
}

void WorldPackets::Quest::QuestGiverOfferReward::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << npcGuid;
    buffer << questId;
    buffer << title;
    buffer << offerRewardText;
    buffer << autoFinish;

    buffer << static_cast<uint32>(emotes.size());
    for (auto const& emote : emotes)
    {
        buffer << emote.emoteDelay;
        buffer << emote.emoteId;
    }

    buffer << static_cast<uint32>(rewardChoiceItems.size());
    for (auto const& item : rewardChoiceItems)
    {
        buffer << item.itemId;
        buffer << item.itemCount;
        buffer << item.displayInfoId;
    }

    buffer << static_cast<uint32>(rewardItems.size());
    for (auto const& item : rewardItems)
    {
        buffer << item.itemId;
        buffer << item.itemCount;
        buffer << item.displayInfoId;
    }

    buffer << rewMoney;
    buffer << questFlags;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_4_2
    buffer << rewSpell;
#endif
}

void WorldPackets::Quest::QuestGiverRequestItems::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << npcGuid;
    buffer << questId;
    buffer << title;
    buffer << requestItemsText;
    buffer << emoteDelay;
    buffer << emoteId;
    buffer << closeOnCancel;
    buffer << requiredMoney;
    buffer << static_cast<uint32>(requiredItems.size());
    for (auto const& item : requiredItems)
    {
        buffer << item.itemId;
        buffer << item.itemCount;
        buffer << item.displayInfoId;
    }
    buffer << unknown;
    buffer << completableFlags;
    buffer << flags2;
    buffer << flags3;
}

void WorldPackets::Quest::QuestQueryResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    char const* Title = quest->GetTitle().c_str();
    size_t titleLen = quest->GetTitle().length();
    char const* Details = quest->GetDetails().c_str();
    size_t detailsLen = quest->GetDetails().length();
    char const* Objectives = quest->GetObjectives().c_str();
    size_t objectivesLen = quest->GetObjectives().length();
    char const*  EndText = quest->GetEndText().c_str();
    size_t endTextLen = quest->GetEndText().length();
    char const* ObjectiveText[QUEST_OBJECTIVES_COUNT];
    size_t objectiveTextLen[QUEST_OBJECTIVES_COUNT];

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        ObjectiveText[i] = quest->ObjectiveText[i].c_str();
        objectiveTextLen[i] = quest->ObjectiveText[i].length();
    }

    int const loc_idx = sessionDbLocaleIndex;
    if (loc_idx >= 0)
    {
        if (QuestLocale const* ql = sObjectMgr.GetQuestLocale(quest->GetQuestId()))
        {
            if (ql->Title.size() >(size_t)loc_idx && !ql->Title[loc_idx].empty())
            {
                Title = ql->Title[loc_idx].c_str();
                titleLen = ql->Title[loc_idx].length();
            }
            if (ql->Details.size() > (size_t)loc_idx && !ql->Details[loc_idx].empty())
            {
                Details = ql->Details[loc_idx].c_str();
                detailsLen = ql->Details[loc_idx].length();
            }
            if (ql->Objectives.size() > (size_t)loc_idx && !ql->Objectives[loc_idx].empty())
            {
                Objectives = ql->Objectives[loc_idx].c_str();
                objectivesLen = ql->Objectives[loc_idx].length();
            }
            if (ql->EndText.size() > (size_t)loc_idx && !ql->EndText[loc_idx].empty())
            {
                EndText = ql->EndText[loc_idx].c_str();
                endTextLen = ql->EndText[loc_idx].length();
            }

            for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
            {
                if (ql->ObjectiveText[i].size() >(size_t)loc_idx && !ql->ObjectiveText[i][loc_idx].empty())
                {
                    ObjectiveText[i] = ql->ObjectiveText[i][loc_idx].c_str();
                    objectiveTextLen[i] = ql->ObjectiveText[i][loc_idx].length();
                }
            }
        }
    }

    buffer << uint32(quest->GetQuestId());                   // quest id
    buffer << uint32(quest->GetQuestMethod());               // Accepted values: 0, 1 or 2. 0==IsAutoComplete() (skip objectives/details)
    buffer << uint32(quest->GetQuestLevel());                // may be 0, static data, in other cases must be used dynamic level: Player::GetQuestLevelForPlayer
    buffer << int32(quest->GetZoneOrSort());                 // zone or sort to display in quest log

    buffer << uint32(quest->GetType());
    //[-ZERO] data << uint32(pQuest->GetSuggestedPlayers());

    buffer << uint32(quest->GetRepObjectiveFaction());       // shown in quest log as part of quest objective
    buffer << uint32(quest->GetRepObjectiveValue());         // shown in quest log as part of quest objective

    buffer << uint32(0);                                      // RequiredOpositeRepFaction
    buffer << uint32(0);                                      // RequiredOpositeRepValue, required faction value with another (oposite) faction (objective)

    buffer << uint32(quest->GetNextQuestInChain());          // client will request this quest from NPC, if not 0

    if (quest->HasQuestFlag(QUEST_FLAGS_HIDDEN_REWARDS))
        buffer << uint32(0);                                  // Hide money rewarded
    else
        buffer << uint32(quest->GetRewOrReqMoney());

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
    buffer << uint32(quest->GetRewMoneyMaxLevel());          // used in XP calculation at client
#endif
    buffer << uint32(quest->GetRewSpell());                  // reward spell, this spell will display (icon) (casted if RewSpellCast==0)

    buffer << uint32(quest->GetSrcItemId());                 // source item id
    buffer << uint32(quest->GetQuestFlags());                // quest flags

    int iI;

    if (quest->HasQuestFlag(QUEST_FLAGS_HIDDEN_REWARDS))
    {
        for (iI = 0; iI < QUEST_REWARDS_COUNT; ++iI)
            buffer << uint32(0) << uint32(0);
        for (iI = 0; iI < QUEST_REWARD_CHOICES_COUNT; ++iI)
            buffer << uint32(0) << uint32(0);
    }
    else
    {
        for (iI = 0; iI < QUEST_REWARDS_COUNT; ++iI)
        {
            buffer << uint32(quest->RewItemId[iI]);
            buffer << uint32(quest->RewItemCount[iI]);
        }
        for (iI = 0; iI < QUEST_REWARD_CHOICES_COUNT; ++iI)
        {
            buffer << uint32(quest->RewChoiceItemId[iI]);
            buffer << uint32(quest->RewChoiceItemCount[iI]);
        }
    }

    buffer << quest->GetPointMapId();
    buffer << quest->GetPointX();
    buffer << quest->GetPointY();
    buffer << quest->GetPointOpt();

    buffer.append(Title, titleLen + 1);
    buffer.append(Objectives, objectivesLen + 1);
    buffer.append(Details, detailsLen + 1);
    buffer.append(EndText, endTextLen + 1);

    for (iI = 0; iI < QUEST_OBJECTIVES_COUNT; ++iI)
    {
        if (quest->ReqCreatureOrGOId[iI] < 0)
        {
            // client expected gameobject template id in form (id|0x80000000)
            buffer << uint32((quest->ReqCreatureOrGOId[iI] * (-1)) | 0x80000000);
        }
        else
            buffer << uint32(quest->ReqCreatureOrGOId[iI]);
        buffer << uint32(quest->ReqCreatureOrGOCount[iI]);
        buffer << uint32(quest->ReqItemId[iI]);
        buffer << uint32(quest->ReqItemCount[iI]);
    }

    for (iI = 0; iI < QUEST_OBJECTIVES_COUNT; ++iI)
        buffer.append(ObjectiveText[iI], objectiveTextLen[iI] + 1);
}
