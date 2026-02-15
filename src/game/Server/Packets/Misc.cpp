#include "Misc.h"

void WorldPackets::Misc::WorldTeleport::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> timeMs;
    recv_data >> location.mapId;
    recv_data >> location.x;
    recv_data >> location.y;
    recv_data >> location.z;
    recv_data >> location.o;
}
