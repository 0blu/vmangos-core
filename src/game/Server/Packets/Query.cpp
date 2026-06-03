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
    if (recv_data.rpos() < recv_data.size())
    { // optional, see 5875 at 0x0056485D
        recv_data.read_skip<uint64>(); // object guid, not used
    }
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
    buffer << time;
}

void WorldPackets::Query::NameQueryResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << playerGuid;
    buffer << name;
#if SUPPORTED_CLIENT_BUILD >= CLIENT_BUILD_1_12_1
    buffer << realmName;
#endif
    buffer << race;
    buffer << gender;
    buffer << class_;
}

void WorldPackets::Query::PageTextQueryResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << pageId;
    buffer << text;
    buffer << nextPageId;
}

void WorldPackets::Query::CreatureQueryResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    if (notFound)
    {
        buffer << uint32(entry | 0x80000000);
        return;
    }

    buffer << entry;
    buffer << name;
    buffer << uint8(0) << uint8(0) << uint8(0); // name2, name3, name4
    buffer << subName;
    buffer << typeFlags;
    buffer << type;
    buffer << petFamily;
    buffer << rank;
    // This field is reserved in the classic creature query response payload and clients expect it as 0.
    // Keeping it explicit preserves packet alignment with the legacy opcode layout.
    buffer << uint32(0);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_7_1
    buffer << petSpellListId;
#endif
    buffer << displayId;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_4_2
    buffer << civilian;
#endif
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
    buffer << racialLeader;
#endif
}

void WorldPackets::Query::GameObjectQueryResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    if (notFound)
    {
        buffer << uint32(entryId | 0x80000000);
        return;
    }

    buffer << entryId;
    buffer << type;
    buffer << displayId;
    buffer << name;
    buffer << uint8(0) << uint8(0) << uint8(0); // name2, name3, name4
#if SUPPORTED_CLIENT_BUILD >= CLIENT_BUILD_1_12_1
    buffer << icon;
    // GameObjectInfo stores this as raw bytes; the client interprets the same 24-byte segment as int32 fields.
    buffer.append(rawData, RawDataSize_1_12_1);
#else
    // Legacy clients consume the same blob format but only the first 16 bytes.
    buffer.append(rawData, RawDataSize_Legacy);
#endif
}

void WorldPackets::Query::NpcTextUpdate::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << textId;
    for (auto const& option : options)
    {
        buffer << option.probability;
        buffer << option.maleText;
        buffer << option.femaleText;
        buffer << option.language;
        buffer << option.emoteDelay1;
        buffer << option.emote1;
        buffer << option.emoteDelay2;
        buffer << option.emote2;
        buffer << option.emoteDelay3;
        buffer << option.emote3;
    }
}
