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
#include "Language.h"
#include "Database/DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Player.h"
#include "GossipDef.h"
#include "ScriptMgr.h"
#include "Creature.h"
#include "Pet.h"
#include "Spell.h"
#include "Chat.h"
#include "CharacterDatabaseCache.h"

enum StableResultCode
{
    STABLE_ERR_MONEY        = 0x01,                         // "you don't have enough money"
    STABLE_ERR_STABLE       = 0x06,                         // currently used in most fail cases
    STABLE_SUCCESS_STABLE   = 0x08,                         // stable success
    STABLE_SUCCESS_UNSTABLE = 0x09,                         // unstable/swap success
    STABLE_SUCCESS_BUY_SLOT = 0x0A,                         // buy slot success
};

void WorldSession::HandleTabardVendorActivateOpcode(WorldPackets::Npc::TabardVendorActivate const& packet)
{
    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(packet.guid, UNIT_NPC_FLAG_TABARDDESIGNER);
    if (!unit)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: HandleTabardVendorActivateOpcode - %s not found or you can't interact with him.", packet.guid.GetString().c_str());
        return;
    }

    GetPlayer()->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);

    SendTabardVendorActivate(packet.guid);
}

void WorldSession::SendTabardVendorActivate(ObjectGuid guid)
{
    auto tabardVendor = std::make_unique<WorldPackets::Npc::TabardVendorActivateResponse>();
    tabardVendor->tabardVendorNpcGuid = guid;
    SendPacket(std::move(tabardVendor));
}

void WorldSession::HandleBankerActivateOpcode(WorldPackets::Npc::BankerActivate const& packet)
{
    if (!CheckBanker(packet.guid))
        return;

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_FEIGN_DEATH))
        GetPlayer()->RemoveSpellsCausingAura(SPELL_AURA_FEIGN_DEATH);

    SendShowBank(packet.guid);
}

void WorldSession::SendShowBank(ObjectGuid guid)
{
    GetPlayer()->m_currentBankerGuid = guid;

    auto packet = std::make_unique<WorldPackets::Npc::ShowBank>();
    packet->bankerGuid = guid;
    SendPacket(std::move(packet));
}

void WorldSession::HandleTrainerListOpcode(WorldPackets::Npc::TrainerList const& packet)
{
    SendTrainerList(packet.guid);
}


void WorldSession::SendTrainerList(ObjectGuid guid)
{
    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_TRAINER);
    if (!unit)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: SendTrainerList - %s not found or you can't interact with him.", guid.GetString().c_str());
        return;
    }

    // trainer list loaded at check;
    if (!unit->IsTrainerOf(_player, true))
        return;

    CreatureInfo const* ci = unit->GetCreatureInfo();
    if (!ci)
        return;

    TrainerSpellData const* cSpells = unit->GetTrainerSpells();
    TrainerSpellData const* tSpells = unit->GetTrainerTemplateSpells();

    if (!cSpells && !tSpells)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: SendTrainerList - Training spells not found for %s", guid.GetString().c_str());
        return;
    }

    GetPlayer()->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);

    uint32 maxcount = (cSpells ? cSpells->spellList.size() : 0) + (tSpells ? tSpells->spellList.size() : 0);
    uint32 trainer_type = cSpells && cSpells->trainerType ? cSpells->trainerType : (tSpells ? tSpells->trainerType : 0);

    std::string strTitle;
    if (TrainerGreetingLocale const* trainerGreeting = sObjectMgr.GetTrainerGreetingLocale(guid.GetEntry()))
    {
        int locale_idx = GetSessionDbLocaleIndex();

        if ((int32)trainerGreeting->Content.size() > locale_idx + 1 && !trainerGreeting->Content[locale_idx + 1].empty())
            strTitle = trainerGreeting->Content[locale_idx + 1];
        else
            strTitle = trainerGreeting->Content[0];
    }
    else
    {
        strTitle = GetMangosString(LANG_NPC_TAINER_HELLO);
    }

    auto trainerPacket = std::make_unique<WorldPackets::Npc::TrainerListResponse>();
    trainerPacket->trainerGuid = guid;
    trainerPacket->trainerType = trainer_type;
    trainerPacket->title = strTitle;

    // reputation discount
    float fDiscountMod = _player->GetReputationPriceDiscount(unit);
    bool can_learn_primary_prof = GetPlayer()->GetFreePrimaryProfessionPoints() > 0;

    auto addSpells = [&](TrainerSpellData const* spellData)
    {
        if (!spellData)
            return;
        for (const auto& itr : spellData->spellList)
        {
            TrainerSpell const* tSpell = &itr.second;

            uint32 triggerSpell = sSpellMgr.GetSpellEntry(tSpell->spell)->EffectTriggerSpell[0];

            if (!_player->IsSpellFitByClassAndRace(triggerSpell))
                continue;

            SpellEntry const* triggerInfo = sSpellMgr.GetSpellEntry(triggerSpell);
            uint32 spellLevel = 0;
            if (tSpell->reqLevel)
                spellLevel = tSpell->reqLevel;
            else if (triggerInfo)
                spellLevel = triggerInfo->spellLevel;
            else
                continue;

            TrainerSpellState state = _player->GetTrainerSpellState(tSpell);
            bool primary_prof_first_rank = sSpellMgr.IsPrimaryProfessionFirstRankSpell(triggerSpell);
            SpellChainNode const* chain_node = sSpellMgr.GetSpellChainNode(triggerSpell);

            WorldPackets::Npc::TrainerSpellEntry entry;
            entry.spellId = tSpell->spell;
            entry.state = state == TRAINER_SPELL_GREEN_DISABLED ? TRAINER_SPELL_GREEN : state;
            entry.cost = static_cast<uint32>(tSpell->spellCost * fDiscountMod + 0.5f);
            entry.canLearnPrimaryProf = primary_prof_first_rank && can_learn_primary_prof ? 1 : 0;
            entry.isPrimaryProfFirstRank = primary_prof_first_rank ? 1 : 0;
            entry.spellLevel = static_cast<uint8>(spellLevel);
            entry.reqSkill = tSpell->reqSkill;
            entry.reqSkillValue = tSpell->reqSkillValue;
            if (chain_node)
            {
                if (chain_node->req)
                {
                    entry.spellReq1 = chain_node->req;
                    entry.spellReq2 = chain_node->prev;
                }
                else
                {
                    entry.spellReq1 = chain_node->prev;
                    entry.spellReq2 = 0;
                }
            }
            trainerPacket->spells.push_back(entry);
        }
    };

    addSpells(cSpells);
    addSpells(tSpells);

    SendPacket(std::move(trainerPacket));
}

void WorldSession::SendTrainingSuccess(ObjectGuid guid, uint32 spellId)
{
    auto packet = std::make_unique<WorldPackets::Npc::TrainerBuySucceeded>();
    packet->trainerGuid = guid;
    packet->spellId = spellId; // should be same as in packet from client
    SendPacket(std::move(packet));
}

void WorldSession::SendTrainingFailure(ObjectGuid guid, uint32 serviceId, uint32 errorCode)
{
    auto packet = std::make_unique<WorldPackets::Npc::TrainerBuyFailed>();
    packet->trainerGuid = guid;
    packet->serviceId = serviceId;
    packet->errorCode = errorCode;
    SendPacket(std::move(packet));
}

void WorldSession::HandleTrainerBuySpellOpcode(WorldPackets::Npc::TrainerBuySpell const& packet)
{
    sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: Received CMSG_TRAINER_BUY_SPELL Trainer: %s, learn spell id is: %u", packet.guid.GetString().c_str(), packet.spellId);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(packet.guid, UNIT_NPC_FLAG_TRAINER);

    if (!unit || !unit->IsTrainerOf(_player, true) || !unit->IsWithinLOSInMap(_player))
    {
        SendTrainingFailure(packet.guid, packet.spellId, TRAIN_FAIL_UNAVAILABLE);
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: HandleTrainerBuySpellOpcode - %s not found or you can't interact with him.", packet.guid.GetString().c_str());
        return;
    }

    // Check if the spell is present in the trainer's spell list.
    TrainerSpellData const* cSpells = unit->GetTrainerSpells();
    TrainerSpellData const* tSpells = unit->GetTrainerTemplateSpells();

    if (!cSpells && !tSpells)
    {
        SendTrainingFailure(packet.guid, packet.spellId, TRAIN_FAIL_UNAVAILABLE);
        return;
    }

    // Try to find the spell in npc_trainer.
    TrainerSpell const* trainer_spell = cSpells ? cSpells->Find(packet.spellId) : nullptr;

    // Not found, try find it in npc_trainer_template.
    if (!trainer_spell && tSpells)
        trainer_spell = tSpells->Find(packet.spellId);

    // Not found anywhere, cheating?
    if (!trainer_spell)
    {
        SendTrainingFailure(packet.guid, packet.spellId, TRAIN_FAIL_UNAVAILABLE);
        return;
    }

    // Can't be learned, cheat? Or double learn with lags...
    if (_player->GetTrainerSpellState(trainer_spell) != TRAINER_SPELL_GREEN)
    {
        SendTrainingFailure(packet.guid, packet.spellId, TRAIN_FAIL_NOT_ENOUGH_SKILL);
        return;
    }

    SpellEntry const* proto = sSpellMgr.GetSpellEntry(trainer_spell->spell);

    // Apply reputation discount.
    uint32 nSpellCost = uint32(trainer_spell->spellCost * _player->GetReputationPriceDiscount(unit) + 0.5f);

    // Check money requirement.
    if (_player->GetMoney() < nSpellCost)
    {
        SendTrainingFailure(packet.guid, packet.spellId, TRAIN_FAIL_NOT_ENOUGH_MONEY);
        return;
    }

    // All is good. Spell can be learned if we reach this point.
    _player->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    _player->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    _player->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);

    Spell* spell;
    if (proto->SpellVisual == 222)
        spell = new Spell(_player, proto, false);
    else
        spell = new Spell(unit, proto, false);

    SpellCastTargets targets;
    targets.setUnitTarget(_player);

    SpellCastResult cast_result = spell->prepare(std::move(targets));
    spell->update(1); // Update the spell right now. Prevents desynch => take twice the money if you click really fast.

    // Only charge player if cast of learning spell was successful.
    if (cast_result == SPELL_CAST_OK)
    {
        _player->ModifyMoney(-int32(nSpellCost));
        SendTrainingSuccess(packet.guid, packet.spellId);
    }
    else
        SendTrainingFailure(packet.guid, packet.spellId, TRAIN_FAIL_UNAVAILABLE);
}

void WorldSession::HandleGossipHelloOpcode(WorldPackets::Npc::GossipHello const& packet)
{
    Creature* pCreature = GetPlayer()->GetNPCIfCanInteractWith(packet.npcGuid, UNIT_NPC_FLAG_NONE);
    if (!pCreature)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: HandleGossipHelloOpcode - %s not found or you can't interact with him.", packet.npcGuid.GetString().c_str());
        return;
    }

    GetPlayer()->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);

    if (!pCreature->HasExtraFlag(CREATURE_FLAG_EXTRA_NO_MOVEMENT_PAUSE))
        pCreature->PauseOutOfCombatMovement();

    if (pCreature->IsSpiritGuide())
        pCreature->SendAreaSpiritHealerQueryOpcode(_player);

    if (!sScriptMgr.OnGossipHello(_player, pCreature))
    {
        _player->PrepareGossipMenu(pCreature, pCreature->GetDefaultGossipMenuId());
        _player->SendPreparedGossip(pCreature);
    }
}

void WorldSession::HandleGossipSelectOptionOpcode(WorldPackets::Npc::GossipSelectOption const& packet)
{
    bool const isCoded = _player->PlayerTalkClass->GossipOptionCoded(packet.gossipListId);
    if (isCoded && packet.code.empty())
        return;  // coded option requires a code from the client

    GetPlayer()->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);

    uint32 sender = _player->PlayerTalkClass->GossipOptionSender(packet.gossipListId);
    uint32 action = _player->PlayerTalkClass->GossipOptionAction(packet.gossipListId);

    // Only forward a non-null code to scripts for coded gossip options.
    const char* code = (isCoded && !packet.code.empty()) ? packet.code.c_str() : nullptr;

    if (packet.guid.IsAnyTypeCreature())
    {
        Creature* pCreature = GetPlayer()->GetNPCIfCanInteractWith(packet.guid, UNIT_NPC_FLAG_NONE);

        if (!pCreature)
        {
            sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: HandleGossipSelectOptionOpcode - %s not found or you can't interact with it.", packet.guid.GetString().c_str());
            return;
        }

        if (!pCreature->HasExtraFlag(CREATURE_FLAG_EXTRA_NO_MOVEMENT_PAUSE))
            pCreature->PauseOutOfCombatMovement();

        if (!sScriptMgr.OnGossipSelect(_player, pCreature, sender, action, code))
            _player->OnGossipSelect(pCreature, packet.gossipListId);
    }
    else if (packet.guid.IsGameObject())
    {
        GameObject* pGo = GetPlayer()->GetGameObjectIfCanInteractWith(packet.guid);

        if (!pGo)
        {
            sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: HandleGossipSelectOptionOpcode - %s not found or you can't interact with it.", packet.guid.GetString().c_str());
            return;
        }

        if (!sScriptMgr.OnGossipSelect(_player, pGo, sender, action, code))
            _player->OnGossipSelect(pGo, packet.gossipListId);
    }
}

void WorldSession::HandleSpiritHealerActivateOpcode(WorldPackets::Npc::SpiritHealerActivate const& packet)
{
    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(packet.guid, UNIT_NPC_FLAG_SPIRITHEALER);
    if (!unit)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: HandleSpiritHealerActivateOpcode - %s not found or you can't interact with him.", packet.guid.GetString().c_str());
        return;
    }

    GetPlayer()->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    SendSpiritResurrect();
}

void WorldSession::SendSpiritResurrect()
{
    _player->ResurrectPlayer(0.5f, true);

    _player->DurabilityLossAll(0.25f, true);

    // get corpse nearest graveyard
    WorldSafeLocsEntry const* corpseGrave = nullptr;
    Corpse* corpse = _player->GetCorpse();
    if (corpse)
        corpseGrave = sObjectMgr.GetClosestGraveYard(
                          corpse->GetPositionX(), corpse->GetPositionY(), corpse->GetPositionZ(), corpse->GetMapId(), _player->GetTeam());

    // now can spawn bones
    _player->SpawnCorpseBones();

    // teleport to nearest from corpse graveyard, if different from nearest to player ghost
    if (corpseGrave)
    {
        WorldSafeLocsEntry const* ghostGrave = sObjectMgr.GetClosestGraveYard(
                _player->GetPositionX(), _player->GetPositionY(), _player->GetPositionZ(), _player->GetMapId(), _player->GetTeam());

        float orientation = _player->GetOrientation();

        // World of Warcraft Client Patch 1.8.0 (2005-10-11)
        // - All graveyards that needed adjustment were changed so that a
        //   character's spirit comes into the world facing toward the Spirit Healer.
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_7_1
        if (float facing = sObjectMgr.GetWorldSafeLocFacing(corpseGrave->ID))
            orientation = facing;
#endif

        if (corpseGrave != ghostGrave)
            _player->TeleportTo(corpseGrave->map_id, corpseGrave->x, corpseGrave->y, corpseGrave->z, orientation);
        // or update at original position
        else
        {
            _player->GetCamera().UpdateVisibilityForOwner();
            _player->UpdateObjectVisibility();
        }
    }
    // or update at original position
    else
    {
        _player->GetCamera().UpdateVisibilityForOwner();
        _player->UpdateObjectVisibility();
    }
}

void WorldSession::HandleBinderActivateOpcode(WorldPackets::Npc::BinderActivate const& packet)
{
    if (!GetPlayer()->IsInWorld() || !GetPlayer()->IsAlive())
        return;

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(packet.npcGuid, UNIT_NPC_FLAG_INNKEEPER);
    if (!unit)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: HandleBinderActivateOpcode - %s not found or you can't interact with him.", packet.npcGuid.GetString().c_str());
        return;
    }

    GetPlayer()->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    SendBindPoint(unit);
}

void WorldSession::SendBindPoint(Creature* npc)
{
    // prevent set homebind to instances in any case
    if (GetPlayer()->GetMap()->Instanceable())
        return;

    // send spell for bind 3286 bind magic
    npc->CastSpell(_player, 3286, true);                    // Bind

    _player->PlayerTalkClass->CloseGossip();
}

void WorldSession::HandleListStabledPetsOpcode(WorldPackets::Npc::ListStabledPets const& packet)
{
    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(packet.npcGuid, UNIT_NPC_FLAG_STABLEMASTER);
    if (!unit)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: HandleListStabledPetsOpcode - %s not found or you can't interact with him.", packet.npcGuid.GetString().c_str());
        return;
    }

    GetPlayer()->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    SendStablePet(packet.npcGuid);
}

void WorldSession::SendStablePet(ObjectGuid guid)
{
    auto stablePetPacket = std::make_unique<WorldPackets::Npc::ListStabledPetsResponse>();
    stablePetPacket->npcGuid = guid;
    stablePetPacket->numSlots = GetPlayer()->m_stableSlots;

    Pet* pet = _player->GetPet();

    // not let move dead pet in slot
    if (pet && pet->IsAlive() && pet->GetPetType() == HUNTER_PET)
    {
        WorldPackets::Npc::StabledPetEntry entry;
        entry.petNumber = pet->GetCharmInfo()->GetPetNumber();
        entry.entry = pet->GetEntry();
        entry.level = pet->GetLevel();
        entry.name = pet->GetName();                        // petname
        entry.loyalty = pet->GetLoyaltyLevel();             // loyalty
        entry.slot = 0x01;                                  // client slot 1 == current pet (0)
        stablePetPacket->pets.push_back(entry);
    }
    // Pet may be despawned if owner went far away from pet for example.
    else if (CharacterPetCache const* currentPetData = sCharacterDatabaseCache.GetCharacterPetByOwner(_player->GetGUIDLow()))
    {
        WorldPackets::Npc::StabledPetEntry entry;
        entry.petNumber = currentPetData->id;
        entry.entry = currentPetData->entry;
        entry.level = currentPetData->level;
        entry.name = currentPetData->name;                  // petname
        entry.loyalty = currentPetData->loyalty;            // loyalty
        entry.slot = 0x01;                                  // client slot 1 == current pet (0)
        stablePetPacket->pets.push_back(entry);
    }
    CharPetMap const& pets = sCharacterDatabaseCache.GetCharPetsMap();
    CharPetMap::const_iterator myPets = pets.find(GetPlayer()->GetGUIDLow());
    if (myPets != pets.end())
        for (const auto it : myPets->second)
            if (it->slot >= PET_SAVE_FIRST_STABLE_SLOT && it->slot <= PET_SAVE_LAST_STABLE_SLOT)
            {
                WorldPackets::Npc::StabledPetEntry entry;
                entry.petNumber = it->id;                   // pet number
                entry.entry = it->entry;                    // creature entry
                entry.level = it->level;                    // level
                entry.name = it->name;                      // name
                entry.loyalty = it->loyalty;                // loyalty
                entry.slot = it->slot + 1;                  // slot
                stablePetPacket->pets.push_back(entry);
            }

    SendPacket(std::move(stablePetPacket));
}

void WorldSession::SendStableResult(uint8 res)
{
    auto packet = std::make_unique<WorldPackets::Npc::StableResult>();
    packet->result = res;
    SendPacket(std::move(packet));
}

bool WorldSession::CheckStableMaster(ObjectGuid guid)
{
    // spell case or GM
    if (guid == GetPlayer()->GetObjectGuid())
    {
        // command case will return only if player have real access to command
        if (!ChatHandler(GetPlayer()).FindCommand("stable"))
        {
            sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "%s attempt open stable in cheating way.", guid.GetString().c_str());
            return false;
        }
    }
    // stable master case
    else
    {
        if (!GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_STABLEMASTER))
        {
            sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "Stablemaster %s not found or you can't interact with him.", guid.GetString().c_str());
            return false;
        }
    }

    return true;
}

void WorldSession::HandleStablePet(WorldPackets::Npc::StablePet const& packet)
{
    if (!GetPlayer()->IsAlive())
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    if (!CheckStableMaster(packet.npcGuid))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    GetPlayer()->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);

    Pet* pet = _player->GetPet();

    // can't place in stable dead pet
    if (!pet || !pet->IsAlive() || pet->GetPetType() != HUNTER_PET)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    uint32 free_slot = PET_SAVE_FIRST_STABLE_SLOT;

    // Find free slot for pet
    bool usedSlots[PET_SAVE_LAST_STABLE_SLOT - PET_SAVE_FIRST_STABLE_SLOT + 1] = {false};
    CharPetMap const& pets = sCharacterDatabaseCache.GetCharPetsMap();
    CharPetMap::const_iterator myPets = pets.find(GetPlayer()->GetGUIDLow());
    if (myPets != pets.end())
        for (const auto it : myPets->second)
            if (it->slot >= PET_SAVE_FIRST_STABLE_SLOT && it->slot <= PET_SAVE_LAST_STABLE_SLOT)
                usedSlots[it->slot - PET_SAVE_FIRST_STABLE_SLOT] = true;

    for (free_slot = PET_SAVE_FIRST_STABLE_SLOT; free_slot <= PET_SAVE_LAST_STABLE_SLOT && usedSlots[free_slot - PET_SAVE_FIRST_STABLE_SLOT]; ++free_slot);

    if (free_slot <= GetPlayer()->m_stableSlots)
    {
        pet->Unsummon(PetSaveMode(free_slot), _player);
        SendStableResult(STABLE_SUCCESS_STABLE);
    }
    else
        SendStableResult(STABLE_ERR_STABLE);
}

void WorldSession::HandleUnstablePet(WorldPackets::Npc::UnstablePet const& packet)
{
    if (!CheckStableMaster(packet.npcGuid))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    GetPlayer()->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);

    CharacterPetCache const* petData = sCharacterDatabaseCache.GetCharacterPetCacheByOwnerAndId(_player->GetGUIDLow(), packet.petNumber);

    if (!petData || petData->slot < PET_SAVE_FIRST_STABLE_SLOT || petData->slot > PET_SAVE_LAST_STABLE_SLOT)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    uint32 creatureId = petData->entry;
    CreatureInfo const* creatureInfo = sObjectMgr.GetCreatureTemplate(creatureId);
    if (!creatureInfo || !creatureInfo->IsTameable())
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // Player may have a pet, but unsummoned currently (too far away from owner ...). Do not erase this pet!
    Pet* pet = _player->GetPet();
    if (pet || sCharacterDatabaseCache.GetCharacterPetByOwner(_player->GetGUIDLow()))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    Pet* newpet = new Pet(HUNTER_PET);
    if (!newpet->LoadPetFromDB(_player, creatureId, packet.petNumber))
    {
        delete newpet;
        newpet = nullptr;
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    SendStableResult(STABLE_SUCCESS_UNSTABLE);
}

void WorldSession::HandleBuyStableSlot(WorldPackets::Npc::BuyStableSlot const& packet)
{
    if (!CheckStableMaster(packet.npcGuid))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    GetPlayer()->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);

    if (GetPlayer()->m_stableSlots < MAX_PET_STABLES)
    {
        StableSlotPricesEntry const* SlotPrice = sStableSlotPricesStore.LookupEntry(GetPlayer()->m_stableSlots + 1);
        if (_player->GetMoney() >= SlotPrice->Price)
        {
            ++GetPlayer()->m_stableSlots;
            _player->ModifyMoney(-int32(SlotPrice->Price));
            SendStableResult(STABLE_SUCCESS_BUY_SLOT);
        }
        else
            SendStableResult(STABLE_ERR_MONEY);
    }
    else
        SendStableResult(STABLE_ERR_STABLE);
}

void WorldSession::HandleStableRevivePet(NullClientPacket const& /*packet*/)
{
}

void WorldSession::HandleStableSwapPet(WorldPackets::Npc::StableSwapPet const& packet)
{
    if (!CheckStableMaster(packet.npcGuid))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    GetPlayer()->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);

    Pet* pet = _player->GetPet();

    if (!pet || !pet->IsAlive() || pet->GetPetType() != HUNTER_PET)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // find swapped pet slot in stable
    CharacterPetCache const* swappedPet = sCharacterDatabaseCache.GetCharacterPetCacheByOwnerAndId(_player->GetGUIDLow(), packet.petNumber);
    if (!swappedPet)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    uint32 slot        = swappedPet->slot;
    uint32 creature_id = swappedPet->entry;

    if (!creature_id)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    CreatureInfo const* creatureInfo = sObjectMgr.GetCreatureTemplate(creature_id);
    if (!creatureInfo || !creatureInfo->IsTameable())
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    pet->Unsummon(PetSaveMode(slot), _player);

    // summon unstabled pet
    Pet* newpet = new Pet;
    if (!newpet->LoadPetFromDB(_player, creature_id, packet.petNumber))
    {
        delete newpet;
        SendStableResult(STABLE_ERR_STABLE);
    }
    else
        SendStableResult(STABLE_SUCCESS_UNSTABLE);
}

void WorldSession::HandleRepairItemOpcode(WorldPackets::Npc::RepairItem const& packet)
{
    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(packet.npcGuid, UNIT_NPC_FLAG_REPAIR);
    if (!unit)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: HandleRepairItemOpcode - %s not found or you can't interact with him.", packet.npcGuid.GetString().c_str());
        return;
    }

    GetPlayer()->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);

    // reputation discount
    float discountMod = _player->GetReputationPriceDiscount(unit);

    if (packet.itemGuid)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "ITEM: %s repair of %s", packet.npcGuid.GetString().c_str(), packet.itemGuid.GetString().c_str());
        if (Item* item = _player->GetItemByGuid(packet.itemGuid))
            _player->DurabilityRepair(item->GetPos(), true, discountMod);
    }
    else
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "ITEM: %s repair all items", packet.npcGuid.GetString().c_str());
        _player->DurabilityRepairAll(true, discountMod);
    }
}
