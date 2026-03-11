#include "Loot.h"

void WorldPackets::Loot::AutoStoreLootItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> lootSlot;
}

void WorldPackets::Loot::LootUnit::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}
