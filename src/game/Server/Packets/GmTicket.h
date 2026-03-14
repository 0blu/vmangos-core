#ifndef MANGOS_PACKETS_GMTICKET_H
#define MANGOS_PACKETS_GMTICKET_H

#include "Packet.h"
#include "SharedDefines.h"

namespace WorldPackets { namespace GmTicket
{
    class GmTicketCreate final : public ClientPacket
    {
    public:
        TicketType ticketType;
        uint32 mapId;
        float x, y, z;
        std::string ticketText;
        std::string reservedForFutureUse;

        explicit GmTicketCreate() : ClientPacket(CMSG_GMTICKET_CREATE), ticketType(), mapId(0), x(0), y(0), z(0) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class GmTicketUpdateText final : public ClientPacket
    {
    public:
        uint8 type = 0;
        std::string ticketText;

        explicit GmTicketUpdateText() : ClientPacket(CMSG_GMTICKET_UPDATETEXT) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::GmTicket

#endif // MANGOS_PACKETS_GMTICKET_H
