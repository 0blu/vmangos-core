#ifndef MANGOS_PACKETS_TRADE_H
#define MANGOS_PACKETS_TRADE_H

#include "Packet.h"
#include "ObjectGuid.h"

namespace WorldPackets { namespace Trade
{
    class InitiateTrade final : public ClientPacket
    {
    public:
        ObjectGuid tradeTargetGuid;

        explicit InitiateTrade() : ClientPacket(CMSG_INITIATE_TRADE) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Trade

#endif // MANGOS_PACKETS_TRADE_H
