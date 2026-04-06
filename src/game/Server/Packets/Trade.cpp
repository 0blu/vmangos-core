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
    buffer << uint8(traderState);
    buffer << uint32(slotCount);
    buffer << uint32(slotCount);
    buffer << uint32(money);
    buffer << uint32(spell);

    for (uint32 i = 0; i < slotCount; ++i)
    {
        buffer << uint8(i);

        if (i < items.size() && items[i].itemId != 0)
        {
            auto const& item = items[i];
            buffer << uint32(item.itemId);
            buffer << uint32(item.displayInfoId);
            buffer << uint32(item.stackCount);
            buffer << uint32(item.isWrapped);
            buffer << item.giftCreator;
            buffer << uint32(item.enchantmentId);
            buffer << item.creator;
            buffer << uint32(item.spellCharges);
            buffer << uint32(item.suffixFactor);
            buffer << uint32(item.randomPropertyId);
            buffer << uint32(item.lockId);
            buffer << uint32(item.maxDurability);
            buffer << uint32(item.durability);
        }
        else
        {
            for (uint8 j = 0; j < 15; ++j)
                buffer << uint32(0);
        }
    }
}
