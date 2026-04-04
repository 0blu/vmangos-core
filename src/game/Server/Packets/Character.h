#ifndef MANGOS_PACKETS_CHARACTER_H
#define MANGOS_PACKETS_CHARACTER_H

#include "Packet.h"
#include "ObjectGuid.h"

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
    // --- Server Packets ---

    class CharCreateResponse final : public ServerPacket
    {
    public:
        uint8 result = 0;

        explicit CharCreateResponse() : ServerPacket(SMSG_CHAR_CREATE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class CharDeleteResponse final : public ServerPacket
    {
    public:
        uint8 result = 0;

        explicit CharDeleteResponse() : ServerPacket(SMSG_CHAR_DELETE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class CharacterLoginFailed final : public ServerPacket
    {
    public:
        uint8 result = 0;

        explicit CharacterLoginFailed() : ServerPacket(SMSG_CHARACTER_LOGIN_FAILED) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class CharRenameResponse final : public ServerPacket
    {
    public:
        uint8 result = 0;
        ObjectGuid guid;
        std::string newName;

        explicit CharRenameResponse() : ServerPacket(SMSG_CHAR_RENAME) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class LoginVerifyWorld final : public ServerPacket
    {
    public:
        uint32 mapId = 0;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float o = 0.0f;

        explicit LoginVerifyWorld() : ServerPacket(SMSG_LOGIN_VERIFY_WORLD) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };
}} // namespace WorldPackets::Character

#endif // MANGOS_PACKETS_CHARACTER_H
