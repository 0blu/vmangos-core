#ifndef MANGOS_PACKETS_SKILL_H
#define MANGOS_PACKETS_SKILL_H

#include "Packet.h"
#include "SharedDefines.h"

namespace WorldPackets { namespace Skill
{
    class LearnTalent final : public ClientPacket
    {
    public:
        uint32 talent_id;
        uint32 requested_rank;


        explicit LearnTalent() : ClientPacket(CMSG_LEARN_TALENT), talent_id(0), requested_rank(0) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Skill

#endif // MANGOS_PACKETS_SKILL_H
