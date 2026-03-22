#include "Character.h"
#include "SharedDefines.h"

void WorldPackets::Character::CharCreate::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> name;

    recv_data >> race;
    recv_data >> class_;

    // extract other data required for player creating
    recv_data >> gender;
    recv_data >> skin;
    recv_data >> face;
    recv_data >> hairStyle;
    recv_data >> hairColor;
    recv_data >> facialHair;
    recv_data >> outfitId;
}

void WorldPackets::Character::CharDelete::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Character::PlayerLogin::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
}

void WorldPackets::Character::CharRename::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
    recv_data >> newname;
}

void WorldPackets::Character::CharCreateResult::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << code;
}

void WorldPackets::Character::CharDeleteResult::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << code;
}

void WorldPackets::Character::CharacterLoginFailed::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << code;
}

void WorldPackets::Character::LoginVerifyWorld::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint32(mapId);
    buffer << float(x);
    buffer << float(y);
    buffer << float(z);
    buffer << float(o);
}

void WorldPackets::Character::GuildMotdEvent::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << eventType;
    buffer << count;
    buffer << motd;
}

void WorldPackets::Character::CharRenameResult::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint8(code);
    if (code == RESPONSE_SUCCESS)
    {
        buffer << guid;
        buffer << newName;
    }
}
