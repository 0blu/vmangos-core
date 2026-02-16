#ifndef MANGOS_PACKETS_CHARACTER_H
#define MANGOS_PACKETS_CHARACTER_H

#include "Packet.h"
#include "ObjectGuid.h"

namespace WorldPackets { namespace Character
{
    class CharCreate final : public ClientPacket
    {
    public:
        std::string name;
        uint8 race, class_;
        uint8 gender, skin, face, hairStyle, hairColor, facialHair, outfitId;

        void ReadFromWorldPacket(WorldPacket& recv_data) override;
    };
}} // namespace WorldPackets::Character

#endif // MANGOS_PACKETS_CHARACTER_H
