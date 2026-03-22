/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "PlayerTaxi.h"

#include <Errors.h>

#include "ByteBuffer.h"
#include "ObjectMgr.h"
#include "Util.h"

void PlayerTaxi::InitTaxiNodesForLevel(uint32 raceId, uint32 classId, uint32 level)
{
    // capital and taxi hub masks
    ChrRacesEntry const* rEntry = sChrRacesStore.LookupEntry(raceId);
    // Hmm, we can only set the first 32 entries this way. TrinityCore uses switch/case for each race without DB.
    std::string firstSerializedMaskPart = std::to_string(rEntry->startingTaxiMask);
    MANGOS_ASSERT(ResetKnownNodesFromSerializedString(firstSerializedMaskPart + " 0 0 0 0 0 0 0"));
}

bool PlayerTaxi::ResetKnownNodesFromSerializedString(std::string const& serializedString)
{
    auto maybeMask = TaxiMask::FromSerializedString(serializedString);
    if (!maybeMask.has_value())
        return false;

    m_knownNodes = maybeMask.value();
    return true;
}

std::string PlayerTaxi::GetKnownAsSerializedString() const
{
    return m_knownNodes.ToSerializedString();
}

TaxiMask PlayerTaxi::GetKnownTaxiNodes(bool isTaxiCheater) const
{
    return isTaxiCheater ? sTaxiAllValidTaxiNetworkNodes : m_knownNodes;
}

bool PlayerTaxi::LoadTaxiDestinationsFromString(std::string const& values, Team team)
{
    ClearTaxiDestinations();

    Tokens tokens = StrSplit(values, " ");

    for (Tokens::iterator iter = tokens.begin(); iter != tokens.end(); ++iter)
    {
        uint32 node = uint32(atol(iter->c_str()));
        AddTaxiDestination(node);
    }

    if (m_TaxiDestinations.empty())
        return true;

    // Check integrity
    if (m_TaxiDestinations.size() < 2)
        return false;

    for (size_t i = 1; i < m_TaxiDestinations.size(); ++i)
    {
        uint32 cost;
        uint32 path;
        sObjectMgr.GetTaxiPath(m_TaxiDestinations[i - 1], m_TaxiDestinations[i], path, cost);
        if (!path)
            return false;
    }

    // can't load taxi path without mount set (quest taxi path?)
    return sObjectMgr.GetTaxiMountDisplayId(GetTaxiSource(), team, true) != 0;
}

std::string PlayerTaxi::SaveTaxiDestinationsToString() const
{
    if (m_TaxiDestinations.size() < 2)
        return "";

    std::ostringstream ss;

    // save only the current path
    for (size_t i = 0; i < 2; ++i)
        ss << m_TaxiDestinations[i] << " ";

    return ss.str();
}

uint32 PlayerTaxi::GetCurrentTaxiPath() const
{
    if (m_TaxiDestinations.size() < 2)
        return 0;

    uint32 path;
    uint32 cost;

    sObjectMgr.GetTaxiPath(m_TaxiDestinations[0], m_TaxiDestinations[1], path, cost);

    return path;
}

uint32 PlayerTaxi::GetCurrentTaxiCost() const
{
    if (m_TaxiDestinations.size() < 2)
        return 0;

    uint32 path;
    uint32 cost;

    sObjectMgr.GetTaxiPath(m_TaxiDestinations[0], m_TaxiDestinations[1], path, cost);

    return uint32(cost * m_discount + 0.5f);
}
