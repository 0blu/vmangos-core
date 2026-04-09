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
    buffer << result;
}

void WorldPackets::Npc::TrainerBuySucceeded::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << ObjectGuid(trainerGuid);
    buffer << spellId;
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

void WorldPackets::Npc::TrainerListResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << trainerGuid;
    buffer << trainerType;
    buffer << uint32(spells.size());
    for (auto const& spell : spells)
    {
        buffer << spell.spellId;
        buffer << spell.state;
        buffer << spell.cost;
        buffer << spell.canLearnPrimaryProf;
        buffer << spell.isPrimaryProfFirstRank;
        buffer << spell.spellLevel;
        buffer << spell.reqSkill;
        buffer << spell.reqSkillValue;
        buffer << spell.spellReq1;
        buffer << spell.spellReq2;
        buffer << uint32(0);
    }
    buffer << title;
}

void WorldPackets::Npc::ListStabledPetsResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << npcGuid;
    buffer << uint8(pets.size());               // place holder for slot show number
    buffer << numSlots;
    for (auto const& pet : pets)
    {
        buffer << pet.petNumber;
        buffer << pet.entry;
        buffer << pet.level;
        buffer << pet.name;
        buffer << pet.loyalty;
        buffer << pet.slot;
    }
}
