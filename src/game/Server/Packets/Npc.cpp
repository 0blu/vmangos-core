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

void WorldPackets::Npc::TabardVendorActivate::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Npc::SpiritHealerActivate::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Npc::RepairItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> npcGuid;
    recv_data >> itemGuid;
}

void WorldPackets::Npc::ListStabledPets::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> npcGuid;
}

void WorldPackets::Npc::StablePet::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> npcGuid;
}

void WorldPackets::Npc::UnstablePet::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> npcGuid;
    recv_data >> petNumber;
}

void WorldPackets::Npc::BuyStableSlot::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> npcGuid;
}

void WorldPackets::Npc::StableSwapPet::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> npcGuid;
    recv_data >> petNumber;
}

void WorldPackets::Npc::GossipSelectOption::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
    recv_data >> gossipListId;
    if (!recv_data.empty())
    {
        recv_data >> code;
    }
}

// --- Server Packets ---

void WorldPackets::Npc::GossipComplete::AppendBodyTo(ByteBuffer& /*buffer*/) const
{
}

void WorldPackets::Npc::ShowBank::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << ObjectGuid(bankerGuid);
}

void WorldPackets::Npc::StableResult::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint8(result);
}

void WorldPackets::Npc::TrainerBuySucceeded::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << ObjectGuid(trainerGuid);
    buffer << uint32(spellId);
}

void WorldPackets::Npc::TrainerBuyFailed::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << trainerGuid;
    buffer << serviceId;
    buffer << errorCode;
}

void WorldPackets::Npc::TabardVendorActivateResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << tabardVendorNpcGuid;
}

void WorldPackets::Npc::NpcTextUpdate::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << textID;
    for (int i = 0; i < 8; ++i)
    {
        buffer << options[i].probability;
        buffer << options[i].maleText;
        buffer << options[i].femaleText;
        buffer << options[i].languageId;
        buffer << options[i].emoteDelay1;
        buffer << options[i].emoteId1;
        buffer << options[i].emoteDelay2;
        buffer << options[i].emoteId2;
        buffer << options[i].emoteDelay3;
        buffer << options[i].emoteId3;
    }
}
