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

void WorldPackets::Pet::PetAbandon::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Pet::PetRename::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> petGuid;
    recv_data >> name;
}

void WorldPackets::Pet::PetStopAttack::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> petGuid;
}

void WorldPackets::Pet::PetUnlearn::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Pet::PetCancelAura::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
    recv_data >> spellId;
}

void WorldPackets::Pet::PetSpellAutocast::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
    recv_data >> spellId;
    recv_data >> state;
}
