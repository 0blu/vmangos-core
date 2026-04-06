#include "Mail.h"

void WorldPackets::Mail::SendMail::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> mailboxGuid;
    recv_data >> receiverName;
    recv_data >> subject;
    recv_data >> body;
    recv_data >> stationeryId;
    recv_data >> packageId;
    recv_data >> itemGuid;
    recv_data >> money;
    recv_data >> COD;

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
    recv_data.read_skip<uint64>(); // const 0
    recv_data.read_skip<uint8>(); // const 0
#endif

}


void WorldPackets::Mail::MailReturnToSender::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> mailboxGuid;
    recv_data >> mailId;
}

void WorldPackets::Mail::MailMarkAsRead::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> mailboxGuid;
    recv_data >> mailId;
}

void WorldPackets::Mail::MailTakeItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> mailboxGuid;
    recv_data >> mailId;
}

void WorldPackets::Mail::MailTakeMoney::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> mailboxGuid;
    recv_data >> mailId;
}

void WorldPackets::Mail::GetMailList::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> mailboxGuid;
}

void WorldPackets::Mail::MailDelete::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> mailboxGuid;
    recv_data >> mailId;
}

void WorldPackets::Mail::MailCreateTextItem::ReadFromWorldPacket(WorldPacket& recv_data)
{
    recv_data >> mailboxGuid;
    recv_data >> mailId;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
    recv_data >> mailTemplateId;
#endif
}

// --- Server Packets ---

void WorldPackets::Mail::ReceivedMail::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint32(notifyDelay);
}

void WorldPackets::Mail::SendMailResult::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << mailId;
    buffer << mailAction;
    buffer << mailError;
    if (mailError == MAIL_ERR_EQUIP_ERROR)
    {
        buffer << equipError;
    }
    else if (mailAction == MAIL_ITEM_TAKEN)
    {
        buffer << itemGuid;  // item guid low?
        buffer << itemCount; // item count?
    }
}

void WorldPackets::Mail::ItemTextQueryResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << itemTextId;
    buffer << text;
}

void WorldPackets::Mail::QueryNextMailTimeResponse::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << nextMailTime;
}

void WorldPackets::Mail::MailListResult::AppendBodyTo(ByteBuffer& buffer) const
{
    buffer << uint8(mails.size());
    for (auto const& mail : mails)
    {
        buffer << uint32(mail.messageId);
        buffer << uint8(mail.messageType);

        if (mail.hasPlayerSender)
            buffer << mail.senderPlayerGuid;
        else if (mail.hasSenderEntry)
            buffer << uint32(mail.senderEntry);

        buffer << mail.subject;
        buffer << uint32(mail.itemTextId);
        buffer << uint32(0); // package (Package.dbc)
        buffer << uint32(mail.stationery);

        if (mail.itemInfo.hasItem)
        {
            buffer << uint32(mail.itemInfo.entry);
            buffer << uint32(mail.itemInfo.enchantmentId);
            buffer << uint32(mail.itemInfo.randomPropertyId);
            buffer << uint32(mail.itemInfo.suffixFactor);
            buffer << uint8(mail.itemInfo.stackCount);
            buffer << uint32(mail.itemInfo.spellCharges);
            buffer << uint32(mail.itemInfo.maxDurability);
            buffer << uint32(mail.itemInfo.durability);
        }
        else
        {
            buffer << uint32(0) << uint32(0) << uint32(0) << uint32(0) << uint8(0) << uint32(0) << uint32(0) << uint32(0);
        }

        buffer << uint32(mail.money);
        buffer << uint32(mail.COD);
        buffer << uint32(mail.checked);
        buffer << float(mail.expireTime);

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
        buffer << uint32(mail.mailTemplateId);
#endif
    }
}
