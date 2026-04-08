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

void WorldPackets::Character::CharCreateResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint8(result);
}

void WorldPackets::Character::CharDeleteResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint8(result);
}

void WorldPackets::Character::CharacterLoginFailed::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint8(result);
}

void WorldPackets::Character::CharRenameResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint8(result);
    if (result == RESPONSE_SUCCESS)
    {
        buffer << guid;
        buffer << newName;
    }
}

void WorldPackets::Character::LoginVerifyWorld::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << location.mapId;
    buffer << location.x;
    buffer << location.y;
    buffer << location.z;
    buffer << location.o;
}

void WorldPackets::Character::CharEnum::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint8(characters.size());
    for (auto const& c : characters)
    {
        buffer << c.guid;
        buffer << c.name;
        buffer << c.race;
        buffer << c.class_;
        buffer << c.gender;
        buffer << c.skin;
        buffer << c.face;
        buffer << c.hairStyle;
        buffer << c.hairColor;
        buffer << c.facialHair;
        buffer << c.level;
        buffer << c.zone;
        buffer << c.map;
        buffer << c.x;
        buffer << c.y;
        buffer << c.z;
        buffer << c.guildId;
        buffer << c.charFlags;
        buffer << c.firstLogin;
        buffer << c.petDisplayId;
        buffer << c.petLevel;
        buffer << c.petFamily;
        for (auto const& slot : c.equipment)
        {
            buffer << slot.displayInfoId;
            buffer << slot.inventoryType;
        }
    }
}
