#include "Quest.h"

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

void WorldPackets::Quest::QuestQueryResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << questId;
    buffer << questMethod;
    buffer << questLevel;
    buffer << zoneOrSort;
    buffer << type;
    buffer << repObjectiveFaction;
    buffer << repObjectiveValue;
    buffer << uint32(0); // RequiredOpositeRepFaction
    buffer << uint32(0); // RequiredOpositeRepValue
    buffer << nextQuestInChain;
    buffer << rewOrReqMoney;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
    buffer << rewMoneyMaxLevel;
#endif
    buffer << rewSpell;
    buffer << srcItemId;
    buffer << questFlags;

    for (int i = 0; i < 4; ++i)
    {
        buffer << rewItemId[i];
        buffer << rewItemCount[i];
    }
    for (int i = 0; i < 6; ++i)
    {
        buffer << rewChoiceItemId[i];
        buffer << rewChoiceItemCount[i];
    }

    buffer << pointMapId;
    buffer << pointX;
    buffer << pointY;
    buffer << pointOpt;

    buffer.append(title.c_str(), title.length() + 1);
    buffer.append(objectives.c_str(), objectives.length() + 1);
    buffer.append(details.c_str(), details.length() + 1);
    buffer.append(endText.c_str(), endText.length() + 1);

    for (int i = 0; i < 4; ++i)
    {
        buffer << questObjectives[i].creatureOrGOId;
        buffer << questObjectives[i].creatureOrGOCount;
        buffer << questObjectives[i].itemId;
        buffer << questObjectives[i].itemCount;
    }

    for (int i = 0; i < 4; ++i)
        buffer.append(objectiveText[i].c_str(), objectiveText[i].length() + 1);
}
