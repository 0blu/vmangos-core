#include "Item.h"

void WorldPackets::Item::QueryItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> itemEntry;
    recv_data >> itemGuid;
}

void WorldPackets::Item::ReadItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> bag;
    recv_data >> slot;
}

void WorldPackets::Item::AutoEquipItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> srcbag;
    recv_data >> srcslot;
}

void WorldPackets::Item::AutoStoreBagItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> srcbag;
    recv_data >> srcslot;
    recv_data >> dstbag;
}

void WorldPackets::Item::SwapItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> dstbag;
    recv_data >> dstslot;
    recv_data >> srcbag;
    recv_data >> srcslot;
}

void WorldPackets::Item::SwapInvItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> srcslot;
    recv_data >> dstslot;
}

void WorldPackets::Item::SplitItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> srcbag;
    recv_data >> srcslot;
    recv_data >> dstbag;
    recv_data >> dstslot;
    recv_data >> count;
}

void WorldPackets::Item::AutoEquipItemSlot::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> itemGuid;
    recv_data >> dstslot;
}

void WorldPackets::Item::DestroyItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    uint8 data1, data2, data3;
    recv_data >> bag;
    recv_data >> slot;
    recv_data >> count;
    recv_data >> data1;
    recv_data >> data2;
    recv_data >> data3;
}

void WorldPackets::Item::AutoBankItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> srcbag;
    recv_data >> srcslot;
}

void WorldPackets::Item::AutoStoreBankItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> srcbag;
    recv_data >> srcslot;
}

void WorldPackets::Item::SetAmmo::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> item;
}

void WorldPackets::Item::SellItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> vendorGuid;
    recv_data >> itemGuid;
    recv_data >> count;
}

void WorldPackets::Item::ListInventory::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Item::BuyBankSlot::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Item::BuyItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> vendorGuid;
    recv_data >> item;
    recv_data >> count;
    recv_data >> unk1;
}

void WorldPackets::Item::BuyItemInSlot::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> vendorGuid;
    recv_data >> item;
    recv_data >> bagGuid;
    recv_data >> bagslot;
    recv_data >> count;
}

void WorldPackets::Item::WrapItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> giftBag;
    recv_data >> giftSlot;
    recv_data >> itemBag;
    recv_data >> itemSlot;
}

void WorldPackets::Item::BuybackItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> vendorGuid;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_7_1
    recv_data >> slot;
#endif
}

void WorldPackets::Item::BuyBankSlotResult::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << result;
}

void WorldPackets::Item::ItemNameQueryResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << itemId;
    buffer << name;
}

void WorldPackets::Item::ReadItemOk::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << itemGuid;
    buffer << itemGuid;
}

void WorldPackets::Item::ReadItemFailed::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << itemGuid;
    buffer << reason;
    buffer << itemGuid;
}

void WorldPackets::Item::ItemEnchantTimeUpdate::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << itemGuid;
    buffer << slot;
    buffer << duration;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
    buffer << playerGuid;
#endif
}

void WorldPackets::Item::ListInventoryResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << vendorGuid;
    if (items.empty())
    {
        buffer << uint8(0);
        buffer << errorCode;
        return;
    }
    buffer << uint8(items.size());
    for (auto const& item : items)
    {
        buffer << item.slot;
        buffer << item.itemId;
        buffer << item.displayInfoId;
        buffer << item.currentCount;
        buffer << item.price;
        buffer << item.maxDurability;
        buffer << item.buyCount;
    }
}

void WorldPackets::Item::ItemQuerySingleResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    if (!found)
    {
        buffer << uint32(itemEntry | 0x80000000);
        return;
    }

    buffer << itemEntry;
    buffer << itemClass;
    buffer << subClass;
    buffer << name;
    buffer << uint8(0); // Name2
    buffer << uint8(0); // Name3
    buffer << uint8(0); // Name4
    buffer << displayInfoId;
    buffer << quality;
    buffer << flags;
    buffer << buyPrice;
    buffer << sellPrice;
    buffer << inventoryType;
    buffer << allowableClass;
    buffer << allowableRace;
    buffer << itemLevel;
    buffer << requiredLevel;
    buffer << requiredSkill;
    buffer << requiredSkillRank;
    buffer << requiredSpell;
    buffer << requiredHonorRank;
    buffer << requiredCityRank;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
    buffer << requiredReputationFaction;
    buffer << requiredReputationRank;
#endif
    buffer << maxCount;
    buffer << stackable;
    buffer << containerSlots;
    for (int i = 0; i < 10; ++i)
    {
        buffer << itemStats[i].type;
        buffer << itemStats[i].value;
    }
    for (int i = 0; i < 5; ++i)
    {
        buffer << damages[i].damageMin;
        buffer << damages[i].damageMax;
        buffer << damages[i].damageType;
    }
    buffer << armor;
    buffer << holyRes;
    buffer << fireRes;
    buffer << natureRes;
    buffer << frostRes;
    buffer << shadowRes;
    buffer << arcaneRes;
    buffer << delay;
    buffer << ammoType;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
    buffer << rangedModRange;
#endif
    for (int i = 0; i < 5; ++i)
    {
        buffer << spells[i].spellId;
        buffer << spells[i].spellTrigger;
        buffer << spells[i].spellCharges;
        buffer << spells[i].spellCooldown;
        buffer << spells[i].spellCategory;
        buffer << spells[i].spellCategoryCooldown;
    }
    buffer << bonding;
    buffer << description;
    buffer << pageText;
    buffer << languageId;
    buffer << pageMaterial;
    buffer << startQuest;
    buffer << lockId;
    buffer << material;
    buffer << sheath;
    buffer << randomProperty;
    buffer << block;
    buffer << itemSet;
    buffer << maxDurability;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
    buffer << area;
#endif
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
    buffer << map;
#endif
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    buffer << bagFamily;
#endif
}
