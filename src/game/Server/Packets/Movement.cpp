#include "Movement.h"

void WorldPackets::Movement::MovementPacket::ReadFromWorldPacket(WorldPacket& recv_data)
{
    opcode = recv_data.GetOpcode();
    recv_data >> movementInfo;
}

void WorldPackets::Movement::MoveTimeSkipped::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
    recv_data >> lag;
}

void WorldPackets::Movement::MoveTeleportAck::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> guid;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
    recv_data >> movementCounter;
#endif
    recv_data >> time;
}
