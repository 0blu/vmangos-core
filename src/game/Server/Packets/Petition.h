#ifndef MANGOS_PACKETS_PETITION_H
#define MANGOS_PACKETS_PETITION_H

#include "Packet.h"
#include "ObjectGuid.h"

namespace WorldPackets { namespace Petition
{
    class PetitionShow final : public ClientPacket
    {
    public:
        ObjectGuid petitionerNpcGuid;

        explicit PetitionShow() : ClientPacket(CMSG_PETITION_SHOWLIST) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class PetitionShowSignatures final : public ClientPacket
    {
    public:
        ObjectGuid itemGuid;

        explicit PetitionShowSignatures() : ClientPacket(CMSG_PETITION_SHOW_SIGNATURES) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QueryPetition final : public ClientPacket
    {
    public:
        uint32 petitionGuid = 0;
        ObjectGuid itemGuid;

        explicit QueryPetition() : ClientPacket(CMSG_PETITION_QUERY) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class PetitionDecline final : public ClientPacket
    {
    public:
        ObjectGuid itemGuid;

        explicit PetitionDecline() : ClientPacket(MSG_PETITION_DECLINE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class TurnInPetition final : public ClientPacket
    {
    public:
        ObjectGuid itemGuid;

        explicit TurnInPetition() : ClientPacket(CMSG_TURN_IN_PETITION) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Petition

#endif // MANGOS_PACKETS_PETITION_H
