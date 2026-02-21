#ifndef MANGOS_PACKETS_PET_H
#define MANGOS_PACKETS_PET_H

#include "Packet.h"
#include "SharedDefines.h"
#include "ObjectGuid.h"

namespace WorldPackets { namespace Pet
{
    class QueryPetName final : public ClientPacket
    {
    public:
        uint32 petNumber;
        ObjectGuid petGuid;

        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Pet

#endif // MANGOS_PACKETS_PET_H
