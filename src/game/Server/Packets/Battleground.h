#ifndef MANGOS_PACKETS_BATTLEGROUND_H
#define MANGOS_PACKETS_BATTLEGROUND_H

#include "Packet.h"
#include "ObjectGuid.h"

namespace WorldPackets { namespace Battleground
{
    class BattlefieldListRequest final : public ClientPacket
    {
    public:
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
        uint32 mapId;
#endif

        explicit BattlefieldListRequest() : ClientPacket(CMSG_BATTLEFIELD_LIST)
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
        , mapId(0)
#endif
        {}

        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class AreaSpiritHealerQuery final : public ClientPacket
    {
    public:
        ObjectGuid guid;

        explicit AreaSpiritHealerQuery() : ClientPacket(CMSG_AREA_SPIRIT_HEALER_QUERY) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class AreaSpiritHealerQueue final : public ClientPacket
    {
    public:
        ObjectGuid guid;

        explicit AreaSpiritHealerQueue() : ClientPacket(CMSG_AREA_SPIRIT_HEALER_QUEUE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class BattlemasterHello final : public ClientPacket
    {
    public:
        ObjectGuid guid;

        explicit BattlemasterHello() : ClientPacket(CMSG_BATTLEMASTER_HELLO) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class BattleFieldPort final : public ClientPacket
    {
    public:
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
        uint32 mapId = 0;
#endif
        uint8 action = 0;

        explicit BattleFieldPort() : ClientPacket(CMSG_BATTLEFIELD_PORT) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class LeaveBattlefield final : public ClientPacket
    {
    public:
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
        uint32 mapId = 0;
#endif

        explicit LeaveBattlefield() : ClientPacket(CMSG_LEAVE_BATTLEFIELD) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class BattlemasterJoin final : public ClientPacket
    {
    public:
        ObjectGuid guid;
        uint32     mapId = 0;
        uint32     instanceId = 0;
        uint8      joinAsGroup = 0;

        explicit BattlemasterJoin() : ClientPacket(CMSG_BATTLEMASTER_JOIN) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class BattlefieldJoin final : public ClientPacket
    {
    public:
        uint32 mapId = 0;

        explicit BattlefieldJoin() : ClientPacket(CMSG_BATTLEFIELD_JOIN) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Battleground

#endif // MANGOS_PACKETS_BATTLEGROUND_H
