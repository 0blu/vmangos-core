#include "Query.h"

void WorldPackets::Query::QueryPlayerName::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> playerGuid;
}

void WorldPackets::Query::QueryCreature::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> entry;
    recv_data >> guid;
}

void WorldPackets::Query::QueryGameObject::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> entryID;
    recv_data >> guid;
}

void WorldPackets::Query::QueryPageText::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> pageID;
    if (recv_data.rpos() < recv_data.wpos())
        recv_data >> gameObject; // optional, when queried via game object
}

void WorldPackets::Query::Whois::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> charName;
}

void WorldPackets::Query::ItemNameQuery::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> itemId;
    recv_data.read_skip<uint64>(); // guid, not used
}

void WorldPackets::Query::QueryTimeResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint32(time);
}
