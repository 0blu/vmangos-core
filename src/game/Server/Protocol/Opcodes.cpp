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

/** \file
    \ingroup u2w
*/

#include "Opcodes.h"

struct Handlers
{
    OpcodeHandler handlers[NUM_MSG_TYPES];
};

template<typename T>
struct get_packet_class
{
};

template<typename PacketClass>
struct get_packet_class<void(WorldSession::*)(PacketClass&)>
{
    using type = PacketClass;
};

constexpr Handlers BuildOpcodeList()
{
    Handlers list{};

    // For WorldPacket handlers
    #define LEGACY_HANDLER(opcode, requiredState, schedulingStrategy, handlerPtr) \
        { \
            OpcodeHandler& ref = list.handlers[(opcode)]; \
            ref.name = #opcode; \
            ref.status = (requiredState); \
            ref.packetProcessing = (schedulingStrategy); \
            ref.handler = (handlerPtr); \
        }

    // For generic handlers
    #define DEFINE_HANDLER(opcode, requiredState, schedulingStrategy, handlerPtr) \
        { \
            OpcodeHandler& ref = list.handlers[(opcode)]; \
            ref.name = #opcode; \
            ref.status = (requiredState); \
            ref.packetProcessing = (schedulingStrategy); \
            ref.handler = &WorldSession::Handle_Generic<get_packet_class<decltype(handlerPtr)>::type, (handlerPtr)>; \
        }

    // Correspondence between opcodes and their names
    LEGACY_HANDLER(MSG_NULL_ACTION,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_BOOTME,                       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_DBLOOKUP,                     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_DBLOOKUP,                     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_QUERY_OBJECT_POSITION,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_QUERY_OBJECT_POSITION,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_QUERY_OBJECT_ROTATION,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_QUERY_OBJECT_ROTATION,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    DEFINE_HANDLER(CMSG_WORLD_TELEPORT,               STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleWorldTeleportOpcode);
    LEGACY_HANDLER(CMSG_TELEPORT_TO_UNIT,             STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleTeleportToUnitOpcode);
    LEGACY_HANDLER(CMSG_ZONE_MAP,                     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_ZONE_MAP,                     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_DEBUG_CHANGECELLZONE,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_EMBLAZON_TABARD_OBSOLETE,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_UNEMBLAZON_TABARD_OBSOLETE,   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_RECHARGE,                     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_LEARN_SPELL,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_CREATEMONSTER,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_DESTROYMONSTER,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_CREATEITEM,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_CREATEGAMEOBJECT,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    LEGACY_HANDLER(SMSG_CHECK_FOR_BOTS,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#endif
    LEGACY_HANDLER(CMSG_MAKEMONSTERATTACKGUID,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    LEGACY_HANDLER(CMSG_BOT_DETECTED2,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
#endif
    LEGACY_HANDLER(CMSG_FORCEACTION,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_FORCEACTIONONOTHER,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_FORCEACTIONSHOW,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_FORCEACTIONSHOW,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    LEGACY_HANDLER(CMSG_PETGODMODE,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_PETGODMODE,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#endif
    LEGACY_HANDLER(SMSG_DEBUGINFOSPELLMISS_OBSOLETE,  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    LEGACY_HANDLER(CMSG_WEATHER_SPEED_CHEAT,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
#endif
    LEGACY_HANDLER(CMSG_UNDRESSPLAYER,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_BEASTMASTER,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GODMODE,                      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_GODMODE,                      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_CHEAT_SETMONEY,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_LEVEL_CHEAT,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_PET_LEVEL_CHEAT,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    LEGACY_HANDLER(CMSG_SET_WORLDSTATE,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
#endif
    LEGACY_HANDLER(CMSG_COOLDOWN_CHEAT,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_USE_SKILL_CHEAT,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_FLAG_QUEST,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_FLAG_QUEST_FINISH,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_CLEAR_QUEST,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_SEND_EVENT,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_DEBUG_AISTATE,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_DEBUG_AISTATE,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_DISABLE_PVP_CHEAT,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_ADVANCE_SPAWN_TIME,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_PVP_PORT_OBSOLETE,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_AUTH_SRP6_BEGIN,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_AUTH_SRP6_PROOF,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_AUTH_SRP6_RECODE,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    DEFINE_HANDLER(CMSG_CHAR_CREATE,                  STATUS_AUTHED,    PACKET_PROCESS_WORLD,         &WorldSession::HandleCharCreateOpcode);
    DEFINE_HANDLER(CMSG_CHAR_ENUM,                    STATUS_AUTHED,    PACKET_PROCESS_WORLD,         &WorldSession::HandleCharEnumOpcode);
    DEFINE_HANDLER(CMSG_CHAR_DELETE,                  STATUS_AUTHED,    PACKET_PROCESS_WORLD,         &WorldSession::HandleCharDeleteOpcode);
    LEGACY_HANDLER(SMSG_AUTH_SRP6_RESPONSE,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_CHAR_CREATE,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_CHAR_ENUM,                    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_CHAR_DELETE,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    DEFINE_HANDLER(CMSG_PLAYER_LOGIN,                 STATUS_AUTHED,    PACKET_PROCESS_WORLD,         &WorldSession::HandlePlayerLoginOpcode);
    LEGACY_HANDLER(SMSG_NEW_WORLD,                    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_TRANSFER_PENDING,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_TRANSFER_ABORTED,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_CHARACTER_LOGIN_FAILED,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_LOGIN_SETTIMESPEED,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_GAMETIME_UPDATE,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GAMETIME_SET,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_GAMETIME_SET,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GAMESPEED_SET,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_GAMESPEED_SET,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_SERVERTIME,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_SERVERTIME,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_PLAYER_LOGOUT,                STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandlePlayerLogoutOpcode);
    LEGACY_HANDLER(CMSG_LOGOUT_REQUEST,               STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleLogoutRequestOpcode);
    LEGACY_HANDLER(SMSG_LOGOUT_RESPONSE,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_LOGOUT_COMPLETE,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_LOGOUT_CANCEL,                STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleLogoutCancelOpcode);
    LEGACY_HANDLER(SMSG_LOGOUT_CANCEL_ACK,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_NAME_QUERY,                   STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleNameQueryOpcode);
    LEGACY_HANDLER(SMSG_NAME_QUERY_RESPONSE,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_PET_NAME_QUERY,               STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetNameQueryOpcode);
    LEGACY_HANDLER(SMSG_PET_NAME_QUERY_RESPONSE,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GUILD_QUERY,                  STATUS_AUTHED,    PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildQueryOpcode);
    LEGACY_HANDLER(SMSG_GUILD_QUERY_RESPONSE,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_ITEM_QUERY_SINGLE,            STATUS_LOGGEDIN,  PACKET_PROCESS_DB_QUERY,      &WorldSession::HandleItemQuerySingleOpcode);
    LEGACY_HANDLER(CMSG_ITEM_QUERY_MULTIPLE,          STATUS_NEVER,     PACKET_PROCESS_DB_QUERY,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_ITEM_QUERY_SINGLE_RESPONSE,   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_ITEM_QUERY_MULTIPLE_RESPONSE, STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_PAGE_TEXT_QUERY,              STATUS_LOGGEDIN,  PACKET_PROCESS_DB_QUERY,      &WorldSession::HandlePageTextQueryOpcode);
    LEGACY_HANDLER(SMSG_PAGE_TEXT_QUERY_RESPONSE,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_QUEST_QUERY,                  STATUS_LOGGEDIN,  PACKET_PROCESS_DB_QUERY,      &WorldSession::HandleQuestQueryOpcode);
    LEGACY_HANDLER(SMSG_QUEST_QUERY_RESPONSE,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GAMEOBJECT_QUERY,             STATUS_LOGGEDIN,  PACKET_PROCESS_DB_QUERY,      &WorldSession::HandleGameObjectQueryOpcode);
    LEGACY_HANDLER(SMSG_GAMEOBJECT_QUERY_RESPONSE,    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_CREATURE_QUERY,               STATUS_LOGGEDIN,  PACKET_PROCESS_DB_QUERY,      &WorldSession::HandleCreatureQueryOpcode);
    LEGACY_HANDLER(SMSG_CREATURE_QUERY_RESPONSE,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_WHO,                          STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleWhoOpcode);
    LEGACY_HANDLER(SMSG_WHO,                          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_WHOIS,                        STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleWhoisOpcode);
    LEGACY_HANDLER(SMSG_WHOIS,                        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_FRIEND_LIST,                  STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleFriendListOpcode);
    LEGACY_HANDLER(SMSG_FRIEND_LIST,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_FRIEND_STATUS,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_ADD_FRIEND,                   STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleAddFriendOpcode);
    LEGACY_HANDLER(CMSG_DEL_FRIEND,                   STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleDelFriendOpcode);
    LEGACY_HANDLER(SMSG_IGNORE_LIST,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_ADD_IGNORE,                   STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleAddIgnoreOpcode);
    LEGACY_HANDLER(CMSG_DEL_IGNORE,                   STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleDelIgnoreOpcode);
    DEFINE_HANDLER(CMSG_GROUP_INVITE,                 STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleGroupInviteOpcode);
    LEGACY_HANDLER(SMSG_GROUP_INVITE,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GROUP_CANCEL,                 STATUS_LOGGEDIN,  PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_GROUP_CANCEL,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GROUP_ACCEPT,                 STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleGroupAcceptOpcode);
    LEGACY_HANDLER(CMSG_GROUP_DECLINE,                STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleGroupDeclineOpcode);
    LEGACY_HANDLER(SMSG_GROUP_DECLINE,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GROUP_UNINVITE,               STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleGroupUninviteOpcode);
    LEGACY_HANDLER(CMSG_GROUP_UNINVITE_GUID,          STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleGroupUninviteGuidOpcode);
    LEGACY_HANDLER(SMSG_GROUP_UNINVITE,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GROUP_SET_LEADER,             STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleGroupSetLeaderOpcode);
    LEGACY_HANDLER(SMSG_GROUP_SET_LEADER,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_LOOT_METHOD,                  STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleLootMethodOpcode);
    LEGACY_HANDLER(CMSG_GROUP_DISBAND,                STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleGroupDisbandOpcode);
    LEGACY_HANDLER(SMSG_GROUP_DESTROYED,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_GROUP_LIST,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PARTY_MEMBER_STATS,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PARTY_COMMAND_RESULT,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_UPDATE_GROUP_MEMBERS,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GUILD_CREATE,                 STATUS_NEVER,     PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildCreateOpcode);
    LEGACY_HANDLER(CMSG_GUILD_INVITE,                 STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildInviteOpcode);
    LEGACY_HANDLER(SMSG_GUILD_INVITE,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GUILD_ACCEPT,                 STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildAcceptOpcode);
    LEGACY_HANDLER(CMSG_GUILD_DECLINE,                STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildDeclineOpcode);
    LEGACY_HANDLER(SMSG_GUILD_DECLINE,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GUILD_INFO,                   STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildInfoOpcode);
    LEGACY_HANDLER(SMSG_GUILD_INFO,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GUILD_ROSTER,                 STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildRosterOpcode);
    LEGACY_HANDLER(SMSG_GUILD_ROSTER,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GUILD_PROMOTE,                STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildPromoteOpcode);
    LEGACY_HANDLER(CMSG_GUILD_DEMOTE,                 STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildDemoteOpcode);
    LEGACY_HANDLER(CMSG_GUILD_LEAVE,                  STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildLeaveOpcode);
    LEGACY_HANDLER(CMSG_GUILD_REMOVE,                 STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildRemoveOpcode);
    LEGACY_HANDLER(CMSG_GUILD_DISBAND,                STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildDisbandOpcode);
    LEGACY_HANDLER(CMSG_GUILD_LEADER,                 STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildLeaderOpcode);
    LEGACY_HANDLER(CMSG_GUILD_MOTD,                   STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildMOTDOpcode);
    LEGACY_HANDLER(SMSG_GUILD_EVENT,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_GUILD_COMMAND_RESULT,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_UPDATE_GUILD,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_MESSAGECHAT,                  STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleMessagechatOpcode);
    LEGACY_HANDLER(SMSG_MESSAGECHAT,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_JOIN_CHANNEL,                 STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleJoinChannelOpcode);
    LEGACY_HANDLER(CMSG_LEAVE_CHANNEL,                STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleLeaveChannelOpcode);
    LEGACY_HANDLER(SMSG_CHANNEL_NOTIFY,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_CHANNEL_LIST,                 STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChannelListOpcode);
    LEGACY_HANDLER(SMSG_CHANNEL_LIST,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_CHANNEL_PASSWORD,             STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChannelPasswordOpcode);
    LEGACY_HANDLER(CMSG_CHANNEL_SET_OWNER,            STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChannelSetOwnerOpcode);
    LEGACY_HANDLER(CMSG_CHANNEL_OWNER,                STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChannelOwnerOpcode);
    LEGACY_HANDLER(CMSG_CHANNEL_MODERATOR,            STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChannelModeratorOpcode);
    LEGACY_HANDLER(CMSG_CHANNEL_UNMODERATOR,          STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChannelUnmoderatorOpcode);
    LEGACY_HANDLER(CMSG_CHANNEL_MUTE,                 STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChannelMuteOpcode);
    LEGACY_HANDLER(CMSG_CHANNEL_UNMUTE,               STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChannelUnmuteOpcode);
    LEGACY_HANDLER(CMSG_CHANNEL_INVITE,               STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChannelInviteOpcode);
    LEGACY_HANDLER(CMSG_CHANNEL_KICK,                 STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChannelKickOpcode);
    LEGACY_HANDLER(CMSG_CHANNEL_BAN,                  STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChannelBanOpcode);
    LEGACY_HANDLER(CMSG_CHANNEL_UNBAN,                STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChannelUnbanOpcode);
    LEGACY_HANDLER(CMSG_CHANNEL_ANNOUNCEMENTS,        STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChannelAnnouncementsOpcode);
    LEGACY_HANDLER(CMSG_CHANNEL_MODERATE,             STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChannelModerateOpcode);
    LEGACY_HANDLER(SMSG_UPDATE_OBJECT,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_DESTROY_OBJECT,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_USE_ITEM,                     STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleUseItemOpcode);
    LEGACY_HANDLER(CMSG_OPEN_ITEM,                    STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleOpenItemOpcode);
    LEGACY_HANDLER(CMSG_READ_ITEM,                    STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleReadItemOpcode);
    LEGACY_HANDLER(SMSG_READ_ITEM_OK,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_READ_ITEM_FAILED,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_ITEM_COOLDOWN,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GAMEOBJ_USE,                  STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleGameObjectUseOpcode);
    LEGACY_HANDLER(CMSG_GAMEOBJ_CHAIR_USE_OBSOLETE,   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_GAMEOBJECT_CUSTOM_ANIM,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_AREATRIGGER,                  STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleAreaTriggerOpcode);
    LEGACY_HANDLER(MSG_MOVE_START_FORWARD,            STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_START_BACKWARD,           STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_STOP,                     STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_START_STRAFE_LEFT,        STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_START_STRAFE_RIGHT,       STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_STOP_STRAFE,              STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_JUMP,                     STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_START_TURN_LEFT,          STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_START_TURN_RIGHT,         STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_STOP_TURN,                STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_START_PITCH_UP,           STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_START_PITCH_DOWN,         STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_STOP_PITCH,               STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_SET_RUN_MODE,             STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_SET_WALK_MODE,            STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_TOGGLE_LOGGING,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_TELEPORT,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_TELEPORT_CHEAT,           STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleMoveSetRawPosition);
    LEGACY_HANDLER(MSG_MOVE_TELEPORT_ACK,             STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMoveTeleportAckOpcode);
    LEGACY_HANDLER(MSG_MOVE_TOGGLE_FALL_LOGGING,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_FALL_LAND,                STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_START_SWIM,               STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_STOP_SWIM,                STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_SET_RUN_SPEED_CHEAT,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_SET_RUN_SPEED,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_SET_RUN_BACK_SPEED_CHEAT, STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_SET_RUN_BACK_SPEED,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_SET_WALK_SPEED_CHEAT,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_SET_WALK_SPEED,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_SET_SWIM_SPEED_CHEAT,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_SET_SWIM_SPEED,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_SET_SWIM_BACK_SPEED_CHEAT, STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_SET_SWIM_BACK_SPEED,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_SET_ALL_SPEED_CHEAT,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_SET_TURN_RATE_CHEAT,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_SET_TURN_RATE,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_TOGGLE_COLLISION_CHEAT,   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_SET_FACING,               STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_SET_PITCH,                STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(MSG_MOVE_WORLDPORT_ACK,            STATUS_TRANSFER,  PACKET_PROCESS_WORLD,         &WorldSession::HandleMoveWorldportAckOpcode);
    LEGACY_HANDLER(SMSG_MONSTER_MOVE,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_MOVE_WATER_WALK,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_MOVE_LAND_WALK,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_MOVE_SET_RAW_POSITION_ACK,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_MOVE_SET_RAW_POSITION,        STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleMoveSetRawPosition);
    LEGACY_HANDLER(SMSG_FORCE_RUN_SPEED_CHANGE,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_FORCE_RUN_SPEED_CHANGE_ACK,   STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleForceSpeedChangeAckOpcodes);
    LEGACY_HANDLER(SMSG_FORCE_RUN_BACK_SPEED_CHANGE,  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_FORCE_RUN_BACK_SPEED_CHANGE_ACK, STATUS_LOGGEDIN, PACKET_PROCESS_MOVEMENT,    &WorldSession::HandleForceSpeedChangeAckOpcodes);
    LEGACY_HANDLER(SMSG_FORCE_SWIM_SPEED_CHANGE,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_FORCE_SWIM_SPEED_CHANGE_ACK,  STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleForceSpeedChangeAckOpcodes);
    LEGACY_HANDLER(SMSG_FORCE_MOVE_ROOT,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_FORCE_MOVE_ROOT_ACK,          STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMoveRootAck);
    LEGACY_HANDLER(SMSG_FORCE_MOVE_UNROOT,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_FORCE_MOVE_UNROOT_ACK,        STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMoveRootAck);
    LEGACY_HANDLER(MSG_MOVE_ROOT,                     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_UNROOT,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_HEARTBEAT,                STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(SMSG_MOVE_KNOCK_BACK,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_MOVE_KNOCK_BACK_ACK,          STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMoveKnockBackAck);
    LEGACY_HANDLER(MSG_MOVE_KNOCK_BACK,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_MOVE_FEATHER_FALL,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_MOVE_NORMAL_FALL,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_MOVE_SET_HOVER,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_MOVE_UNSET_HOVER,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_MOVE_HOVER_ACK,               STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementFlagChangeToggleAck);
    LEGACY_HANDLER(MSG_MOVE_HOVER,                    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_TRIGGER_CINEMATIC_CHEAT,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_OPENING_CINEMATIC,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_TRIGGER_CINEMATIC,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_NEXT_CINEMATIC_CAMERA,        STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleNextCinematicCamera);
    LEGACY_HANDLER(CMSG_COMPLETE_CINEMATIC,           STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleCompleteCinematic);
    LEGACY_HANDLER(SMSG_TUTORIAL_FLAGS,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_TUTORIAL_FLAG,                STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleTutorialFlagOpcode);
    LEGACY_HANDLER(CMSG_TUTORIAL_CLEAR,               STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleTutorialClearOpcode);
    LEGACY_HANDLER(CMSG_TUTORIAL_RESET,               STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleTutorialResetOpcode);
    LEGACY_HANDLER(CMSG_STANDSTATECHANGE,             STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleStandStateChangeOpcode);
    LEGACY_HANDLER(CMSG_EMOTE,                        STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleEmoteOpcode);
    LEGACY_HANDLER(SMSG_EMOTE,                        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_TEXT_EMOTE,                   STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleTextEmoteOpcode);
    LEGACY_HANDLER(SMSG_TEXT_EMOTE,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_AUTOEQUIP_GROUND_ITEM,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_AUTOSTORE_GROUND_ITEM,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_AUTOSTORE_LOOT_ITEM,          STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleAutostoreLootItemOpcode);
    LEGACY_HANDLER(CMSG_STORE_LOOT_IN_SLOT,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_AUTOEQUIP_ITEM,               STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleAutoEquipItemOpcode);
    LEGACY_HANDLER(CMSG_AUTOSTORE_BAG_ITEM,           STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleAutoStoreBagItemOpcode);
    LEGACY_HANDLER(CMSG_SWAP_ITEM,                    STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleSwapItem);
    LEGACY_HANDLER(CMSG_SWAP_INV_ITEM,                STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleSwapInvItemOpcode);
    LEGACY_HANDLER(CMSG_SPLIT_ITEM,                   STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleSplitItemOpcode);
    LEGACY_HANDLER(CMSG_AUTOEQUIP_ITEM_SLOT,          STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleAutoEquipItemSlotOpcode);
    LEGACY_HANDLER(CMSG_DROP_ITEM,                    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_DESTROYITEM,                  STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleDestroyItemOpcode);
    LEGACY_HANDLER(SMSG_INVENTORY_CHANGE_FAILURE,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_OPEN_CONTAINER,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_INSPECT,                      STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleInspectOpcode);
    LEGACY_HANDLER(SMSG_INSPECT,                      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_INITIATE_TRADE,               STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleInitiateTradeOpcode);
    LEGACY_HANDLER(CMSG_BEGIN_TRADE,                  STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleBeginTradeOpcode);
    LEGACY_HANDLER(CMSG_BUSY_TRADE,                   STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleBusyTradeOpcode);
    LEGACY_HANDLER(CMSG_IGNORE_TRADE,                 STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleIgnoreTradeOpcode);
    LEGACY_HANDLER(CMSG_ACCEPT_TRADE,                 STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleAcceptTradeOpcode);
    LEGACY_HANDLER(CMSG_UNACCEPT_TRADE,               STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleUnacceptTradeOpcode);
    LEGACY_HANDLER(CMSG_CANCEL_TRADE,                 STATUS_LOGGEDIN_OR_RECENTLY_LOGGEDOUT, PACKET_PROCESS_MAP,         &WorldSession::HandleCancelTradeOpcode);
    LEGACY_HANDLER(CMSG_SET_TRADE_ITEM,               STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleSetTradeItemOpcode);
    LEGACY_HANDLER(CMSG_CLEAR_TRADE_ITEM,             STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleClearTradeItemOpcode);
    LEGACY_HANDLER(CMSG_SET_TRADE_GOLD,               STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleSetTradeGoldOpcode);
    LEGACY_HANDLER(SMSG_TRADE_STATUS,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_TRADE_STATUS_EXTENDED,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_INITIALIZE_FACTIONS,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SET_FACTION_VISIBLE,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SET_FACTION_STANDING,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_SET_FACTION_ATWAR,            STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleSetFactionAtWarOpcode);
    LEGACY_HANDLER(CMSG_SET_FACTION_CHEAT,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_Deprecated);
    LEGACY_HANDLER(SMSG_SET_PROFICIENCY,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_SET_ACTION_BUTTON,            STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleSetActionButtonOpcode);
    LEGACY_HANDLER(SMSG_ACTION_BUTTONS,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_INITIAL_SPELLS,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_LEARNED_SPELL,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SUPERCEDED_SPELL,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_NEW_SPELL_SLOT,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_CAST_SPELL,                   STATUS_LOGGEDIN,  PACKET_PROCESS_SPELLS,   &WorldSession::HandleCastSpellOpcode);
    LEGACY_HANDLER(CMSG_CANCEL_CAST,                  STATUS_LOGGEDIN,  PACKET_PROCESS_SPELLS,   &WorldSession::HandleCancelCastOpcode);
    LEGACY_HANDLER(SMSG_CAST_RESULT,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPELL_START,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPELL_GO,                     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPELL_FAILURE,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPELL_COOLDOWN,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_COOLDOWN_EVENT,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_CANCEL_AURA,                  STATUS_LOGGEDIN,  PACKET_PROCESS_SPELLS,        &WorldSession::HandleCancelAuraOpcode);
    LEGACY_HANDLER(SMSG_UPDATE_AURA_DURATION,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PET_CAST_FAILED,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_CHANNEL_START,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_CHANNEL_UPDATE,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_CANCEL_CHANNELLING,           STATUS_LOGGEDIN,  PACKET_PROCESS_SPELLS,        &WorldSession::HandleCancelChanneling);
    LEGACY_HANDLER(SMSG_AI_REACTION,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_SET_SELECTION,                STATUS_LOGGEDIN,  PACKET_PROCESS_SPELLS,        &WorldSession::HandleSetSelectionOpcode);
    LEGACY_HANDLER(CMSG_SET_TARGET_OBSOLETE,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_UNUSED,                       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_UNUSED2,                      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_ATTACKSWING,                  STATUS_LOGGEDIN,  PACKET_PROCESS_SPELLS,        &WorldSession::HandleAttackSwingOpcode);
    LEGACY_HANDLER(CMSG_ATTACKSTOP,                   STATUS_LOGGEDIN,  PACKET_PROCESS_SPELLS,        &WorldSession::HandleAttackStopOpcode);
    LEGACY_HANDLER(SMSG_ATTACKSTART,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_ATTACKSTOP,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_ATTACKSWING_NOTINRANGE,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_ATTACKSWING_BADFACING,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_ATTACKSWING_NOTSTANDING,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_ATTACKSWING_DEADTARGET,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_ATTACKSWING_CANT_ATTACK,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_ATTACKERSTATEUPDATE,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_VICTIMSTATEUPDATE_OBSOLETE,   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_DAMAGE_DONE_OBSOLETE,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_DAMAGE_TAKEN_OBSOLETE,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_CANCEL_COMBAT,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PLAYER_COMBAT_XP_GAIN_OBSOLETE,    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
    LEGACY_HANDLER(SMSG_SPELLHEALLOG,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPELLENERGIZELOG,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#endif
    LEGACY_HANDLER(CMSG_SHEATHE_OBSOLETE,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_SAVE_PLAYER,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_SETDEATHBINDPOINT,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_BINDPOINTUPDATE,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GETDEATHBINDZONE,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_BINDZONEREPLY,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PLAYERBOUND,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
    LEGACY_HANDLER(SMSG_CLIENT_CONTROL_UPDATE,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#endif
    LEGACY_HANDLER(CMSG_REPOP_REQUEST,                STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleRepopRequestOpcode);
    LEGACY_HANDLER(SMSG_RESURRECT_REQUEST,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_RESURRECT_RESPONSE,           STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleResurrectResponseOpcode);
    LEGACY_HANDLER(CMSG_LOOT,                         STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleLootOpcode);
    LEGACY_HANDLER(CMSG_LOOT_MONEY,                   STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleLootMoneyOpcode);
    LEGACY_HANDLER(CMSG_LOOT_RELEASE,                 STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleLootReleaseOpcode);
    LEGACY_HANDLER(SMSG_LOOT_RESPONSE,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_LOOT_RELEASE_RESPONSE,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_LOOT_REMOVED,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_LOOT_MONEY_NOTIFY,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_LOOT_ITEM_NOTIFY,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_LOOT_CLEAR_MONEY,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_ITEM_PUSH_RESULT,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_DUEL_REQUESTED,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_DUEL_OUTOFBOUNDS,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_DUEL_INBOUNDS,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_DUEL_COMPLETE,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_DUEL_WINNER,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_DUEL_ACCEPTED,                STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleDuelAcceptedOpcode);
    LEGACY_HANDLER(CMSG_DUEL_CANCELLED,               STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleDuelCancelledOpcode);
    LEGACY_HANDLER(SMSG_MOUNTRESULT,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_DISMOUNTRESULT,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PUREMOUNT_CANCELLED_OBSOLETE, STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_MOUNTSPECIAL_ANIM,            STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleMountSpecialAnimOpcode);
    LEGACY_HANDLER(SMSG_MOUNTSPECIAL_ANIM,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PET_TAME_FAILURE,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_PET_SET_ACTION,               STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetSetAction);
    LEGACY_HANDLER(CMSG_PET_ACTION,                   STATUS_LOGGEDIN,  PACKET_PROCESS_SPELLS,        &WorldSession::HandlePetAction);
    LEGACY_HANDLER(CMSG_PET_ABANDON,                  STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetAbandon);
    LEGACY_HANDLER(CMSG_PET_RENAME,                   STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetRename);
    LEGACY_HANDLER(SMSG_PET_NAME_INVALID,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PET_SPELLS,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_5_1
    LEGACY_HANDLER(SMSG_PET_MODE,                     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#endif
    LEGACY_HANDLER(CMSG_GOSSIP_HELLO,                 STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleGossipHelloOpcode);
    LEGACY_HANDLER(CMSG_GOSSIP_SELECT_OPTION,         STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleGossipSelectOptionOpcode);
    LEGACY_HANDLER(SMSG_GOSSIP_MESSAGE,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_GOSSIP_COMPLETE,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_NPC_TEXT_QUERY,               STATUS_LOGGEDIN,  PACKET_PROCESS_DB_QUERY,      &WorldSession::HandleNpcTextQueryOpcode);
    LEGACY_HANDLER(SMSG_NPC_TEXT_UPDATE,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_NPC_WONT_TALK,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_QUESTGIVER_STATUS_QUERY,      STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleQuestgiverStatusQueryOpcode);
    LEGACY_HANDLER(SMSG_QUESTGIVER_STATUS,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_QUESTGIVER_HELLO,             STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleQuestgiverHelloOpcode);
    LEGACY_HANDLER(SMSG_QUESTGIVER_QUEST_LIST,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_QUESTGIVER_QUERY_QUEST,       STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleQuestgiverQueryQuestOpcode);
    LEGACY_HANDLER(CMSG_QUESTGIVER_QUEST_AUTOLAUNCH,  STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleQuestgiverQuestAutoLaunch);
    LEGACY_HANDLER(SMSG_QUESTGIVER_QUEST_DETAILS,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_QUESTGIVER_ACCEPT_QUEST,      STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleQuestgiverAcceptQuestOpcode);
    LEGACY_HANDLER(CMSG_QUESTGIVER_COMPLETE_QUEST,    STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleQuestgiverCompleteQuest);
    LEGACY_HANDLER(SMSG_QUESTGIVER_REQUEST_ITEMS,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_QUESTGIVER_REQUEST_REWARD,    STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleQuestgiverRequestRewardOpcode);
    LEGACY_HANDLER(SMSG_QUESTGIVER_OFFER_REWARD,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_QUESTGIVER_CHOOSE_REWARD,     STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleQuestgiverChooseRewardOpcode);
    LEGACY_HANDLER(SMSG_QUESTGIVER_QUEST_INVALID,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_QUESTGIVER_CANCEL,            STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleQuestgiverCancel);
    LEGACY_HANDLER(SMSG_QUESTGIVER_QUEST_COMPLETE,    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_QUESTGIVER_QUEST_FAILED,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_QUESTLOG_SWAP_QUEST,          STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleQuestLogSwapQuest);
    LEGACY_HANDLER(CMSG_QUESTLOG_REMOVE_QUEST,        STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleQuestLogRemoveQuest);
    LEGACY_HANDLER(SMSG_QUESTLOG_FULL,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_QUESTUPDATE_FAILED,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_QUESTUPDATE_FAILEDTIMER,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_QUESTUPDATE_COMPLETE,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_QUESTUPDATE_ADD_KILL,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_QUESTUPDATE_ADD_ITEM,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_QUEST_CONFIRM_ACCEPT,         STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleQuestConfirmAccept);
    LEGACY_HANDLER(SMSG_QUEST_CONFIRM_ACCEPT,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_PUSHQUESTTOPARTY,             STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePushQuestToParty);
    LEGACY_HANDLER(CMSG_LIST_INVENTORY,               STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleListInventoryOpcode);
    LEGACY_HANDLER(SMSG_LIST_INVENTORY,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_SELL_ITEM,                    STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleSellItemOpcode);
    LEGACY_HANDLER(SMSG_SELL_ITEM,                    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_BUY_ITEM,                     STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleBuyItemOpcode);
    LEGACY_HANDLER(CMSG_BUY_ITEM_IN_SLOT,             STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleBuyItemInSlotOpcode);
    LEGACY_HANDLER(SMSG_BUY_ITEM,                     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_BUY_FAILED,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_TAXICLEARALLNODES,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_TAXIENABLEALLNODES,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_TAXISHOWNODES,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_SHOWTAXINODES,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_TAXINODE_STATUS_QUERY,        STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleTaxiNodeStatusQueryOpcode);
    LEGACY_HANDLER(SMSG_TAXINODE_STATUS,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_TAXIQUERYAVAILABLENODES,      STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleTaxiQueryAvailableNodes);
    LEGACY_HANDLER(CMSG_ACTIVATETAXI,                 STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleActivateTaxiOpcode);
    LEGACY_HANDLER(SMSG_ACTIVATETAXIREPLY,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_NEW_TAXI_PATH,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_TRAINER_LIST,                 STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleTrainerListOpcode);
    LEGACY_HANDLER(SMSG_TRAINER_LIST,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_TRAINER_BUY_SPELL,            STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleTrainerBuySpellOpcode);
    LEGACY_HANDLER(SMSG_TRAINER_BUY_SUCCEEDED,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_TRAINER_BUY_FAILED,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_BINDER_ACTIVATE,              STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleBinderActivateOpcode);
    LEGACY_HANDLER(SMSG_PLAYERBINDERROR,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_BANKER_ACTIVATE,              STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleBankerActivateOpcode);
    LEGACY_HANDLER(SMSG_SHOW_BANK,                    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_BUY_BANK_SLOT,                STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleBuyBankSlotOpcode);
    LEGACY_HANDLER(SMSG_BUY_BANK_SLOT_RESULT,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_PETITION_SHOWLIST,            STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetitionShowListOpcode);
    LEGACY_HANDLER(SMSG_PETITION_SHOWLIST,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_PETITION_BUY,                 STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetitionBuyOpcode);
    LEGACY_HANDLER(CMSG_PETITION_SHOW_SIGNATURES,     STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetitionShowSignOpcode);
    LEGACY_HANDLER(SMSG_PETITION_SHOW_SIGNATURES,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_PETITION_SIGN,                STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetitionSignOpcode);
    LEGACY_HANDLER(SMSG_PETITION_SIGN_RESULTS,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_PETITION_DECLINE,              STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetitionDeclineOpcode);
    LEGACY_HANDLER(CMSG_OFFER_PETITION,               STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleOfferPetitionOpcode);
    LEGACY_HANDLER(CMSG_TURN_IN_PETITION,             STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleTurnInPetitionOpcode);
    LEGACY_HANDLER(SMSG_TURN_IN_PETITION_RESULTS,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_PETITION_QUERY,               STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetitionQueryOpcode);
    LEGACY_HANDLER(SMSG_PETITION_QUERY_RESPONSE,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_FISH_NOT_HOOKED,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_FISH_ESCAPED,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_BUG,                          STATUS_NEVER,     PACKET_PROCESS_MAP,           &WorldSession::HandleBugOpcode);
    LEGACY_HANDLER(SMSG_NOTIFICATION,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_PLAYED_TIME,                  STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePlayedTime);
    LEGACY_HANDLER(SMSG_PLAYED_TIME,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_QUERY_TIME,                   STATUS_LOGGEDIN,  PACKET_PROCESS_DB_QUERY,      &WorldSession::HandleQueryTimeOpcode);
    LEGACY_HANDLER(SMSG_QUERY_TIME_RESPONSE,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_LOG_XPGAIN,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    LEGACY_HANDLER(SMSG_AURACASTLOG,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#endif
    LEGACY_HANDLER(CMSG_RECLAIM_CORPSE,               STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleReclaimCorpseOpcode);
    LEGACY_HANDLER(CMSG_WRAP_ITEM,                    STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleWrapItemOpcode);
    LEGACY_HANDLER(SMSG_LEVELUP_INFO,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_MINIMAP_PING,                  STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleMinimapPingOpcode);
    LEGACY_HANDLER(SMSG_RESISTLOG,                    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_ENCHANTMENTLOG,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_SET_SKILL_CHEAT,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_START_MIRROR_TIMER,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PAUSE_MIRROR_TIMER,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_STOP_MIRROR_TIMER,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_PING,                         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_EarlyProccess);
    LEGACY_HANDLER(SMSG_PONG,                         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_CLEAR_COOLDOWN,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_GAMEOBJECT_PAGETEXT,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_SETSHEATHED,                  STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleSetSheathedOpcode);
    LEGACY_HANDLER(SMSG_COOLDOWN_CHEAT,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPELL_DELAYED,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_PLAYER_MACRO_OBSOLETE,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_PLAYER_MACRO_OBSOLETE,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GHOST,                        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GM_INVIS,                     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_INVALID_PROMOTION_CODE,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_GM_BIND_OTHER,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_GM_SUMMON,                     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_ITEM_TIME_UPDATE,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_ITEM_ENCHANT_TIME_UPDATE,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_AUTH_CHALLENGE,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_AUTH_SESSION,                 STATUS_NEVER,     PACKET_PROCESS_MAP,           &WorldSession::Handle_EarlyProccess);
    LEGACY_HANDLER(SMSG_AUTH_RESPONSE,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_GM_SHOWLABEL,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    LEGACY_HANDLER(CMSG_PET_CAST_SPELL,               STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetCastSpellOpcode);
#endif
    LEGACY_HANDLER(MSG_SAVE_GUILD_EMBLEM,             STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleSaveGuildEmblemOpcode);
    LEGACY_HANDLER(MSG_TABARDVENDOR_ACTIVATE,         STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleTabardVendorActivateOpcode);
    LEGACY_HANDLER(SMSG_PLAY_SPELL_VISUAL,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_ZONEUPDATE,                   STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleZoneUpdateOpcode);
    LEGACY_HANDLER(SMSG_PARTYKILLLOG,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_COMPRESSED_UPDATE_OBJECT,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    LEGACY_HANDLER(SMSG_PLAY_SPELL_IMPACT,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#endif
    LEGACY_HANDLER(SMSG_EXPLORATION_EXPERIENCE,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GM_SET_SECURITY_GROUP,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GM_NUKE,                      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_RANDOM_ROLL,                   STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleRandomRollOpcode);
    LEGACY_HANDLER(SMSG_ENVIRONMENTALDAMAGELOG,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_RWHOIS,                       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_RWHOIS,                       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_LOOKING_FOR_GROUP,             STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleLFGOpcode);
    LEGACY_HANDLER(CMSG_SET_LOOKING_FOR_GROUP,        STATUS_UNHANDLED, PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_UNLEARN_SPELL,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_UNLEARN_SKILL,                STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleUnlearnSkillOpcode);
    LEGACY_HANDLER(SMSG_REMOVED_SPELL,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_DECHARGE,                     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GMTICKET_CREATE,              STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleGMTicketCreateOpcode);
    LEGACY_HANDLER(SMSG_GMTICKET_CREATE,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GMTICKET_UPDATETEXT,          STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleGMTicketUpdateTextOpcode);
    LEGACY_HANDLER(SMSG_GMTICKET_UPDATETEXT,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_ACCOUNT_DATA_MD5,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_REQUEST_ACCOUNT_DATA,         STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleRequestAccountData);
    LEGACY_HANDLER(CMSG_UPDATE_ACCOUNT_DATA,          STATUS_LOGGEDIN_OR_RECENTLY_LOGGEDOUT, PACKET_PROCESS_WORLD,         &WorldSession::HandleUpdateAccountData);
    LEGACY_HANDLER(SMSG_UPDATE_ACCOUNT_DATA,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_CLEAR_FAR_SIGHT_IMMEDIATE,    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_POWERGAINLOG_OBSOLETE,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GM_TEACH,                     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GM_CREATE_ITEM_TARGET,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GMTICKET_GETTICKET,           STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleGMTicketGetTicketOpcode);
    LEGACY_HANDLER(SMSG_GMTICKET_GETTICKET,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_UNLEARN_TALENTS,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_GAMEOBJECT_SPAWN_ANIM,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_GAMEOBJECT_DESPAWN_ANIM,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_CORPSE_QUERY,                  STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleCorpseQueryOpcode);
    LEGACY_HANDLER(CMSG_GMTICKET_DELETETICKET,        STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleGMTicketDeleteTicketOpcode);
    LEGACY_HANDLER(SMSG_GMTICKET_DELETETICKET,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_CHAT_WRONG_FACTION,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GMTICKET_SYSTEMSTATUS,        STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleGMTicketSystemStatusOpcode);
    LEGACY_HANDLER(SMSG_GMTICKET_SYSTEMSTATUS,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
               // Can call convert corpse on different map.
    LEGACY_HANDLER(CMSG_SPIRIT_HEALER_ACTIVATE,       STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleSpiritHealerActivateOpcode);
    LEGACY_HANDLER(CMSG_SET_STAT_CHEAT,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_SET_REST_START,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_SKILL_BUY_STEP,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_SKILL_BUY_RANK,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_XP_CHEAT,                     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_SPIRIT_HEALER_CONFIRM,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_CHARACTER_POINT_CHEAT,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_GOSSIP_POI,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_CHAT_IGNORED,                 STATUS_LOGGEDIN,  PACKET_PROCESS_CHANNEL,       &WorldSession::HandleChatIgnoredOpcode);
    LEGACY_HANDLER(CMSG_GM_VISION,                    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_SERVER_COMMAND,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GM_SILENCE,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GM_REVEALTO,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GM_RESURRECT,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GM_SUMMONMOB,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GM_MOVECORPSE,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GM_FREEZE,                    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GM_UBERINVIS,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GM_REQUEST_PLAYER_INFO,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_GM_PLAYER_INFO,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GUILD_RANK,                   STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildRankOpcode);
    LEGACY_HANDLER(CMSG_GUILD_ADD_RANK,               STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildAddRankOpcode);
    LEGACY_HANDLER(CMSG_GUILD_DEL_RANK,               STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildDelRankOpcode);
    LEGACY_HANDLER(CMSG_GUILD_SET_PUBLIC_NOTE,        STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildSetPublicNoteOpcode);
    LEGACY_HANDLER(CMSG_GUILD_SET_OFFICER_NOTE,       STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildSetOfficerNoteOpcode);
    LEGACY_HANDLER(SMSG_LOGIN_VERIFY_WORLD,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_CLEAR_EXPLORATION,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_SEND_MAIL,                    STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleSendMail);
    LEGACY_HANDLER(SMSG_SEND_MAIL_RESULT,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GET_MAIL_LIST,                STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleGetMailList);
    LEGACY_HANDLER(SMSG_MAIL_LIST_RESULT,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_BATTLEFIELD_LIST,             STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleBattlefieldListOpcode);
    LEGACY_HANDLER(SMSG_BATTLEFIELD_LIST,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_BATTLEFIELD_JOIN,             STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleBattlefieldJoinOpcode);
    LEGACY_HANDLER(SMSG_BATTLEFIELD_WIN,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_BATTLEFIELD_LOSE,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_TAXICLEARNODE,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_TAXIENABLENODE,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_ITEM_TEXT_QUERY,              STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleItemTextQuery);
    LEGACY_HANDLER(SMSG_ITEM_TEXT_QUERY_RESPONSE,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_MAIL_TAKE_MONEY,              STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleMailTakeMoney);
    LEGACY_HANDLER(CMSG_MAIL_TAKE_ITEM,               STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD, /*COD*/ &WorldSession::HandleMailTakeItem);
    LEGACY_HANDLER(CMSG_MAIL_MARK_AS_READ,            STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleMailMarkAsRead);
    LEGACY_HANDLER(CMSG_MAIL_RETURN_TO_SENDER,        STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleMailReturnToSender);
    LEGACY_HANDLER(CMSG_MAIL_DELETE,                  STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleMailDelete);
    LEGACY_HANDLER(CMSG_MAIL_CREATE_TEXT_ITEM,        STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleMailCreateTextItem);
    LEGACY_HANDLER(SMSG_SPELLLOGMISS,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPELLLOGEXECUTE,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_DEBUGAURAPROC,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PERIODICAURALOG,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPELLDAMAGESHIELD,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPELLNONMELEEDAMAGELOG,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_LEARN_TALENT,                 STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleLearnTalentOpcode);
    LEGACY_HANDLER(SMSG_RESURRECT_FAILED,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_TOGGLE_PVP,                   STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleTogglePvP);
    LEGACY_HANDLER(SMSG_ZONE_UNDER_ATTACK,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_AUCTION_HELLO,                 STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleAuctionHelloOpcode);
    LEGACY_HANDLER(CMSG_AUCTION_SELL_ITEM,            STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleAuctionSellItem);
    LEGACY_HANDLER(CMSG_AUCTION_REMOVE_ITEM,          STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleAuctionRemoveItem);
    LEGACY_HANDLER(CMSG_AUCTION_LIST_ITEMS,           STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleAuctionListItems);
    LEGACY_HANDLER(CMSG_AUCTION_LIST_OWNER_ITEMS,     STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleAuctionListOwnerItems);
    LEGACY_HANDLER(CMSG_AUCTION_PLACE_BID,            STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleAuctionPlaceBid);
    LEGACY_HANDLER(SMSG_AUCTION_COMMAND_RESULT,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_AUCTION_LIST_RESULT,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_AUCTION_OWNER_LIST_RESULT,    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_AUCTION_BIDDER_NOTIFICATION,  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_AUCTION_OWNER_NOTIFICATION,   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PROCRESIST,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_STANDSTATE_CHANGE_FAILURE,    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_DISPEL_FAILED,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPELLORDAMAGE_IMMUNE,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_AUCTION_LIST_BIDDER_ITEMS,    STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleAuctionListBidderItems);
    LEGACY_HANDLER(SMSG_AUCTION_BIDDER_LIST_RESULT,   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SET_FLAT_SPELL_MODIFIER,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SET_PCT_SPELL_MODIFIER,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_SET_AMMO,                     STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleSetAmmoOpcode);
    LEGACY_HANDLER(SMSG_CORPSE_RECLAIM_DELAY,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_SET_ACTIVE_MOVER,             STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleSetActiveMoverOpcode);
    LEGACY_HANDLER(CMSG_PET_CANCEL_AURA,              STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetCancelAuraOpcode);
    LEGACY_HANDLER(CMSG_PLAYER_AI_CHEAT,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_CANCEL_AUTO_REPEAT_SPELL,     STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleCancelAutoRepeatSpellOpcode);
    LEGACY_HANDLER(MSG_GM_ACCOUNT_ONLINE,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_LIST_STABLED_PETS,             STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleListStabledPetsOpcode);
    LEGACY_HANDLER(CMSG_STABLE_PET,                   STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleStablePet);
    LEGACY_HANDLER(CMSG_UNSTABLE_PET,                 STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleUnstablePet);
    LEGACY_HANDLER(CMSG_BUY_STABLE_SLOT,              STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleBuyStableSlot);
    LEGACY_HANDLER(SMSG_STABLE_RESULT,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_STABLE_REVIVE_PET,            STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleStableRevivePet);
    LEGACY_HANDLER(CMSG_STABLE_SWAP_PET,              STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleStableSwapPet);
    LEGACY_HANDLER(MSG_QUEST_PUSH_RESULT,             STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleQuestPushResult);
    LEGACY_HANDLER(SMSG_PLAY_MUSIC,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PLAY_OBJECT_SOUND,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_REQUEST_PET_INFO,             STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleRequestPetInfoOpcode);
    LEGACY_HANDLER(CMSG_FAR_SIGHT,                    STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleFarSightOpcode);
    LEGACY_HANDLER(SMSG_SPELLDISPELLOG,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_DAMAGE_CALC_LOG,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_ENABLE_DAMAGE_LOG,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GROUP_CHANGE_SUB_GROUP,       STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleGroupChangeSubGroupOpcode);
    LEGACY_HANDLER(CMSG_REQUEST_PARTY_MEMBER_STATS,   STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleRequestPartyMemberStatsOpcode);
    LEGACY_HANDLER(CMSG_GROUP_SWAP_SUB_GROUP,         STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleGroupSwapSubGroupOpcode);
    LEGACY_HANDLER(CMSG_RESET_FACTION_CHEAT,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_AUTOSTORE_BANK_ITEM,          STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleAutoStoreBankItemOpcode);
    LEGACY_HANDLER(CMSG_AUTOBANK_ITEM,                STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleAutoBankItemOpcode);
    LEGACY_HANDLER(MSG_QUERY_NEXT_MAIL_TIME,          STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleQueryNextMailTime);
    LEGACY_HANDLER(SMSG_RECEIVED_MAIL,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_RAID_GROUP_ONLY,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_SET_DURABILITY_CHEAT,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_SET_PVP_RANK_CHEAT,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_ADD_PVP_MEDAL_CHEAT,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_DEL_PVP_MEDAL_CHEAT,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_SET_PVP_TITLE,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_PVP_CREDIT,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_AUCTION_REMOVED_NOTIFICATION, STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GROUP_RAID_CONVERT,           STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleGroupRaidConvertOpcode);
    LEGACY_HANDLER(CMSG_GROUP_ASSISTANT_LEADER,       STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleGroupAssistantLeaderOpcode);
    LEGACY_HANDLER(CMSG_BUYBACK_ITEM,                 STATUS_LOGGEDIN,  PACKET_PROCESS_SELF_ITEMS,    &WorldSession::HandleBuybackItem);
    LEGACY_HANDLER(SMSG_SERVER_MESSAGE,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_MEETINGSTONE_JOIN,            STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleMeetingStoneJoinOpcode);
    LEGACY_HANDLER(CMSG_MEETINGSTONE_LEAVE,           STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleMeetingStoneLeaveOpcode);
    LEGACY_HANDLER(CMSG_MEETINGSTONE_CHEAT,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_MEETINGSTONE_SETQUEUE,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_MEETINGSTONE_INFO,            STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleMeetingStoneInfoOpcode);
    LEGACY_HANDLER(SMSG_MEETINGSTONE_COMPLETE,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_MEETINGSTONE_IN_PROGRESS,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_MEETINGSTONE_MEMBER_ADDED,    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GMTICKETSYSTEM_TOGGLE,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_CANCEL_GROWTH_AURA,           STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleCancelGrowthAuraOpcode);
    LEGACY_HANDLER(SMSG_CANCEL_AUTO_REPEAT,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_STANDSTATE_UPDATE,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_LOOT_ALL_PASSED,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_LOOT_ROLL_WON,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_LOOT_ROLL,                    STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleLootRoll);
    LEGACY_HANDLER(SMSG_LOOT_START_ROLL,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_LOOT_ROLL,                    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_LOOT_MASTER_GIVE,             STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleLootMasterGiveOpcode);
    LEGACY_HANDLER(SMSG_LOOT_MASTER_LIST,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SET_FORCED_REACTIONS,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPELL_FAILED_OTHER,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_GAMEOBJECT_RESET_STATE,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_REPAIR_ITEM,                  STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleRepairItemOpcode);
    LEGACY_HANDLER(SMSG_CHAT_PLAYER_NOT_FOUND,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_TALENT_WIPE_CONFIRM,           STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleTalentWipeConfirmOpcode);
    LEGACY_HANDLER(SMSG_SUMMON_REQUEST,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_SUMMON_RESPONSE,              STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleSummonResponseOpcode);
    LEGACY_HANDLER(MSG_MOVE_TOGGLE_GRAVITY_CHEAT,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_MONSTER_MOVE_TRANSPORT,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PET_BROKEN,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_MOVE_FEATHER_FALL,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_MOVE_WATER_WALK,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_SERVER_BROADCAST,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_SELF_RES,                     STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleSelfResOpcode);
    LEGACY_HANDLER(SMSG_FEIGN_DEATH_RESISTED,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_RUN_SCRIPT,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_SCRIPT_MESSAGE,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_DUEL_COUNTDOWN,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_AREA_TRIGGER_MESSAGE,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_TOGGLE_HELM,                  STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleShowingHelmOpcode);
    LEGACY_HANDLER(CMSG_TOGGLE_CLOAK,                 STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleShowingCloakOpcode);
    LEGACY_HANDLER(SMSG_MEETINGSTONE_JOINFAILED,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PLAYER_SKINNED,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_DURABILITY_DAMAGE_DEATH,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_SET_EXPLORATION,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_SET_ACTIONBAR_TOGGLES,        STATUS_AUTHED,    PACKET_PROCESS_MAP,           &WorldSession::HandleSetActionBarTogglesOpcode);
    LEGACY_HANDLER(MSG_DELETE_GUILD_CHARTER,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_PETITION_RENAME,               STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetitionRenameOpcode);
    LEGACY_HANDLER(SMSG_INIT_WORLD_STATES,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_UPDATE_WORLD_STATE,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_ITEM_NAME_QUERY,              STATUS_LOGGEDIN,  PACKET_PROCESS_DB_QUERY,      &WorldSession::HandleItemNameQueryOpcode);
    LEGACY_HANDLER(SMSG_ITEM_NAME_QUERY_RESPONSE,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PET_ACTION_FEEDBACK,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_CHAR_RENAME,                  STATUS_AUTHED,    PACKET_PROCESS_WORLD,         &WorldSession::HandleCharRenameOpcode);
    LEGACY_HANDLER(SMSG_CHAR_RENAME,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_MOVE_SPLINE_DONE,             STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMoveSplineDoneOpcode);
    LEGACY_HANDLER(CMSG_MOVE_FALL_RESET,              STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementOpcodes);
    LEGACY_HANDLER(SMSG_INSTANCE_SAVE_CREATED,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_RAID_INSTANCE_INFO,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_REQUEST_RAID_INFO,            STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleRequestRaidInfoOpcode);
    LEGACY_HANDLER(CMSG_MOVE_TIME_SKIPPED,            STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMoveTimeSkippedOpcode);
    LEGACY_HANDLER(CMSG_MOVE_FEATHER_FALL_ACK,        STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementFlagChangeToggleAck);
    LEGACY_HANDLER(CMSG_MOVE_WATER_WALK_ACK,          STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMovementFlagChangeToggleAck);
    LEGACY_HANDLER(CMSG_MOVE_NOT_ACTIVE_MOVER,        STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleMoveNotActiveMoverOpcode);
    LEGACY_HANDLER(SMSG_PLAY_SOUND,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_BATTLEFIELD_STATUS,           STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleBattlefieldStatusOpcode);
    LEGACY_HANDLER(SMSG_BATTLEFIELD_STATUS,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_BATTLEFIELD_PORT,             STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleBattleFieldPortOpcode);
    LEGACY_HANDLER(MSG_INSPECT_HONOR_STATS,           STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleInspectHonorStatsOpcode);
    LEGACY_HANDLER(CMSG_BATTLEMASTER_HELLO,           STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleBattlemasterHelloOpcode);
    LEGACY_HANDLER(CMSG_MOVE_START_SWIM_CHEAT,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_MOVE_STOP_SWIM_CHEAT,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_FORCE_WALK_SPEED_CHANGE,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_FORCE_WALK_SPEED_CHANGE_ACK,  STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleForceSpeedChangeAckOpcodes);
    LEGACY_HANDLER(SMSG_FORCE_SWIM_BACK_SPEED_CHANGE, STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,     &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_FORCE_SWIM_BACK_SPEED_CHANGE_ACK,  STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,           &WorldSession::HandleForceSpeedChangeAckOpcodes);
    LEGACY_HANDLER(SMSG_FORCE_TURN_RATE_CHANGE,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_FORCE_TURN_RATE_CHANGE_ACK,   STATUS_LOGGEDIN,  PACKET_PROCESS_MOVEMENT,      &WorldSession::HandleForceSpeedChangeAckOpcodes);
    LEGACY_HANDLER(MSG_PVP_LOG_DATA,                  STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePVPLogDataOpcode);
    LEGACY_HANDLER(CMSG_LEAVE_BATTLEFIELD,            STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleLeaveBattlefieldOpcode);
    LEGACY_HANDLER(CMSG_AREA_SPIRIT_HEALER_QUERY,     STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleAreaSpiritHealerQueryOpcode);
    LEGACY_HANDLER(CMSG_AREA_SPIRIT_HEALER_QUEUE,     STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleAreaSpiritHealerQueueOpcode);
    LEGACY_HANDLER(SMSG_AREA_SPIRIT_HEALER_TIME,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GM_UNTEACH,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_5_1
    LEGACY_HANDLER(SMSG_WARDEN_DATA,                  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_WARDEN_DATA,                  STATUS_AUTHED,    PACKET_PROCESS_WORLD,         &WorldSession::HandleWardenDataOpcode);
#endif
    LEGACY_HANDLER(SMSG_GROUP_JOINED_BATTLEGROUND,    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_BATTLEGROUND_PLAYER_POSITIONS, STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,          &WorldSession::HandleBattleGroundPlayerPositionsOpcode);
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_5_1
    LEGACY_HANDLER(SMSG_PARTY_MEMBER_STATS_FULL,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#endif
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_6_1
    LEGACY_HANDLER(CMSG_PET_STOP_ATTACK,              STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetStopAttack);
    LEGACY_HANDLER(SMSG_BINDER_CONFIRM,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_BATTLEGROUND_PLAYER_JOINED,   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_BATTLEGROUND_PLAYER_LEFT,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_BATTLEMASTER_JOIN,            STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleBattlemasterJoinOpcode);
    LEGACY_HANDLER(SMSG_ADDON_INFO,                   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_PET_UNLEARN,                  STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetUnlearnOpcode);
    LEGACY_HANDLER(SMSG_PET_UNLEARN_CONFIRM,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_PET_SPELL_AUTOCAST,           STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandlePetSpellAutocastOpcode);
#endif
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_7_1
    LEGACY_HANDLER(SMSG_WEATHER,                      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PLAY_TIME_WARNING,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_MINIGAME_SETUP,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_MINIGAME_STATE,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_MINIGAME_MOVE,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_MINIGAME_MOVE_FAILED,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_RAID_INSTANCE_MESSAGE,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_COMPRESSED_MOVES,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#endif
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_8_4
    LEGACY_HANDLER(CMSG_GUILD_INFO_TEXT,              STATUS_LOGGEDIN,  PACKET_PROCESS_GUILD,         &WorldSession::HandleGuildChangeInfoTextOpcode);
    LEGACY_HANDLER(SMSG_CHAT_RESTRICTED,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_SET_RUN_SPEED,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_SET_RUN_BACK_SPEED,    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_SET_SWIM_SPEED,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_SET_WALK_SPEED,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_SET_SWIM_BACK_SPEED,   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_SET_TURN_RATE,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_MOVE_UNROOT,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_MOVE_FEATHER_FALL,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_MOVE_NORMAL_FALL,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_MOVE_SET_HOVER,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_MOVE_UNSET_HOVER,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_MOVE_WATER_WALK,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_MOVE_LAND_WALK,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_MOVE_START_SWIM,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_MOVE_STOP_SWIM,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_MOVE_SET_RUN_MODE,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPLINE_MOVE_SET_WALK_MODE,    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#endif
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_9_4
    LEGACY_HANDLER(CMSG_GM_NUKE_ACCOUNT,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(MSG_GM_DESTROY_CORPSE,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GM_DESTROY_ONLINE_CORPSE,     STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_ACTIVATETAXIEXPRESS,          STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleActivateTaxiExpressOpcode);
    LEGACY_HANDLER(SMSG_SET_FACTION_ATWAR,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_GAMETIMEBIAS_SET,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_DEBUG_ACTIONS_START,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_DEBUG_ACTIONS_STOP,           STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_SET_FACTION_INACTIVE,         STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleSetFactionInactiveOpcode);
    LEGACY_HANDLER(CMSG_SET_WATCHED_FACTION,          STATUS_LOGGEDIN,  PACKET_PROCESS_MAP,           &WorldSession::HandleSetWatchedFactionOpcode);
    LEGACY_HANDLER(MSG_MOVE_TIME_SKIPPED,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_SPLINE_MOVE_ROOT,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_SET_EXPLORATION_ALL,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_INVALIDATE_PLAYER,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#endif
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_10_2
    LEGACY_HANDLER(CMSG_RESET_INSTANCES,              STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleResetInstancesOpcode);
    LEGACY_HANDLER(SMSG_INSTANCE_RESET,               STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_INSTANCE_RESET_FAILED,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_UPDATE_LAST_INSTANCE,         STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_RAID_TARGET_UPDATE,            STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleRaidTargetUpdateOpcode);
    LEGACY_HANDLER(MSG_RAID_READY_CHECK,              STATUS_LOGGEDIN,  PACKET_PROCESS_GROUP,         &WorldSession::HandleRaidReadyCheckOpcode);
    LEGACY_HANDLER(CMSG_LUA_USAGE,                    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_PET_ACTION_SOUND,             STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_PET_DISMISS_SOUND,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_GHOSTEE_GONE,                 STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_GM_UPDATE_TICKET_STATUS,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_GM_TICKET_STATUS_UPDATE,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_SET_DUNGEON_DIFFICULTY,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(CMSG_GMSURVEY_SUBMIT,              STATUS_LOGGEDIN,  PACKET_PROCESS_WORLD,         &WorldSession::HandleGMSurveySubmitOpcode);
    LEGACY_HANDLER(SMSG_UPDATE_INSTANCE_OWNERSHIP,    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_IGNORE_KNOCKBACK_CHEAT,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_CHAT_PLAYER_AMBIGUOUS,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_DELAY_GHOST_TELEPORT,          STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
#endif
#if SUPPORTED_CLIENT_BUILD > CLIENT_BUILD_1_11_2
    LEGACY_HANDLER(SMSG_SPELLINSTAKILLLOG,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPELL_UPDATE_CHAIN_TARGETS,   STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_CHAT_FILTERED,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_EXPECTED_SPAM_RECORDS,        STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_SPELLSTEALLOG,                STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_LOTTERY_QUERY_OBSOLETE,       STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_LOTTERY_QUERY_RESULT_OBSOLETE, STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(CMSG_BUY_LOTTERY_TICKET_OBSOLETE,  STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_LOTTERY_RESULT_OBSOLETE,      STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_CHARACTER_PROFILE,            STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(SMSG_CHARACTER_PROFILE_REALM_CONNECTED, STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
    LEGACY_HANDLER(MSG_UNUSED_826,                    STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_NULL);
    LEGACY_HANDLER(SMSG_DEFENSE_MESSAGE,              STATUS_NEVER,     PACKET_PROCESS_MAX_TYPE,      &WorldSession::Handle_ServerSide);
#endif

    // Throw compile time error when a handler is not initialized
    for (int i = 0; i < NUM_MSG_TYPES; i++)
    {
        if (list.handlers[i].name == nullptr)
        {
            throw "One or more packet handlers are not initialized";
        }
    }

    return list;
}

// If you get "Constexpr variable 'handlerList' must be initialized by a constant expression"
// you have to verify that every MSG type has a handler.
constexpr Handlers handlerList = BuildOpcodeList();

constexpr OpcodeHandler emptyHandler = {
    "<none>",
    STATUS_UNHANDLED,
    PACKET_PROCESS_MAX_TYPE,
    &WorldSession::Handle_NULL
};

OpcodeHandler const& LookupOpcodeHandler(uint16 id)
{
    if (id >= NUM_MSG_TYPES)
        return emptyHandler;
    return handlerList.handlers[id];
}

char const* LookupOpcodeName(uint16 id)
{
    if (id >= NUM_MSG_TYPES)
        return "Received unknown opcode, it's more than max!";
    return handlerList.handlers[id].name;
}
