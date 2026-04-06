#include "Trade.h"

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
    buffer << playerGuid;
}

void WorldPackets::Trade::TradeStatusExtended::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << traderState;
    buffer << tradeSlotCount;
    buffer << slotCount;
    buffer << money;
    buffer << spell;

    for (uint32 i = 0; i < slotCount; ++i)
    {
        buffer << uint8(i);

        if (i < items.size() && items[i].itemId != 0)
        {
            auto const& item = items[i];
            buffer << item.itemId;
            buffer << item.displayInfoId;
            buffer << item.stackCount;
            buffer << item.isWrapped;
            buffer << item.giftCreator;
            buffer << item.enchantmentId;
            buffer << item.creator;
            buffer << item.spellCharges;
            buffer << item.suffixFactor;
            buffer << item.randomPropertyId;
            buffer << item.lockId;
            buffer << item.maxDurability;
            buffer << item.durability;
        }
        else
        {
            for (uint8 j = 0; j < 15; ++j)
                buffer << uint32(0);
        }
    }
}
