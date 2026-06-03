#ifndef MANGOS_PACKETS_QUERY_H
#define MANGOS_PACKETS_QUERY_H

#include "Packet.h"
#include "ObjectGuid.h"

struct CreatureInfo;

namespace WorldPackets { namespace Query
{
    class QueryPlayerName final : public ClientPacket
    {
    public:
        ObjectGuid playerGuid;

        explicit QueryPlayerName() : ClientPacket(CMSG_NAME_QUERY) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QueryCreature final : public ClientPacket
    {
    public:
        uint32 entry = 0;
        ObjectGuid guid;

        explicit QueryCreature() : ClientPacket(CMSG_CREATURE_QUERY) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QueryGameObject final : public ClientPacket
    {
    public:
        uint32 entryID = 0;
        ObjectGuid guid;

        explicit QueryGameObject() : ClientPacket(CMSG_GAMEOBJECT_QUERY) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QueryPageText final : public ClientPacket
    {
    public:
        uint32 pageID = 0;

        explicit QueryPageText() : ClientPacket(CMSG_PAGE_TEXT_QUERY) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class Whois final : public ClientPacket
    {
    public:
        std::string charName;

        explicit Whois() : ClientPacket(CMSG_WHOIS) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class ItemNameQuery final : public ClientPacket
    {
    public:
        uint32 itemId = 0;

        explicit ItemNameQuery() : ClientPacket(CMSG_ITEM_NAME_QUERY) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class QueryTimeResponse final : public ServerPacket
    {
    public:
        uint32 time = 0;

        explicit QueryTimeResponse() : ServerPacket(SMSG_QUERY_TIME_RESPONSE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class NameQueryResponse final : public ServerPacket
    {
    public:
        ObjectGuid playerGuid;
        std::string name;
#if SUPPORTED_CLIENT_BUILD >= CLIENT_BUILD_1_12_1
        std::string realmName; // realm name for cross-realm BG usage
#endif
        uint32 race = 0;
        uint32 gender = 0;
        uint32 class_ = 0;

        explicit NameQueryResponse() : ServerPacket(SMSG_NAME_QUERY_RESPONSE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class PageTextQueryResponse final : public ServerPacket
    {
    public:
        uint32 pageId = 0;
        std::string text;
        uint32 nextPageId = 0;

        explicit PageTextQueryResponse() : ServerPacket(SMSG_PAGE_TEXT_QUERY_RESPONSE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class CreatureQueryResponse final : public ServerPacket
    {
    public:
        int sessionDbLocaleIndex = -1;
        uint32 entry = 0;                 // queried creature entry
        bool notFound = false;            // whether queried entry was not found in templates
        CreatureInfo const* creatureInfo = nullptr;

        CreatureQueryResponse() : ServerPacket(SMSG_CREATURE_QUERY_RESPONSE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    class GameObjectQueryResponse final : public ServerPacket
    {
    public:
        static constexpr uint32 RawDataSize_1_12_1 = 24 * sizeof(int32);
        static constexpr uint32 RawDataSize_Legacy = 16 * sizeof(int32);

        uint32 entryId = 0;               // queried gameobject entry
        bool notFound = false;            // whether queried entry was not found in templates
        uint32 type = 0;
        uint32 displayId = 0;
        std::string name;
#if SUPPORTED_CLIENT_BUILD >= CLIENT_BUILD_1_12_1
        std::string icon;
#endif
        uint8 rawData[RawDataSize_1_12_1] = {};

        GameObjectQueryResponse() : ServerPacket(SMSG_GAMEOBJECT_QUERY_RESPONSE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

    struct NpcTextOption
    {
        float probability = 0.0f;
        std::string maleText;
        std::string femaleText;
        uint32 language = 0;
        uint32 emoteDelay1 = 0;
        uint32 emote1 = 0;
        uint32 emoteDelay2 = 0;
        uint32 emote2 = 0;
        uint32 emoteDelay3 = 0;
        uint32 emote3 = 0;
    };

    class NpcTextUpdate final : public ServerPacket
    {
    public:
        uint32 textId = 0;
        NpcTextOption options[8];

        NpcTextUpdate() : ServerPacket(SMSG_NPC_TEXT_UPDATE) {}
        void AppendBodyTo(ByteBuffer& buffer) const override;
    };

}} // namespace WorldPackets::Query

#endif // MANGOS_PACKETS_QUERY_H
