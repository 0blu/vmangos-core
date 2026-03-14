#include "Taxi.h"

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
    recv_data >> guid;
    recv_data >> node1;
    recv_data >> node2;
}
