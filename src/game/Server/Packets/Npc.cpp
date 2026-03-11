#include "Npc.h"

void WorldPackets::Npc::GossipHello::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> npcGuid;
}

void WorldPackets::Npc::NpcTextQuery::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> textID;
    recv_data >> guid;
}

void WorldPackets::Npc::TrainerList::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Npc::TrainerBuySpell::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
    recv_data >> spellId;
}

void WorldPackets::Npc::BinderActivate::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> npcGuid;
}

void WorldPackets::Npc::BankerActivate::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}
