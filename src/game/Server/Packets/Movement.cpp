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
