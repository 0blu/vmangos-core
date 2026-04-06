#ifndef MANGOS_PACKETS_QUEST_H
#define MANGOS_PACKETS_QUEST_H

#include "Packet.h"
#include "ObjectGuid.h"
#include "SharedDefines.h"
#include <string>

namespace WorldPackets { namespace Quest
{
    class QueryQuest final : public ClientPacket
    {
    public:
        uint32 questEntry; // db entry id

        explicit QueryQuest() : ClientPacket(CMSG_QUEST_QUERY), questEntry(0) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QuestgiverStatusQuery final : public ClientPacket
    {
    public:
        ObjectGuid guid;

        explicit QuestgiverStatusQuery() : ClientPacket(CMSG_QUESTGIVER_STATUS_QUERY) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QuestgiverHello final : public ClientPacket
    {
    public:
        ObjectGuid guid;

        explicit QuestgiverHello() : ClientPacket(CMSG_QUESTGIVER_HELLO) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QuestgiverAcceptQuest final : public ClientPacket
    {
    public:
        ObjectGuid guid;
        uint32 quest = 0;

        explicit QuestgiverAcceptQuest() : ClientPacket(CMSG_QUESTGIVER_ACCEPT_QUEST) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QuestgiverQueryQuest final : public ClientPacket
    {
    public:
        ObjectGuid guid;
        uint32 quest = 0;

        explicit QuestgiverQueryQuest() : ClientPacket(CMSG_QUESTGIVER_QUERY_QUEST) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QuestgiverChooseReward final : public ClientPacket
    {
    public:
        ObjectGuid guid;
        uint32 quest = 0;
        uint32 reward = 0;

        explicit QuestgiverChooseReward() : ClientPacket(CMSG_QUESTGIVER_CHOOSE_REWARD) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QuestgiverRequestReward final : public ClientPacket
    {
    public:
        ObjectGuid guid;
        uint32 quest = 0;

        explicit QuestgiverRequestReward() : ClientPacket(CMSG_QUESTGIVER_REQUEST_REWARD) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QuestgiverCompleteQuest final : public ClientPacket
    {
    public:
        ObjectGuid guid;
        uint32 quest = 0;

        explicit QuestgiverCompleteQuest() : ClientPacket(CMSG_QUESTGIVER_COMPLETE_QUEST) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QuestLogSwapQuest final : public ClientPacket
    {
    public:
        uint8 slot1 = 0;
        uint8 slot2 = 0;

        explicit QuestLogSwapQuest() : ClientPacket(CMSG_QUESTLOG_SWAP_QUEST) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QuestLogRemoveQuest final : public ClientPacket
    {
    public:
        uint8 slot = 0;

        explicit QuestLogRemoveQuest() : ClientPacket(CMSG_QUESTLOG_REMOVE_QUEST) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QuestConfirmAccept final : public ClientPacket
    {
    public:
        uint32 questId = 0;

        explicit QuestConfirmAccept() : ClientPacket(CMSG_QUEST_CONFIRM_ACCEPT) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class PushQuestToParty final : public ClientPacket
    {
    public:
        uint32 questId = 0;

        explicit PushQuestToParty() : ClientPacket(CMSG_PUSHQUESTTOPARTY) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QuestPushResult final : public ClientPacket
    {
    public:
        ObjectGuid guid;
        uint8 msg = 0;

        explicit QuestPushResult() : ClientPacket(MSG_QUEST_PUSH_RESULT) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
    // --- Server Packets ---

    class QuestLogFull final : public ServerPacket
    {
    public:
        explicit QuestLogFull() : ServerPacket(SMSG_QUESTLOG_FULL) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    struct QuestObjective
    {
        uint32 creatureOrGOId = 0;
        uint32 creatureOrGOCount = 0;
        uint32 itemId = 0;
        uint32 itemCount = 0;
    };

    class QuestQueryResponse final : public ServerPacket
    {
    public:
        uint32 questId = 0;
        uint32 questMethod = 0;               // Accepted values: 0, 1 or 2. 0==IsAutoComplete() (skip objectives/details)
        uint32 questLevel = 0;                 // may be 0, static data, in other cases must be used dynamic level: Player::GetQuestLevelForPlayer
        int32 zoneOrSort = 0;                  // zone or sort to display in quest log
        uint32 type = 0;
        uint32 repObjectiveFaction = 0;        // shown in quest log as part of quest objective
        uint32 repObjectiveValue = 0;          // shown in quest log as part of quest objective
        uint32 nextQuestInChain = 0;           // client will request this quest from NPC, if not 0
        uint32 rewOrReqMoney = 0;              // set to 0 when QUEST_FLAGS_HIDDEN_REWARDS
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
        uint32 rewMoneyMaxLevel = 0;           // used in XP calculation at client
#endif
        uint32 rewSpell = 0;                   // reward spell, this spell will display (icon) (casted if RewSpellCast==0)
        uint32 srcItemId = 0;
        uint32 questFlags = 0;
        uint32 rewItemId[4] = {};
        uint32 rewItemCount[4] = {};
        uint32 rewChoiceItemId[6] = {};
        uint32 rewChoiceItemCount[6] = {};
        uint32 pointMapId = 0;
        float pointX = 0.0f;
        float pointY = 0.0f;
        uint32 pointOpt = 0;
        std::string title;
        std::string objectives;
        std::string details;
        std::string endText;
        QuestObjective questObjectives[4];
        std::string objectiveText[4];

        explicit QuestQueryResponse() : ServerPacket(SMSG_QUEST_QUERY_RESPONSE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

}} // namespace WorldPackets::Quest

#endif // MANGOS_PACKETS_QUEST_H
