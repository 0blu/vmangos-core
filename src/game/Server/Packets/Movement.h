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
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
        uint32     movementCounter = 0;
#endif
        uint32     time = 0;

        MoveTeleportAck() : ClientPacket(MSG_MOVE_TELEPORT_ACK) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    // Used for CMSG_FORCE_RUN_SPEED_CHANGE_ACK, CMSG_FORCE_RUN_BACK_SPEED_CHANGE_ACK,
    // CMSG_FORCE_SWIM_SPEED_CHANGE_ACK, CMSG_FORCE_WALK_SPEED_CHANGE_ACK,
    // CMSG_FORCE_SWIM_BACK_SPEED_CHANGE_ACK, CMSG_FORCE_TURN_RATE_CHANGE_ACK
    class MoveSpeedAck final : public ClientPacket
    {
    public:
        ObjectGuid   guid;
        uint32       movementCounter = 0;
        MovementInfo movementInfo;
        float        speed = 0.0f;
        uint32       packetTime = 0;

        MoveSpeedAck() : ClientPacket(OPCODE_WILL_BE_SET_IN_READ_FUNCTION) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    // Used for CMSG_MOVE_HOVER_ACK, CMSG_MOVE_FEATHER_FALL_ACK, CMSG_MOVE_WATER_WALK_ACK
    class MoveFlagChangeAck final : public ClientPacket
    {
    public:
        ObjectGuid   guid;
        uint32       movementCounter = 0;
        MovementInfo movementInfo;
        bool         apply = false;
        uint32       packetTime = 0;

        MoveFlagChangeAck() : ClientPacket(OPCODE_WILL_BE_SET_IN_READ_FUNCTION) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    // Used for CMSG_FORCE_MOVE_ROOT_ACK, CMSG_FORCE_MOVE_UNROOT_ACK
    class MoveRootAck final : public ClientPacket
    {
    public:
        ObjectGuid   guid;
        uint32       movementCounter = 0;
        MovementInfo movementInfo;
        uint32       packetTime = 0;

        MoveRootAck() : ClientPacket(OPCODE_WILL_BE_SET_IN_READ_FUNCTION) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Movement

#endif // MANGOS_PACKETS_MOVEMENT_H
