#ifndef MANGOS_PACKET_H
#define MANGOS_PACKET_H
#include <Errors.h>

#include "WorldPacket.h"

class Packet
{
public:
    virtual ~Packet() = default;
};

class ClientPacket : public Packet
{
public:
    /** Updates the fields of this object instance by a given WorldPacket */
    virtual void ReadFromWorldPacket(WorldPacket& recv_data) = 0;
};

/** Indicate an empty packet from the client */
class NullClientPacket final : public ClientPacket
{
public:
    void ReadFromWorldPacket(WorldPacket& recv_data) override {}
};

#endif // MANGOS_PACKET_H
