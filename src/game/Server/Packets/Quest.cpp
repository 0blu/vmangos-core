#include "Quest.h"

void WorldPackets::Quest::QueryQuest::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> questEntry;
}
