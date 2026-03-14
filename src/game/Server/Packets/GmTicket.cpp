#include "GmTicket.h"

void WorldPackets::GmTicket::GmTicketCreate::ReadFromWorldPacket(WorldPacket& recv_data)
{
    uint8 type;
    recv_data >> type;
    ticketType = static_cast<TicketType>(type);

    recv_data >> mapId;
    recv_data >> x;
    recv_data >> y;
    recv_data >> z;
    recv_data >> ticketText;
    recv_data >> reservedForFutureUse;
}

void WorldPackets::GmTicket::GmTicketUpdateText::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> type;
    recv_data >> ticketText;
}
