#ifndef MANGOS_PACKETS_MOVEMENT_H
#define MANGOS_PACKETS_MOVEMENT_H

#include "Packet.h"
#include "MovementInfo.h"

namespace WorldPackets { namespace Movement
{
    class MovementPacket final : public ClientPacket
    {
    public:
        MovementInfo movementInfo;

        MovementPacket() : ClientPacket(OPCODE_WILL_BE_SET_IN_READ_FUNCTION) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class MoveTimeSkipped final : public ClientPacket
    {
    public:
        ObjectGuid guid;
        uint32     lag = 0;

        MoveTimeSkipped() : ClientPacket(CMSG_MOVE_TIME_SKIPPED) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class MoveTeleportAck final : public ClientPacket
    {
    public:
        ObjectGuid guid;
        uint32     movementCounter = 0;
        uint32     time = 0;

        MoveTeleportAck() : ClientPacket(MSG_MOVE_TELEPORT_ACK) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Movement

#endif // MANGOS_PACKETS_MOVEMENT_H
