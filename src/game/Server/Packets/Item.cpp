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
    buffer << uint8(reason);
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
        buffer << uint8(errorCode);
        return;
    }
    buffer << uint8(items.size());
    for (auto const& item : items)
    {
        buffer << uint32(item.slot);
        buffer << uint32(item.itemId);
        buffer << uint32(item.displayInfoId);
        buffer << uint32(item.currentCount);
        buffer << uint32(item.price);
        buffer << uint32(item.maxDurability);
        buffer << uint32(item.buyCount);
    }
}

void WorldPackets::Item::ItemQuerySingleResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    if (!found)
    {
        buffer << uint32(itemId | 0x80000000);
        return;
    }

    buffer << uint32(itemId);
    buffer << uint32(itemClass);
    buffer << uint32(subClass);
    buffer << name;
    buffer << uint8(0); // Name2
    buffer << uint8(0); // Name3
    buffer << uint8(0); // Name4
    buffer << uint32(displayInfoId);
    buffer << uint32(quality);
    buffer << uint32(flags);
    buffer << uint32(buyPrice);
    buffer << uint32(sellPrice);
    buffer << uint32(inventoryType);
    buffer << uint32(allowableClass);
    buffer << uint32(allowableRace);
    buffer << uint32(itemLevel);
    buffer << uint32(requiredLevel);
    buffer << uint32(requiredSkill);
    buffer << uint32(requiredSkillRank);
    buffer << uint32(requiredSpell);
    buffer << uint32(requiredHonorRank);
    buffer << uint32(requiredCityRank);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
    buffer << uint32(requiredReputationFaction);
    buffer << uint32(requiredReputationRank);
#endif
    buffer << int32(maxCount);
    buffer << int32(stackable);
    buffer << uint32(containerSlots);
    for (int i = 0; i < 10; ++i)
    {
        buffer << uint32(itemStats[i].type);
        buffer << int32(itemStats[i].value);
    }
    for (int i = 0; i < 5; ++i)
    {
        buffer << float(damages[i].damageMin);
        buffer << float(damages[i].damageMax);
        buffer << uint32(damages[i].damageType);
    }
    buffer << uint32(armor);
    buffer << uint32(holyRes);
    buffer << uint32(fireRes);
    buffer << uint32(natureRes);
    buffer << uint32(frostRes);
    buffer << uint32(shadowRes);
    buffer << uint32(arcaneRes);
    buffer << uint32(delay);
    buffer << uint32(ammoType);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
    buffer << float(rangedModRange);
#endif
    for (int i = 0; i < 5; ++i)
    {
        buffer << uint32(spells[i].spellId);
        buffer << uint32(spells[i].spellTrigger);
        buffer << int32(spells[i].spellCharges);
        buffer << uint32(spells[i].spellCooldown);
        buffer << uint32(spells[i].spellCategory);
        buffer << uint32(spells[i].spellCategoryCooldown);
    }
    buffer << uint32(bonding);
    buffer << description;
    buffer << uint32(pageText);
    buffer << uint32(languageId);
    buffer << uint32(pageMaterial);
    buffer << uint32(startQuest);
    buffer << uint32(lockId);
    buffer << int32(material);
    buffer << uint32(sheath);
    buffer << int32(randomProperty);
    buffer << uint32(block);
    buffer << uint32(itemSet);
    buffer << uint32(maxDurability);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
    buffer << uint32(area);
#endif
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
    buffer << uint32(map);
#endif
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    buffer << uint32(bagFamily);
#endif
}
