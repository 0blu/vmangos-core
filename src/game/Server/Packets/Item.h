#ifndef MANGOS_PACKETS_ITEM_H
#define MANGOS_PACKETS_ITEM_H

#include "Packet.h"
#include "ObjectGuid.h"
#include <string>
#include <vector>

namespace WorldPackets { namespace Item
{
    class QueryItem final : public ClientPacket
    {
    public:
        uint32 itemEntry; // db entry id
        ObjectGuid itemGuid;

        explicit QueryItem() : ClientPacket(CMSG_ITEM_QUERY_SINGLE), itemEntry(0) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class ReadItem final : public ClientPacket
    {
    public:
        uint8 bag = 0;
        uint8 slot = 0;

        explicit ReadItem() : ClientPacket(CMSG_READ_ITEM) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class AutoEquipItem final : public ClientPacket
    {
    public:
        uint8 srcbag = 0;
        uint8 srcslot = 0;

        explicit AutoEquipItem() : ClientPacket(CMSG_AUTOEQUIP_ITEM) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class AutoStoreBagItem final : public ClientPacket
    {
    public:
        uint8 srcbag = 0;
        uint8 srcslot = 0;
        uint8 dstbag = 0;

        explicit AutoStoreBagItem() : ClientPacket(CMSG_AUTOSTORE_BAG_ITEM) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class SwapItem final : public ClientPacket
    {
    public:
        uint8 dstbag = 0;
        uint8 dstslot = 0;
        uint8 srcbag = 0;
        uint8 srcslot = 0;

        explicit SwapItem() : ClientPacket(CMSG_SWAP_ITEM) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class SwapInvItem final : public ClientPacket
    {
    public:
        uint8 srcslot = 0;
        uint8 dstslot = 0;

        explicit SwapInvItem() : ClientPacket(CMSG_SWAP_INV_ITEM) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class SplitItem final : public ClientPacket
    {
    public:
        uint8 srcbag = 0;
        uint8 srcslot = 0;
        uint8 dstbag = 0;
        uint8 dstslot = 0;
        uint8 count = 0;

        explicit SplitItem() : ClientPacket(CMSG_SPLIT_ITEM) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class AutoEquipItemSlot final : public ClientPacket
    {
    public:
        ObjectGuid itemGuid;
        uint8 dstslot = 0;

        explicit AutoEquipItemSlot() : ClientPacket(CMSG_AUTOEQUIP_ITEM_SLOT) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class DestroyItem final : public ClientPacket
    {
    public:
        uint8 bag = 0;
        uint8 slot = 0;
        uint8 count = 0;

        explicit DestroyItem() : ClientPacket(CMSG_DESTROYITEM) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class AutoBankItem final : public ClientPacket
    {
    public:
        uint8 srcbag = 0;
        uint8 srcslot = 0;

        explicit AutoBankItem() : ClientPacket(CMSG_AUTOBANK_ITEM) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class AutoStoreBankItem final : public ClientPacket
    {
    public:
        uint8 srcbag = 0;
        uint8 srcslot = 0;

        explicit AutoStoreBankItem() : ClientPacket(CMSG_AUTOSTORE_BANK_ITEM) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class SetAmmo final : public ClientPacket
    {
    public:
        uint32 item = 0;

        explicit SetAmmo() : ClientPacket(CMSG_SET_AMMO) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class SellItem final : public ClientPacket
    {
    public:
        ObjectGuid vendorGuid;
        ObjectGuid itemGuid;
        uint8 count = 0;

        explicit SellItem() : ClientPacket(CMSG_SELL_ITEM) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class ListInventory final : public ClientPacket
    {
    public:
        ObjectGuid guid;

        explicit ListInventory() : ClientPacket(CMSG_LIST_INVENTORY) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class BuyBankSlot final : public ClientPacket
    {
    public:
        ObjectGuid guid;

        explicit BuyBankSlot() : ClientPacket(CMSG_BUY_BANK_SLOT) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class BuyItem final : public ClientPacket
    {
    public:
        ObjectGuid vendorGuid;
        uint32 item = 0;
        uint8 count = 0;
        uint8 unk1 = 0;

        explicit BuyItem() : ClientPacket(CMSG_BUY_ITEM) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class BuyItemInSlot final : public ClientPacket
    {
    public:
        ObjectGuid vendorGuid;
        ObjectGuid bagGuid;
        uint32 item = 0;
        uint8 bagslot = 0;
        uint8 count = 0;

        explicit BuyItemInSlot() : ClientPacket(CMSG_BUY_ITEM_IN_SLOT) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class WrapItem final : public ClientPacket
    {
    public:
        uint8 giftBag = 0;
        uint8 giftSlot = 0;
        uint8 itemBag = 0;
        uint8 itemSlot = 0;

        explicit WrapItem() : ClientPacket(CMSG_WRAP_ITEM) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class BuybackItem final : public ClientPacket
    {
    public:
        ObjectGuid vendorGuid;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_7_1
        uint32 slot = 0;
#endif

        explicit BuybackItem() : ClientPacket(CMSG_BUYBACK_ITEM) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
    // --- Server Packets ---

    class BuyBankSlotResult final : public ServerPacket
    {
    public:
        uint32 result = 0;

        explicit BuyBankSlotResult() : ServerPacket(SMSG_BUY_BANK_SLOT_RESULT) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class ItemNameQueryResponse final : public ServerPacket
    {
    public:
        uint32 itemId = 0;
        std::string name;

        explicit ItemNameQueryResponse() : ServerPacket(SMSG_ITEM_NAME_QUERY_RESPONSE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class ReadItemOk final : public ServerPacket
    {
    public:
        ObjectGuid itemGuid;

        explicit ReadItemOk() : ServerPacket(SMSG_READ_ITEM_OK) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class ReadItemFailed final : public ServerPacket
    {
    public:
        ObjectGuid itemGuid;
        uint8 reason = 0;

        explicit ReadItemFailed() : ServerPacket(SMSG_READ_ITEM_FAILED) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class ItemEnchantTimeUpdate final : public ServerPacket
    {
    public:
        ObjectGuid itemGuid;
        uint32 slot = 0;
        uint32 duration = 0;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
        ObjectGuid playerGuid;
#endif

        explicit ItemEnchantTimeUpdate() : ServerPacket(SMSG_ITEM_ENCHANT_TIME_UPDATE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    struct VendorItemEntry
    {
        uint32 slot = 0;
        uint32 itemId = 0;
        uint32 displayInfoId = 0;
        uint32 currentCount = 0;
        uint32 price = 0;
        uint32 maxDurability = 0;
        uint32 buyCount = 0;
    };

    class ListInventoryResponse final : public ServerPacket
    {
    public:
        ObjectGuid vendorGuid;
        std::vector<VendorItemEntry> items;
        uint8 errorCode = 0; // non-zero if vendor has no items

        explicit ListInventoryResponse() : ServerPacket(SMSG_LIST_INVENTORY) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    struct ItemStatEntry
    {
        uint32 type = 0;
        int32 value = 0;
    };

    struct ItemDamageEntry
    {
        float damageMin = 0.0f;
        float damageMax = 0.0f;
        uint32 damageType = 0;
    };

    struct ItemSpellEntry
    {
        uint32 spellId = 0;
        uint32 spellTrigger = 0;
        int32 spellCharges = 0;  // negative means item should be consumed once charges are consumed
        uint32 spellCooldown = 0;
        uint32 spellCategory = 0;
        uint32 spellCategoryCooldown = 0;
    };

    class ItemQuerySingleResponse final : public ServerPacket
    {
    public:
        bool found = false;
        uint32 itemId = 0;
        uint32 itemClass = 0;
        uint32 subClass = 0;                   // client known only 0 subclass for consumables (and 1-2 obsolete subclasses)
        std::string name;                      // max length 256
        uint32 displayInfoId = 0;
        uint32 quality = 0;
        uint32 flags = 0;
        uint32 buyPrice = 0;
        uint32 sellPrice = 0;
        uint32 inventoryType = 0;
        uint32 allowableClass = 0;
        uint32 allowableRace = 0;
        uint32 itemLevel = 0;
        uint32 requiredLevel = 0;
        uint32 requiredSkill = 0;
        uint32 requiredSkillRank = 0;
        uint32 requiredSpell = 0;
        uint32 requiredHonorRank = 0;
        uint32 requiredCityRank = 0;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
        uint32 requiredReputationFaction = 0;
        uint32 requiredReputationRank = 0;     // send value only if reputation faction id set (needed for some items)
#endif
        int32 maxCount = 0;
        int32 stackable = 0;
        uint32 containerSlots = 0;
        ItemStatEntry itemStats[10];
        ItemDamageEntry damages[5];
        // resistances (7)
        uint32 armor = 0;
        uint32 holyRes = 0;
        uint32 fireRes = 0;
        uint32 natureRes = 0;
        uint32 frostRes = 0;
        uint32 shadowRes = 0;
        uint32 arcaneRes = 0;
        uint32 delay = 0;
        uint32 ammoType = 0;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
        float rangedModRange = 0.0f;
#endif
        ItemSpellEntry spells[5];              // cooldowns use DBC data if item_template values not set
        uint32 bonding = 0;
        std::string description;
        uint32 pageText = 0;
        uint32 languageId = 0;
        uint32 pageMaterial = 0;
        uint32 startQuest = 0;
        uint32 lockId = 0;
        int32 material = 0;
        uint32 sheath = 0;
        int32 randomProperty = 0;
        uint32 block = 0;
        uint32 itemSet = 0;
        uint32 maxDurability = 0;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
        uint32 area = 0;
#endif
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
        uint32 map = 0;
#endif
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
        uint32 bagFamily = 0;
#endif

        explicit ItemQuerySingleResponse() : ServerPacket(SMSG_ITEM_QUERY_SINGLE_RESPONSE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

}} // namespace WorldPackets::Item

#endif // MANGOS_PACKETS_ITEM_H
