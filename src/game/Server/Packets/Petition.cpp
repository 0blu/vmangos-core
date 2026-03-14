#include "Petition.h"

void WorldPackets::Petition::PetitionShow::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> petitionerNpcGuid;
}

void WorldPackets::Petition::PetitionShowSignatures::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> itemGuid;
}

void WorldPackets::Petition::QueryPetition::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> petitionGuid;
    recv_data >> itemGuid;
}

void WorldPackets::Petition::PetitionDecline::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> itemGuid;
}

void WorldPackets::Petition::TurnInPetition::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> itemGuid;
}

void WorldPackets::Petition::PetitionRename::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> itemGuid;
    recv_data >> newName;
}
