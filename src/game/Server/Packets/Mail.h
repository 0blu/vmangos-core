#ifndef MANGOS_PACKETS_MAIL_H
#define MANGOS_PACKETS_MAIL_H

#include "Packet.h"
#include "SharedDefines.h"
#include "ObjectGuid.h"

namespace WorldPackets { namespace Mail
{
    class SendMail final : public ClientPacket
    {
    public:
        ObjectGuid  mailboxGuid;
        std::string receiverName;
        std::string subject;
        std::string body;
        uint32      stationeryId;
        uint32      packageId;
        ObjectGuid  itemGuid;
        uint32      money;
        uint32      COD; // Cash On Delivery

        explicit SendMail() : ClientPacket(CMSG_SEND_MAIL), stationeryId(0), packageId(0), money(0), COD(0) {}

        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };

    class MailReturnToSender final : public ClientPacket
    {
    public:
        ObjectGuid mailboxGuid;
        uint32 mailId = 0;

        explicit MailReturnToSender() : ClientPacket(CMSG_MAIL_RETURN_TO_SENDER) {}
        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Mail

#endif // MANGOS_PACKETS_MAIL_H
