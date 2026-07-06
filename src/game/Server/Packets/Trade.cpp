#include "Trade.h"

#include "SharedDefines.h"

void WorldPackets::Trade::InitiateTrade::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> tradeTargetGuid;
}

void WorldPackets::Trade::SetTradeGold::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> gold;
}

void WorldPackets::Trade::SetTradeItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> tradeSlot;
    recv_data >> bag;
    recv_data >> slot;
}

void WorldPackets::Trade::ClearTradeItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> tradeSlot;
}

void WorldPackets::Trade::AcceptTrade::ReadFromWorldPacket(WorldPacket& recv_data)
{
    // some unused variable with is set to 1 when the player got TRADE_STATUS_OPEN_WINDOW at least once in this session
    recv_data.read_skip<uint32>();
}

void WorldPackets::Trade::TradeStatus::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << status;
    switch (status)
    {
        case TRADE_STATUS_BEGIN_TRADE:
            buffer << playerGuid;
            break;
        case TRADE_STATUS_CLOSE_WINDOW:
            buffer << closeResult;
            buffer << closeUnk;
            buffer << closeItemLimitCategory;
            break;
        case TRADE_STATUS_ONLY_CONJURED:
            buffer << slot;
            break;
        default:
            break;
    }
}

void WorldPackets::Trade::TradeStatusExtended::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << traderState;
    buffer << tradeSlotCount;
    buffer << displayedTradeSlotCount;
    buffer << traderMoney;
    buffer << spellId;

    for (uint8 slotIndex = 0; slotIndex < items.size(); ++slotIndex)
    {
        buffer << slotIndex;

        TradeStatusExtendedItem const& item = items[slotIndex];
        if (!item.itemTemplate)
        {
            for (uint8 zeroField = 0; zeroField < 15; ++zeroField)
                buffer << static_cast<uint32>(0); // since we have no item, we need to fill the space with something
            continue;
        }

        buffer << item.itemTemplate->ItemId;
        buffer << item.itemTemplate->DisplayInfoID;
        buffer << item.stackCount;
        buffer << item.isWrapped;
        buffer << item.giftCreatorGuid;
        buffer << item.enchantmentId;
        buffer << item.creatorGuid;
        buffer << item.spellCharges;
        buffer << item.itemSuffixFactor;
        buffer << item.itemRandomPropertyId;
        buffer << item.itemTemplate->LockID;
        buffer << item.maxDurability;
        buffer << item.durability;
    }
}

