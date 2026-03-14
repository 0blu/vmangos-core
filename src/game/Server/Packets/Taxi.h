#ifndef MANGOS_PACKETS_TAXI_H
#define MANGOS_PACKETS_TAXI_H

#include "Packet.h"
#include "ObjectGuid.h"

namespace WorldPackets { namespace Taxi
{
    class TaxiNodeStatusQuery final : public ClientPacket
    {
    public:
        ObjectGuid creatureGuidNearTaxi;

        explicit TaxiNodeStatusQuery() : ClientPacket(CMSG_TAXINODE_STATUS_QUERY) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class TaxiQueryAvailableNodes final : public ClientPacket
    {
    public:
        ObjectGuid guid;

        explicit TaxiQueryAvailableNodes() : ClientPacket(CMSG_TAXIQUERYAVAILABLENODES) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class ActivateTaxi final : public ClientPacket
    {
    public:
        ObjectGuid guid;
        uint32     node1 = 0;
        uint32     node2 = 0;

        explicit ActivateTaxi() : ClientPacket(CMSG_ACTIVATETAXI) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Taxi

#endif // MANGOS_PACKETS_TAXI_H
