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
}} // namespace WorldPackets::Petition

#endif // MANGOS_PACKETS_PETITION_H
