#include "Skill.h"

void WorldPackets::Skill::LearnTalent::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> talent_id;
    recv_data >> requested_rank;
}
