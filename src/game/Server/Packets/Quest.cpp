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

void WorldPackets::Quest::QuestLogFull::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

void WorldPackets::Quest::QuestQueryResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint32(questId);
    buffer << uint32(questMethod);
    buffer << uint32(questLevel);
    buffer << int32(zoneOrSort);
    buffer << uint32(type);
    buffer << uint32(repObjectiveFaction);
    buffer << uint32(repObjectiveValue);
    buffer << uint32(0); // RequiredOpositeRepFaction
    buffer << uint32(0); // RequiredOpositeRepValue
    buffer << uint32(nextQuestInChain);
    buffer << uint32(rewOrReqMoney);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
    buffer << uint32(rewMoneyMaxLevel);
#endif
    buffer << uint32(rewSpell);
    buffer << uint32(srcItemId);
    buffer << uint32(questFlags);

    for (int i = 0; i < 4; ++i)
    {
        buffer << uint32(rewItemId[i]);
        buffer << uint32(rewItemCount[i]);
    }
    for (int i = 0; i < 6; ++i)
    {
        buffer << uint32(rewChoiceItemId[i]);
        buffer << uint32(rewChoiceItemCount[i]);
    }

    buffer << uint32(pointMapId);
    buffer << float(pointX);
    buffer << float(pointY);
    buffer << uint32(pointOpt);

    buffer.append(title.c_str(), title.length() + 1);
    buffer.append(objectives.c_str(), objectives.length() + 1);
    buffer.append(details.c_str(), details.length() + 1);
    buffer.append(endText.c_str(), endText.length() + 1);

    for (int i = 0; i < 4; ++i)
    {
        buffer << uint32(questObjectives[i].creatureOrGOId);
        buffer << uint32(questObjectives[i].creatureOrGOCount);
        buffer << uint32(questObjectives[i].itemId);
        buffer << uint32(questObjectives[i].itemCount);
    }

    for (int i = 0; i < 4; ++i)
        buffer.append(objectiveText[i].c_str(), objectiveText[i].length() + 1);
}
