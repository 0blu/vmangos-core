#include "Taxi.h"

#include <Errors.h>

void WorldPackets::Taxi::TaxiNodeStatusQuery::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> creatureGuidNearTaxi; // Normal clients would send the GUID of the flightmaster
}

void WorldPackets::Taxi::TaxiQueryAvailableNodes::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Taxi::ActivateTaxi::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> flightmasterGuid;
    recv_data >> node1;
    recv_data >> node2;
}

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
void WorldPackets::Taxi::ActivateTaxiExpress::ReadFromWorldPacket(WorldPacket& recv_data)
{
    uint32 node_count;
    recv_data >> flightmasterGuid;
    recv_data >> totalcost;
    recv_data >> node_count;
    for (uint32 i = 0; i < node_count; ++i)
    {
        uint32 node;
        recv_data >> node;
        nodes.push_back(node);
    }
}
#endif

void WorldPackets::Taxi::TaxiNodeStatus::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << flightmasterGuid;
    buffer << isKnown;
}

void WorldPackets::Taxi::NewTaxiPath::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

void WorldPackets::Taxi::ShowTaxiNodes::AppendBodyTo(ByteBuffer& buffer) const
{
    bool constexpr requestWasOkay = true;
    // if false "You dont know any flight locations connected to this one"
    // and the nodes below are just for client debug purposes
    buffer << static_cast<uint32>(requestWasOkay);
    if (requestWasOkay)
    {
        buffer << flightmasterGuid;
        buffer << currentNode;
    }

    // Vanilla client reads 4 * uint64, but it also swaps the endians, so we are just doing 8 * uint32
    uint32 binaryNodeBuffer[8]{};
    for (auto const& nodeEntry : knownNodes)
    {
        uint32 nodeIndex = nodeEntry - 1;
        uint8  field = uint8(nodeIndex / 32);
        uint32 submask = 1 << (nodeIndex % 32);
        MANGOS_DEBUG_ASSERT(field < 8);
        binaryNodeBuffer[field] |= submask;
    }
    for (auto const& bufPart : binaryNodeBuffer)
    {
        buffer << bufPart;
    }
}
