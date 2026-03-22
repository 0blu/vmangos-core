#ifndef MANGOS_PACKETS_CHARACTER_H
#define MANGOS_PACKETS_CHARACTER_H

#include "Packet.h"
#include "ObjectGuid.h"
#include "SharedDefines.h"

namespace WorldPackets { namespace Character
{
    class CharCreate final : public ClientPacket
    {
    public:
        std::string name;
        uint8 race, class_;
        uint8 gender, skin, face, hairStyle, hairColor, facialHair, outfitId;

        explicit CharCreate() : ClientPacket(CMSG_CHAR_CREATE),
        race(0), class_(0), gender(0), skin(0), face(0), hairStyle(0), hairColor(0), facialHair(0), outfitId(0) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class CharDelete final : public ClientPacket
    {
    public:
        ObjectGuid guid;

        explicit CharDelete() : ClientPacket(CMSG_CHAR_DELETE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class PlayerLogin final : public ClientPacket
    {
    public:
        ObjectGuid guid;

        explicit PlayerLogin() : ClientPacket(CMSG_PLAYER_LOGIN) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class CharRename final : public ClientPacket
    {
    public:
        ObjectGuid  guid;
        std::string newname;

        explicit CharRename() : ClientPacket(CMSG_CHAR_RENAME) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class CharCreateResult final : public ServerPacket
    {
    public:
        uint8 code = 0;

        explicit CharCreateResult() : ServerPacket(SMSG_CHAR_CREATE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class CharDeleteResult final : public ServerPacket
    {
    public:
        uint8 code = 0;

        explicit CharDeleteResult() : ServerPacket(SMSG_CHAR_DELETE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class CharacterLoginFailed final : public ServerPacket
    {
    public:
        uint8 code = 0;

        explicit CharacterLoginFailed() : ServerPacket(SMSG_CHARACTER_LOGIN_FAILED) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class LoginVerifyWorld final : public ServerPacket
    {
    public:
        uint32 mapId = 0;
        Position position;

        explicit LoginVerifyWorld() : ServerPacket(SMSG_LOGIN_VERIFY_WORLD) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class GuildMotdEvent final : public ServerPacket
    {
    public:
        uint8 eventType = 0;
        uint8 count = 0;
        std::string motd;

        explicit GuildMotdEvent() : ServerPacket(SMSG_GUILD_EVENT) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class CharRenameResult final : public ServerPacket
    {
    public:
        uint8 code = 0;
        ObjectGuid guid;
        std::string newName;

        explicit CharRenameResult() : ServerPacket(SMSG_CHAR_RENAME) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };
}} // namespace WorldPackets::Character

#endif // MANGOS_PACKETS_CHARACTER_H
