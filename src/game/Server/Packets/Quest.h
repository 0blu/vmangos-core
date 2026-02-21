#ifndef MANGOS_PACKETS_QUEST_H
#define MANGOS_PACKETS_QUEST_H

#include "Packet.h"
#include "SharedDefines.h"

namespace WorldPackets { namespace Quest
{
    class QueryQuest final : public ClientPacket
    {
    public:
        uint32 questEntry; // db entry id

        explicit QueryQuest() : ClientPacket(CMSG_QUEST_QUERY), questEntry(0) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Quest

#endif // MANGOS_PACKETS_QUEST_H
