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
    buffer << uint32(0); // unknown
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
    buffer.append(rawData, 24); // these are read as int32
#else
    buffer.append(rawData, 16); // these are read as int32
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
        buffer << option.emoteDelay0;
        buffer << option.emote0;
        buffer << option.emoteDelay1;
        buffer << option.emote1;
        buffer << option.emoteDelay2;
        buffer << option.emote2;
    }
}

