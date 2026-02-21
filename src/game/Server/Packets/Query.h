#ifndef MANGOS_PACKETS_QUERY_H
#define MANGOS_PACKETS_QUERY_H

#include "Packet.h"
#include "ObjectGuid.h"

namespace WorldPackets { namespace Query
{
    class QueryPlayerName final : public ClientPacket
    {
    public:
        ObjectGuid playerGuid;

        explicit QueryPlayerName() : ClientPacket(CMSG_NAME_QUERY) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Query

#endif // MANGOS_PACKETS_QUERY_H
