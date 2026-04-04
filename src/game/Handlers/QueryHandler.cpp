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
#include "Database/DatabaseImpl.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "ObjectGuid.h"
#include "Player.h"

void WorldSession::SendNameQueryOpcode(Player* p)
{
    if (!p)
        return;

    auto nameResponse = std::make_unique<WorldPackets::Query::NameQueryResponse>();
    nameResponse->playerGuid = p->GetObjectGuid();
    nameResponse->name = p->GetName();
#if SUPPORTED_CLIENT_BUILD >= CLIENT_BUILD_1_12_1
    nameResponse->realmName = "";                           // realm name for cross realm BG usage
#endif
    nameResponse->race = p->GetRace();
    nameResponse->gender = p->GetGender();
    nameResponse->class_ = p->GetClass();
    SendPacket(std::move(nameResponse));
}

void WorldSession::SendNameQueryOpcodeFromDB(ObjectGuid guid)
{
    // Using the cache...
    if (PlayerCacheData* pData = sObjectMgr.GetPlayerDataByGUID(guid.GetCounter()))
    {
        auto nameResponse = std::make_unique<WorldPackets::Query::NameQueryResponse>();
        nameResponse->playerGuid = ObjectGuid(HIGHGUID_PLAYER, pData->uiGuid);
        nameResponse->name = pData->sName;
#if SUPPORTED_CLIENT_BUILD >= CLIENT_BUILD_1_12_1
        nameResponse->realmName = "";
#endif
        nameResponse->race = pData->uiRace;
        nameResponse->gender = pData->uiGender;
        nameResponse->class_ = pData->uiClass;
        SendPacket(std::move(nameResponse));
    }
}

void WorldSession::SendNameQueryOpcodeFromDBCallBack(QueryResult* result, uint32 accountId)
{
    if (!result)
        return;

    WorldSession* session = sWorld.FindSession(accountId);
    if (!session)
    {
        delete result;
        return;
    }

    Field* fields = result->Fetch();
    uint32 lowguid      = fields[0].GetUInt32();
    std::string name = fields[1].GetCppString();
    uint8 pRace = 0, pGender = 0, pClass = 0;
    if (!name.empty())
    {
        pRace        = fields[2].GetUInt8();
        pGender      = fields[3].GetUInt8();
        pClass       = fields[4].GetUInt8();
    }

    auto nameResponse = std::make_unique<WorldPackets::Query::NameQueryResponse>();
    nameResponse->playerGuid = ObjectGuid(HIGHGUID_PLAYER, lowguid);
    nameResponse->name = name;
#if SUPPORTED_CLIENT_BUILD >= CLIENT_BUILD_1_12_1
    nameResponse->realmName = "";
#endif
    nameResponse->race = pRace;
    nameResponse->gender = pGender;
    nameResponse->class_ = pClass;
    session->SendPacket(std::move(nameResponse));

    delete result;
}

void WorldSession::HandleQueryPlayerNameOpcode(WorldPackets::Query::QueryPlayerName const& packet)
{
    Player* pChar = sObjectMgr.GetPlayer(packet.playerGuid);

    if (pChar)
        SendNameQueryOpcode(pChar);
    else
        SendNameQueryOpcodeFromDB(packet.playerGuid);
}

void WorldSession::HandleQueryTimeOpcode(NullClientPacket const& /*packet*/)
{
    SendQueryTimeResponse();
}

// Only _static_ data send in this packet !!!
void WorldSession::HandleCreatureQueryOpcode(WorldPackets::Query::QueryCreature const& packet)
{
    CreatureInfo const* ci = sObjectMgr.GetCreatureTemplate(packet.entry);
    if (ci)
    {
        std::string const* name = &ci->name;
        std::string const* subName = &ci->subname;

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            CreatureLocale const* cl = sObjectMgr.GetCreatureLocale(packet.entry);
            if (cl)
            {
                if (cl->Name.size() > size_t(loc_idx) && !cl->Name[loc_idx].empty())
                    name = &cl->Name[loc_idx];
                if (cl->SubName.size() > size_t(loc_idx) && !cl->SubName[loc_idx].empty())
                    subName = &cl->SubName[loc_idx];
            }
        }

        auto response = std::make_unique<WorldPackets::Query::CreatureQueryResponse>();
        response->entry = packet.entry;
        response->found = true;
        response->name = *name;
        response->subName = *subName;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
        response->typeFlags = ci->GetTypeFlags();
#else
        response->typeFlags = ci->static_flags1;
#endif
        response->type = ci->type;
        response->petFamily = ci->pet_family;
        response->rank = ci->rank;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_7_1
        response->petSpellListId = ci->pet_spell_list_id;
#endif
        response->displayId = ci->display_id[0];
        response->civilian = ci->civilian;
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
        response->racialLeader = ci->racial_leader;
#endif
        SendPacket(std::move(response));
    }
    else
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: CMSG_CREATURE_QUERY - Guid: %s Entry: %u NO CREATURE INFO!",
                  packet.guid.GetString().c_str(), packet.entry);
        auto response = std::make_unique<WorldPackets::Query::CreatureQueryResponse>();
        response->entry = packet.entry;
        response->found = false;
        SendPacket(std::move(response));
    }
}

// Only _static_ data send in this packet !!!
void WorldSession::HandleGameObjectQueryOpcode(WorldPackets::Query::QueryGameObject const& packet)
{
    GameObjectInfo const* info = sObjectMgr.GetGameObjectTemplate(packet.entryID);
    if (info)
    {
        char const* name = info->name.c_str();
        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            GameObjectLocale const* gl = sObjectMgr.GetGameObjectLocale(packet.entryID);
            if (gl)
            {
                if (gl->Name.size() > size_t(loc_idx) && !gl->Name[loc_idx].empty())
                    name = gl->Name[loc_idx].c_str();
            }
        }

        auto response = std::make_unique<WorldPackets::Query::GameObjectQueryResponse>();
        response->entryID = packet.entryID;
        response->found = true;
        response->type = info->type;
        response->displayId = info->displayId;
        response->name = name;
#if SUPPORTED_CLIENT_BUILD >= CLIENT_BUILD_1_12_1
        response->icon = info->icon;
        static_assert(sizeof(response->rawData) >= 24 * sizeof(int32), "rawData must hold 24 int32");
        memcpy(response->rawData, info->raw.data, 24 * sizeof(int32));
#else
        static_assert(sizeof(response->rawData) >= 16 * sizeof(int32), "rawData must hold 16 int32");
        memcpy(response->rawData, info->raw.data, 16 * sizeof(int32));
#endif
        SendPacket(std::move(response));
    }
    else
    {
        sLog.Out(LOG_BASIC, LOG_LVL_DEBUG, "WORLD: CMSG_GAMEOBJECT_QUERY - Guid: %s Entry: %u Missing gameobject info!",
                  packet.guid.GetString().c_str(), packet.entryID);
        auto response = std::make_unique<WorldPackets::Query::GameObjectQueryResponse>();
        response->entryID = packet.entryID;
        response->found = false;
        SendPacket(std::move(response));
    }
}

void WorldSession::HandleCorpseQueryOpcode(NullClientPacket const& /*packet*/)
{
    Corpse* corpse = GetPlayer()->GetCorpse();

    if (!corpse)
    {
        auto response = std::make_unique<WorldPackets::Query::CorpseQueryResponse>();
        response->found = false;
        SendPacket(std::move(response));
        return;
    }

    uint32 corpsemapid = corpse->GetMapId();
    float x = corpse->GetPositionX();
    float y = corpse->GetPositionY();
    float z = corpse->GetPositionZ();
    int32 mapid = corpsemapid;

    // if corpse at different map
    if (corpsemapid != _player->GetMapId())
    {
        // search entrance map for proper show entrance
        if (MapEntry const* temp = sMapStorage.LookupEntry<MapEntry>(mapid))
        {
            if (temp->IsDungeon() && temp->ghostEntranceMap >= 0)
            {
                // if corpse map have entrance
                if (TerrainInfo const* entranceMap = sTerrainMgr.LoadTerrain(temp->ghostEntranceMap))
                {
                    mapid = temp->ghostEntranceMap;
                    x = temp->ghostEntranceX;
                    y = temp->ghostEntranceY;
                    z = entranceMap->GetHeightStatic(x, y, MAX_HEIGHT);
                }
            }
        }
    }

    auto response = std::make_unique<WorldPackets::Query::CorpseQueryResponse>();
    response->found = true;
    response->mapId = mapid;
    response->x = x;
    response->y = y;
    response->z = z;
    response->corpseMapId = corpsemapid;
    SendPacket(std::move(response));
}

void WorldSession::HandleNpcTextQueryOpcode(WorldPackets::Npc::NpcTextQuery const& packet)
{
    NpcText const* pGossip = sObjectMgr.GetNpcText(packet.textID);

    auto npcTextUpdate = std::make_unique<WorldPackets::Npc::NpcTextUpdate>();
    npcTextUpdate->textID = packet.textID;

    if (!pGossip)
    {
        for (uint32 i = 0; i < 8; ++i)
        {
            npcTextUpdate->options[i].maleText = "Greetings $N";
            npcTextUpdate->options[i].femaleText = "Greetings $N";
        }
    }
    else
    {
        int loc_idx = GetSessionDbLocaleIndex();
        for (int i = 0; i < 8; ++i)
        {
            BroadcastText const* bct = sObjectMgr.GetBroadcastTextLocale(pGossip->Options[i].BroadcastTextID);
            if (bct)
            {
                std::string const& maleText = bct->GetText(loc_idx, GENDER_MALE, true);
                std::string const& femaleText = bct->GetText(loc_idx, GENDER_FEMALE, true);

                npcTextUpdate->options[i].probability = pGossip->Options[i].Probability;

                if (maleText.empty())
                    npcTextUpdate->options[i].maleText = femaleText;
                else
                    npcTextUpdate->options[i].maleText = maleText;

                if (femaleText.empty())
                    npcTextUpdate->options[i].femaleText = maleText;
                else
                    npcTextUpdate->options[i].femaleText = femaleText;

                npcTextUpdate->options[i].languageId = bct->languageId;
                npcTextUpdate->options[i].emoteDelay1 = bct->emoteDelay1;
                npcTextUpdate->options[i].emoteId1 = bct->emoteId1;
                npcTextUpdate->options[i].emoteDelay2 = bct->emoteDelay2;
                npcTextUpdate->options[i].emoteId2 = bct->emoteId2;
                npcTextUpdate->options[i].emoteDelay3 = bct->emoteDelay3;
                npcTextUpdate->options[i].emoteId3 = bct->emoteId3;
            }
            else
            {
                npcTextUpdate->options[i].maleText = "Greetings $N";
                npcTextUpdate->options[i].femaleText = "Greetings $N";
            }
        }
    }

    SendPacket(std::move(npcTextUpdate));
}

void WorldSession::HandlePageTextQueryOpcode(WorldPackets::Query::QueryPageText const& packet)
{
    uint32 pageID = packet.pageID;
    while (pageID)
    {
        PageText const* pPage = sPageTextStore.LookupEntry<PageText>(pageID);
        auto pageResponse = std::make_unique<WorldPackets::Query::PageTextQueryResponse>();
        pageResponse->pageId = pageID;

        if (!pPage)
        {
            pageResponse->text = "Item page missing.";
            pageResponse->nextPageId = 0;
            pageID = 0;
        }
        else
        {
            char const* text = pPage->text;

            int loc_idx = GetSessionDbLocaleIndex();
            if (loc_idx >= 0)
            {
                PageTextLocale const* pl = sObjectMgr.GetPageTextLocale(pageID);
                if (pl)
                {
                    if (pl->text.size() > size_t(loc_idx) && !pl->text[loc_idx].empty())
                        text = pl->text[loc_idx].c_str();
                }
            }

            pageResponse->text = text;
            pageResponse->nextPageId = pPage->next_page;
            pageID = pPage->next_page;
        }
        SendPacket(std::move(pageResponse));
    }
}

void WorldSession::SendQueryTimeResponse()
{
    auto packet = std::make_unique<WorldPackets::Query::QueryTimeResponse>();
    packet->time = static_cast<uint32>(time(nullptr));
    SendPacket(std::move(packet));
}
