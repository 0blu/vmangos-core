/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 * Copyright (C) 2009-2011 MaNGOSZero <https://github.com/mangos/zero>
 * Copyright (C) 2011-2016 Nostalrius <https://nostalrius.org>
 * Copyright (C) 2016-2017 Elysium Project <https://github.com/elysium-project>
 *
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

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Path.h"
#include "WaypointMovementGenerator.h"

void WorldSession::HandleTaxiNodeStatusQueryOpcode(WorldPackets::Taxi::TaxiNodeStatusQuery const& packet)
{
    SendTaxiStatus(packet.creatureGuidNearTaxi);
}

void WorldSession::SendTaxiStatus(ObjectGuid guid)
{
    // cheating checks
    Creature* unit = GetPlayer()->GetMap()->GetCreature(guid);
    if (!unit)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WorldSession::SendTaxiStatus - %s not found or you can't interact with it.", guid.GetString().c_str());
        return;
    }

    uint32 nodeEntry = sObjectMgr.GetNearestTaxiNode(unit->GetWorldLocation(), GetPlayer()->GetTeam());

    // not found nearest
    if (nodeEntry == 0)
        return;

    auto taxiStatus = std::make_unique<WorldPackets::Taxi::TaxiNodeStatus>();
    taxiStatus->flightmasterGuid = guid;
    taxiStatus->isKnown = GetPlayer()->m_taxi.IsKnownTaxiNode(nodeEntry);
    SendPacket(std::move(taxiStatus));
}

void WorldSession::HandleTaxiQueryAvailableNodes(WorldPackets::Taxi::TaxiQueryAvailableNodes const& packet)
{
    // cheating checks
    Creature const* flightmaster = GetPlayer()->GetNPCIfCanInteractWith(packet.guid, UNIT_NPC_FLAG_FLIGHTMASTER);
    if (!flightmaster)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: HandleTaxiQueryAvailableNodes - %s not found or you can't interact with him.", packet.guid.GetString().c_str());
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_FEIGN_DEATH))
        GetPlayer()->RemoveSpellsCausingAura(SPELL_AURA_FEIGN_DEATH);

    if (SendLearnNewTaxiNode(flightmaster) == LearnTaxiNodeResult::KnownAlready)
        SendTaxiMenu(flightmaster); // Only send TaxiMenu if known already
}

void WorldSession::SendTaxiMenu(Creature const* flightmaster)
{
    // find current node
    uint32 taxiNode = sObjectMgr.GetNearestTaxiNode(flightmaster->GetWorldLocation(), GetPlayer()->GetTeam());

    if (taxiNode == 0)
        return;

    auto showTaxi = std::make_unique<WorldPackets::Taxi::ShowTaxiNodes>();
    showTaxi->flightmasterGuid = flightmaster->GetObjectGuid();
    showTaxi->currentNode = taxiNode;
    showTaxi->knownNodes = GetPlayer()->m_taxi.GetKnownTaxiNodes(GetPlayer()->IsTaxiCheater()).GetEntryList();
    SendPacket(std::move(showTaxi));
}

void WorldSession::SendDoFlight(uint32 mountDisplayId, uint32 path, uint32 pathNode)
{
    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_FEIGN_DEATH))
        GetPlayer()->RemoveSpellsCausingAura(SPELL_AURA_FEIGN_DEATH);

    while (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
        GetPlayer()->GetMotionMaster()->MovementExpired(false);

    if (mountDisplayId)
        GetPlayer()->Mount(mountDisplayId);

    // Check if it's a multi path
    if (!GetPlayer()->m_taxi.GetTaxiPath().empty())
        GetPlayer()->GetMotionMaster()->MoveTaxiFlight();
    else
        GetPlayer()->GetMotionMaster()->MoveTaxiFlight(path, pathNode);
}

LearnTaxiNodeResult WorldSession::SendLearnNewTaxiNode(Creature const* flightmaster)
{
    // find current node
    uint32 nearestNodeEntry = sObjectMgr.GetNearestTaxiNode(flightmaster->GetWorldLocation(), GetPlayer()->GetTeam());

    if (nearestNodeEntry == 0)
        return LearnTaxiNodeResult::InvalidNode;

    if (!GetPlayer()->m_taxi.LearnTaxiNode(nearestNodeEntry))
        return LearnTaxiNodeResult::KnownAlready;

    auto taxiStatus = std::make_unique<WorldPackets::Taxi::TaxiNodeStatus>();
    taxiStatus->flightmasterGuid = flightmaster->GetObjectGuid();
    taxiStatus->isKnown = true;
    SendPacket(std::move(taxiStatus));

    // Just the message "New flight path discovered!"
    auto newPath = std::make_unique<WorldPackets::Taxi::NewTaxiPath>();
    SendPacket(std::move(newPath));

    return LearnTaxiNodeResult::LearnedItJustNow;
}

#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
void WorldSession::HandleActivateTaxiExpressOpcode(WorldPackets::Taxi::ActivateTaxiExpress const& packet)
{
    Creature* npc = GetPlayer()->GetNPCIfCanInteractWith(packet.flightmasterGuid, UNIT_NPC_FLAG_FLIGHTMASTER);
    if (!npc)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: HandleActivateTaxiExpressOpcode - %s not found or you can't interact with it.", packet.flightmasterGuid.GetString().c_str());
        return;
    }

    if (packet.nodes.empty())
        return;

    GetPlayer()->ActivateTaxiPathTo(packet.nodes, npc);
}
#endif

void WorldSession::HandleActivateTaxiOpcode(WorldPackets::Taxi::ActivateTaxi const& packet)
{
    std::vector<uint32> nodes { packet.node1, packet.node2 };

    Creature* npc = GetPlayer()->GetNPCIfCanInteractWith(packet.flightmasterGuid, UNIT_NPC_FLAG_FLIGHTMASTER);
    if (!npc)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: HandleActivateTaxiOpcode - %s not found or you can't interact with it.", packet.flightmasterGuid.GetString().c_str());
        return;
    }

    GetPlayer()->ActivateTaxiPathTo(nodes, npc);
}
