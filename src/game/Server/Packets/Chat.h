#ifndef MANGOS_PACKETS_CHAT_H
#define MANGOS_PACKETS_CHAT_H

#include "Packet.h"
#include "SharedDefines.h"

namespace WorldPackets { namespace Chat
{
    class ChatMessage final : public ClientPacket
    {
    public:
        uint32 type;
        uint32 lang;
        std::string whisperTargetOrChannel;
        std::string message;

        explicit ChatMessage() : ClientPacket(CMSG_MESSAGECHAT), type(0), lang(0) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
    // --- Server Packets ---

    class ChatWrongFaction final : public ServerPacket
    {
    public:
        explicit ChatWrongFaction() : ServerPacket(SMSG_CHAT_WRONG_FACTION) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class ChatRestricted final : public ServerPacket
    {
    public:
        explicit ChatRestricted() : ServerPacket(SMSG_CHAT_RESTRICTED) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

}} // namespace WorldPackets::Chat

#endif // MANGOS_PACKETS_CHAT_H
