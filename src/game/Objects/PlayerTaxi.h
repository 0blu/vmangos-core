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

#ifndef _PLAYER_TAXI_H
#define _PLAYER_TAXI_H

#include "Common.h"
#include "DBCStructure.h"
#include "SharedDefines.h"
#include "TaxiMask.h"

#include <deque>

class PlayerTaxi
{
public:
    void InitTaxiNodesForLevel(uint32 raceId, uint32 classId, uint32 level);

    // return false on parse error
    [[nodiscard]]
    bool ResetKnownNodesFromSerializedString(std::string const& serializedString);
    std::string GetKnownAsSerializedString() const;

    bool IsKnownTaxiNode(uint32 nodeEntry) const
    {
        return m_knownNodes.HasEntry(nodeEntry);
    }
    TaxiMask GetKnownTaxiNodes(bool isTaxiCheater) const;

    // returns true if it was updated
    bool LearnTaxiNode(uint32 nodeEntry)
    {
        if (m_knownNodes.HasEntry(nodeEntry))
            return false;

        m_knownNodes.SetEntry(nodeEntry, true);
        return true;
    }

    // Destinations
    bool LoadTaxiDestinationsFromString(std::string const& values, Team team);
    std::string SaveTaxiDestinationsToString() const;

    void ClearTaxiDestinations()
    {
        m_TaxiDestinations.clear();
        m_taxiPath.clear();
        m_discount = 1.0f;
    }
    void AddTaxiDestination(uint32 dest) { m_TaxiDestinations.push_back(dest); }
    void SetDiscount(float discount) { m_discount = discount; }
    uint32 GetTaxiSource() const { return m_TaxiDestinations.empty() ? 0 : m_TaxiDestinations.front(); }
    uint32 GetTaxiDestination() const { return m_TaxiDestinations.size() < 2 ? 0 : m_TaxiDestinations[1]; }
    uint32 GetCurrentTaxiPath() const;
    uint32 GetCurrentTaxiCost() const;
    uint32 NextTaxiDestination()
    {
        m_TaxiDestinations.pop_front();
        return GetTaxiDestination();
    }

    TaxiPathNodeList const& GetTaxiPath() const { return m_taxiPath; };
    void AddTaxiPathNode(TaxiPathNodeEntry const& entry)
    {
        m_taxiPath.resize(m_taxiPath.size() + 1);
        m_taxiPath.set(m_taxiPath.size() - 1, &entry);
    }
    bool empty() const { return m_TaxiDestinations.empty(); }

private:
    float m_discount = 0.0f;
    TaxiMask m_knownNodes;
    std::deque<uint32> m_TaxiDestinations;
    TaxiPathNodeList m_taxiPath;
};

#endif
