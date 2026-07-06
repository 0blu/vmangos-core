#include "Item.h"
#include "ItemDefines.h"
#include "SpellEntry.h"

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

void WorldPackets::Item::OpenContainer::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << itemGuid;
}

void WorldPackets::Item::BuyFailed::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << vendorGuid;
    buffer << itemEntry;
    buffer << reason;
}

void WorldPackets::Item::SellItemResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << vendorGuid;
    buffer << itemGuid;
    buffer << reason;
}

void WorldPackets::Item::BuyItemResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << vendorGuid;
    buffer << vendorSlot;
    buffer << newCount;
    buffer << purchaseCount;
}

void WorldPackets::Item::InventoryChangeFailure::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << reason;
    if (reason != static_cast<uint8>(EQUIP_ERR_OK))
    {
        if (reason == static_cast<uint8>(EQUIP_ERR_CANT_EQUIP_LEVEL_I))
            buffer << requiredLevel;
        buffer << item1Guid;
        buffer << item2Guid;
        buffer << bagSlot;
    }
}

void WorldPackets::Item::ItemPushResult::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << playerGuid;
    buffer << received;
    buffer << created;
    buffer << showInChat;
    buffer << bagSlot;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
    buffer << itemSlot;
#endif
    buffer << itemEntry;
    buffer << suffixFactor;
    buffer << randomPropertyId;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
    buffer << count;
#endif
}

void WorldPackets::Item::ItemCooldown::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << itemGuid;
    buffer << spellId;
}

void WorldPackets::Item::EnchantmentLog::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << casterGuid;
    buffer << ownerGuid;
    buffer << itemEntry;
    buffer << spellId;
    buffer << uint8(showAffiliation ? 1 : 0);
}

void WorldPackets::Item::ItemQuerySingleResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    if (!found)
    {
        buffer << (itemEntry | 0x80000000);
        return;
    }

    buffer << itemEntry;
    buffer << itemClass;
    buffer << itemSubClass;
    buffer << name;
    buffer << "";
    buffer << "";
    buffer << "";
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
    for (ItemStatValue const& stat : stats)
    {
        buffer << stat.type;
        buffer << stat.value;
    }
    for (ItemDamageValue const& damage : damages)
    {
        buffer << damage.minimum;
        buffer << damage.maximum;
        buffer << damage.type;
    }
    buffer << armor;
    buffer << holyResistance;
    buffer << fireResistance;
    buffer << natureResistance;
    buffer << frostResistance;
    buffer << shadowResistance;
    buffer << arcaneResistance;
    buffer << delay;
    buffer << ammoType;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
    buffer << rangedModRange;
#endif
    for (ItemSpellValue const& spell : spells)
    {
        buffer << spell.spellId;
        buffer << spell.trigger;
        buffer << spell.charges;
        buffer << spell.cooldown;
        buffer << spell.category;
        buffer << spell.categoryCooldown;
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

void WorldPackets::Item::ListInventoryResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << vendorGuid;
    buffer << uint8(items.size());
    if (items.empty())
    {
        buffer << reason;
        return;
    }

    for (VendorInventoryItem const& item : items)
    {
        buffer << item.slot;
        buffer << item.itemEntry;
        buffer << item.displayInfoId;
        buffer << item.availableCount;
        buffer << item.price;
        buffer << item.maxDurability;
        buffer << item.buyCount;
    }
}
