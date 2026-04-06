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
    buffer << notifyDelay;
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
        buffer << mail.messageId;
        buffer << mail.messageType;

        if (mail.hasPlayerSender)
            buffer << mail.senderPlayerGuid;
        else if (mail.hasSenderEntry)
            buffer << mail.senderEntry;

        buffer << mail.subject;
        buffer << mail.itemTextId;
        buffer << uint32(0); // package (Package.dbc)
        buffer << mail.stationery;

        if (mail.itemInfo.hasItem)
        {
            buffer << mail.itemInfo.entry;
            buffer << mail.itemInfo.enchantmentId;
            buffer << mail.itemInfo.randomPropertyId;
            buffer << mail.itemInfo.suffixFactor;
            buffer << mail.itemInfo.stackCount;
            buffer << mail.itemInfo.spellCharges;
            buffer << mail.itemInfo.maxDurability;
            buffer << mail.itemInfo.durability;
        }
        else
        {
            buffer << uint32(0) << uint32(0) << uint32(0) << uint32(0) << uint8(0) << uint32(0) << uint32(0) << uint32(0);
        }

        buffer << mail.money;
        buffer << mail.COD;
        buffer << mail.checked;
        buffer << mail.expireTime;

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
        buffer << mail.mailTemplateId;
#endif
    }
}
