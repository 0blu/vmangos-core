#include "Pet.h"

void WorldPackets::Pet::QueryPetName::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> petNumber;
    recv_data >> petGuid;
}

void WorldPackets::Pet::PetAction::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> petGuid;
    recv_data >> data;
    recv_data >> targetGuid;
}
