#include "Channel.h"

void WorldPackets::Channel::JoinChannel::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> channelName;
    recv_data >> channelPassword;
}
