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

void WorldPackets::Pet::PetSetAction::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> petGuid;
    // Packet with 2 actions is 24 bytes: 8 (GUID) + 2 * 8 (position + data per action)
    count = (recv_data.size() == 24) ? 2 : 1;
    for (uint8 i = 0; i < count; ++i)
    {
        recv_data >> actions[i].position;
        recv_data >> actions[i].data;
    }
}
