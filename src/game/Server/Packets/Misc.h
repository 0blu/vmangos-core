#ifndef MANGOS_PACKETS_MISC_H
#define MANGOS_PACKETS_MISC_H

#include "Packet.h"
#include "ObjectGuid.h"
#include "SharedDefines.h"

namespace WorldPackets { namespace Misc
{
    class WorldTeleport final : public ClientPacket
    {
    public:
        uint32 timeMs;
        WorldLocation location;

        explicit WorldTeleport() : ClientPacket(CMSG_WORLD_TELEPORT), timeMs(0) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class AddFriend final : public ClientPacket
    {
    public:
        std::string friendName;

        explicit AddFriend() : ClientPacket(CMSG_ADD_FRIEND) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class DelFriend final : public ClientPacket
    {
    public:
        ObjectGuid friendGuid;

        explicit DelFriend() : ClientPacket(CMSG_DEL_FRIEND) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class AddIgnore final : public ClientPacket
    {
    public:
        std::string ignoreName;

        explicit AddIgnore() : ClientPacket(CMSG_ADD_IGNORE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class DelIgnore final : public ClientPacket
    {
    public:
        ObjectGuid ignoreGuid;

        explicit DelIgnore() : ClientPacket(CMSG_DEL_IGNORE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Misc

#endif // MANGOS_PACKETS_MISC_H
