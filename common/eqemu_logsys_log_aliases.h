/**
 * EQEmulator: Everquest Server Emulator
 * Copyright (C) 2001-2019 EQEmulator Development Team (https://github.com/EQEmu/Server)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY except by those people which sell it, which
 * are required to give you total support for your newly bought product;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
*/

#ifndef EQEMU_EQEMU_LOGSYS_LOG_ALIASES_H
#define EQEMU_EQEMU_LOGSYS_LOG_ALIASES_H

#ifdef BUILD_LOGGING

/**
 * RFC 5424
 */

#define LogEmergency(message, ...) do {\
    if (LogSys.log_settings[Logs::Emergency].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Emergency, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogAlert(message, ...) do {\
    if (LogSys.log_settings[Logs::Alert].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Alert, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogCritical(message, ...) do {\
    if (LogSys.log_settings[Logs::Critical].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Critical, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogError(message, ...) do {\
    if (LogSys.log_settings[Logs::Error].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Error, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogWarning(message, ...) do {\
    if (LogSys.log_settings[Logs::Warning].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Warning, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogNotice(message, ...) do {\
    if (LogSys.log_settings[Logs::Notice].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Notice, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogInfo(message, ...) do {\
    if (LogSys.log_settings[Logs::Info].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Info, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogDebug(message, ...) do {\
    if (LogSys.log_settings[Logs::Debug].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Debug, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogAA(message, ...) do {\
    if (LogSys.log_settings[Logs::AA].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::AA, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogAADetail(message, ...) do {\
    if (LogSys.log_settings[Logs::AA].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::AA, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogAI(message, ...) do {\
    if (LogSys.log_settings[Logs::AI].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::AI, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogAIDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::AI].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::AI, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogAggro(message, ...) do {\
    if (LogSys.log_settings[Logs::Aggro].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Aggro, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogAggroDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Aggro].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Aggro, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogAttack(message, ...) do {\
    if (LogSys.log_settings[Logs::Attack].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Attack, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogAttackDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Attack].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Attack, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogPacketClientServer(message, ...) do {\
    if (LogSys.log_settings[Logs::PacketClientServer].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::PacketClientServer, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogPacketClientServerDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::PacketClientServer].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::PacketClientServer, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogCombat(message, ...) do {\
    if (LogSys.log_settings[Logs::Combat].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Combat, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogCombatDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Combat].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Combat, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogCommands(message, ...) do {\
    if (LogSys.log_settings[Logs::Commands].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Commands, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogCommandsDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Commands].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Commands, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogCrash(message, ...) do {\
    if (LogSys.log_settings[Logs::Crash].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Crash, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogCrashDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Crash].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Crash, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogDoors(message, ...) do {\
    if (LogSys.log_settings[Logs::Doors].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Doors, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogDoorsDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Doors].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Doors, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogGuilds(message, ...) do {\
    if (LogSys.log_settings[Logs::Guilds].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Guilds, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogGuildsDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Guilds].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Guilds, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogInventory(message, ...) do {\
    if (LogSys.log_settings[Logs::Inventory].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Inventory, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogInventoryDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Inventory].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Inventory, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogLauncher(message, ...) do {\
    if (LogSys.log_settings[Logs::Launcher].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Launcher, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogLauncherDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Launcher].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Launcher, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogNetcode(message, ...) do {\
    if (LogSys.log_settings[Logs::Netcode].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Netcode, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogNetcodeDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Netcode].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Netcode, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogNormal(message, ...) do {\
    if (LogSys.log_settings[Logs::Normal].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Normal, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogNormalDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Normal].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Normal, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogObject(message, ...) do {\
    if (LogSys.log_settings[Logs::Object].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Object, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogObjectDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Object].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Object, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogPathing(message, ...) do {\
    if (LogSys.log_settings[Logs::Pathing].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Pathing, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogPathingDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Pathing].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Pathing, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogQSServer(message, ...) do {\
    if (LogSys.log_settings[Logs::QSServer].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::QSServer, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogQSServerDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::QSServer].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::QSServer, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogQuests(message, ...) do {\
    if (LogSys.log_settings[Logs::Quests].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Quests, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogQuestsDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Quests].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Quests, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogRules(message, ...) do {\
    if (LogSys.log_settings[Logs::Rules].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Rules, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogRulesDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Rules].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Rules, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogSkills(message, ...) do {\
    if (LogSys.log_settings[Logs::Skills].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Skills, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogSkillsDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Skills].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Skills, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogSpawns(message, ...) do {\
    if (LogSys.log_settings[Logs::Spawns].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Spawns, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogSpawnsDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Spawns].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Spawns, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogSpells(message, ...) do {\
    if (LogSys.log_settings[Logs::Spells].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Spells, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogSpellsDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Spells].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Spells, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogTCPConnection(message, ...) do {\
    if (LogSys.log_settings[Logs::TCPConnection].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::TCPConnection, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogTCPConnectionDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::TCPConnection].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::TCPConnection, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogTasks(message, ...) do {\
    if (LogSys.log_settings[Logs::Tasks].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Tasks, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogTasksDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Tasks].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Tasks, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogTradeskills(message, ...) do {\
    if (LogSys.log_settings[Logs::Tradeskills].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Tradeskills, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogTradeskillsDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Tradeskills].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Tradeskills, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogTrading(message, ...) do {\
    if (LogSys.log_settings[Logs::Trading].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Trading, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogTradingDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Trading].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Trading, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogMySQLError(message, ...) do {\
    if (LogSys.log_settings[Logs::MySQLError].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::MySQLError, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogMySQLErrorDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::MySQLError].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::MySQLError, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogMySQLQuery(message, ...) do {\
    if (LogSys.log_settings[Logs::MySQLQuery].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::MySQLQuery, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogMySQLQueryDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::MySQLQuery].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::MySQLQuery, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogEQMac(message, ...) do {\
    if (LogSys.log_settings[Logs::EQMac].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::EQMac, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogEQMacDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::EQMac].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::EQMac, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogQuestDebug(message, ...) do {\
    if (LogSys.log_settings[Logs::QuestDebug].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::QuestDebug, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogQuestDebugDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::QuestDebug].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::QuestDebug, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogLoginserver(message, ...) do {\
    if (LogSys.log_settings[Logs::Loginserver].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Loginserver, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogLoginserverDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Loginserver].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Loginserver, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogTraps(message, ...) do {\
    if (LogSys.log_settings[Logs::Traps].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Traps, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogTrapsDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Traps].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Traps, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogStatus(message, ...) do {\
    if (LogSys.log_settings[Logs::Status].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Status, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogStatusDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Status].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Status, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogZoneServer(message, ...) do {\
    if (LogSys.log_settings[Logs::ZoneServer].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::ZoneServer, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogZoneServerDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::ZoneServer].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::ZoneServer, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogMaps(message, ...) do {\
    if (LogSys.log_settings[Logs::Maps].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Maps, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogMapsDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Maps].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Maps, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogFaction(message, ...) do {\
    if (LogSys.log_settings[Logs::Faction].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Faction, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogFactionDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Faction].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Faction, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogCharacter(message, ...) do {\
    if (LogSys.log_settings[Logs::Character].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Character, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogCharacterDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Character].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Character, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogGroup(message, ...) do {\
    if (LogSys.log_settings[Logs::Group].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Group, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogGroupDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Group].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Group, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogCorpse(message, ...) do {\
    if (LogSys.log_settings[Logs::Corpse].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Corpse, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogCorpseDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Corpse].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Corpse, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogBazaar(message, ...) do {\
    if (LogSys.log_settings[Logs::Bazaar].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Bazaar, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogBazaarDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Bazaar].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Bazaar, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogDisc(message, ...) do {\
    if (LogSys.log_settings[Logs::Discs].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Discs, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogDiscDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Discs].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Discs, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogBoats(message, ...) do {\
    if (LogSys.log_settings[Logs::Boats].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Boats, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogBoatsDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Boats].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Boats, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogPTimers(message, ...) do {\
    if (LogSys.log_settings[Logs::PTimers].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::PTimers, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogPTimersDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::PTimers].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::PTimers, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogNexus(message, ...) do {\
    if (LogSys.log_settings[Logs::Nexus].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Nexus, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogNexusDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Nexus].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Nexus, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogPets(message, ...) do {\
    if (LogSys.log_settings[Logs::Pets].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Pets, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogPetsDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Pets].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Pets, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogRegen(message, ...) do {\
    if (LogSys.log_settings[Logs::Regen].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Regen, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogRegenDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Regen].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Regen, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogFocus(message, ...) do {\
    if (LogSys.log_settings[Logs::Focus].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Focus, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogFoscusDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Focus].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Focus, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogDeath(message, ...) do {\
    if (LogSys.log_settings[Logs::Death].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Death, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogDeathDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Death].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Death, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogQuestErrors(message, ...) do {\
    if (LogSys.log_settings[Logs::QuestErrors].is_category_enabled = 1)\
        OutF(LogSys, Logs::General, Logs::QuestErrors, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogQuestErrorsDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::QuestErrors].is_category_enabled = 1)\
        OutF(LogSys, Logs::Detail, Logs::QuestErrors, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogScheduler(message, ...) do {\
    if (LogSys.log_settings[Logs::Scheduler].is_category_enabled == 1)\
        OutF(LogSys, Logs::General, Logs::Scheduler, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogSchedulerDetail(message, ...) do {\
    if (LogSys.log_settings[Logs::Scheduler].is_category_enabled == 1)\
        OutF(LogSys, Logs::Detail, Logs::Scheduler, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define Log(debug_level, log_category, message, ...) do {\
    if (LogSys.log_settings[log_category].is_category_enabled == 1)\
        LogSys.Out(debug_level, log_category, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#define LogF(debug_level, log_category, message, ...) do {\
    if (LogSys.log_settings[log_category].is_category_enabled == 1)\
        OutF(LogSys, debug_level, log_category, __FILE__, __func__, __LINE__, message, ##__VA_ARGS__);\
} while (0)

#else
#define LogEmergency(message, ...) do {\
} while (0)

#define LogAlert(message, ...) do {\
} while (0)

#define LogCritical(message, ...) do {\
} while (0)

#define LogError(message, ...) do {\
} while (0)

#define LogWarning(message, ...) do {\
} while (0)

#define LogNotice(message, ...) do {\
} while (0)

#define LogInfo(message, ...) do {\
} while (0)

#define LogDebug(message, ...) do {\
} while (0)

#define LogAA(message, ...) do {\
} while (0)

#define LogAADetail(message, ...) do {\
} while (0)

#define LogAI(message, ...) do {\
} while (0)

#define LogAIDetail(message, ...) do {\
} while (0)

#define LogAggro(message, ...) do {\
} while (0)

#define LogAggroDetail(message, ...) do {\
} while (0)

#define LogAttack(message, ...) do {\
} while (0)

#define LogAttackDetail(message, ...) do {\
} while (0)

#define LogPacketClientServer(message, ...) do {\
} while (0)

#define LogPacketClientServerDetail(message, ...) do {\
} while (0)

#define LogCombat(message, ...) do {\
} while (0)

#define LogCombatDetail(message, ...) do {\
} while (0)

#define LogCommands(message, ...) do {\
} while (0)

#define LogCommandsDetail(message, ...) do {\
} while (0)

#define LogCrash(message, ...) do {\
} while (0)

#define LogCrashDetail(message, ...) do {\
} while (0)

#define LogDoors(message, ...) do {\
} while (0)

#define LogDoorsDetail(message, ...) do {\
} while (0)

#define LogGuilds(message, ...) do {\
} while (0)

#define LogGuildsDetail(message, ...) do {\
 } while (0)

#define LogInventory(message, ...) do {\
} while (0)

#define LogInventoryDetail(message, ...) do {\
} while (0)

#define LogLauncher(message, ...) do {\
} while (0)

#define LogLauncherDetail(message, ...) do {\
} while (0)

#define LogNetcode(message, ...) do {\
} while (0)

#define LogNetcodeDetail(message, ...) do {\
} while (0)

#define LogNormal(message, ...) do {\
} while (0)

#define LogNormalDetail(message, ...) do {\
} while (0)

#define LogObject(message, ...) do {\
} while (0)

#define LogObjectDetail(message, ...) do {\
} while (0)

#define LogPathing(message, ...) do {\
} while (0)

#define LogPathingDetail(message, ...) do {\
} while (0)

#define LogQSServer(message, ...) do {\
} while (0)

#define LogQSServerDetail(message, ...) do {\
} while (0)

#define LogQuests(message, ...) do {\
} while (0)

#define LogQuestsDetail(message, ...) do {\
} while (0)

#define LogRules(message, ...) do {\
} while (0)

#define LogRulesDetail(message, ...) do {\
} while (0)

#define LogSkills(message, ...) do {\
} while (0)

#define LogSkillsDetail(message, ...) do {\
} while (0)

#define LogSpawns(message, ...) do {\
} while (0)

#define LogSpawnsDetail(message, ...) do {\
} while (0)

#define LogSpells(message, ...) do {\
} while (0)

#define LogSpellsDetail(message, ...) do {\
} while (0)

#define LogTCPConnection(message, ...) do {\
} while (0)

#define LogTCPConnectionDetail(message, ...) do {\
} while (0)

#define LogTasks(message, ...) do {\
} while (0)

#define LogTasksDetail(message, ...) do {\
} while (0)

#define LogTradeskills(message, ...) do {\
} while (0)

#define LogTradeskillsDetail(message, ...) do {\
} while (0)

#define LogTrading(message, ...) do {\
} while (0)

#define LogTradingDetail(message, ...) do {\
} while (0)

#define LogMySQLError(message, ...) do {\
} while (0)

#define LogMySQLErrorDetail(message, ...) do {\
} while (0)

#define LogMySQLQuery(message, ...) do {\
} while (0)

#define LogMySQLQueryDetail(message, ...) do {\
} while (0)

#define LogEQMac(message, ...) do {\
} while (0)

#define LogEQMacDetail(message, ...) do {\
} while (0)

#define LogQuestDebug(message, ...) do {\
} while (0)

#define LogQuestDebugDetail(message, ...) do {\
} while (0)

#define LogLoginserver(message, ...) do {\
} while (0)

#define LogLoginserverDetail(message, ...) do {\
} while (0)

#define LogTraps(message, ...) do {\
} while (0)

#define LogTrapsDetail(message, ...) do {\
} while (0)

#define LogStatus(message, ...) do {\
} while (0)

#define LogStatusDetail(message, ...) do {\
} while (0)

#define LogZoneServer(message, ...) do {\
} while (0)

#define LogZoneServerDetail(message, ...) do {\
} while (0)

#define LogMaps(message, ...) do {\
} while (0)

#define LogMapsDetail(message, ...) do {\
} while (0)

#define LogFaction(message, ...) do {\
} while (0)

#define LogFactionDetail(message, ...) do {\
} while (0)

#define LogCharacter(message, ...) do {\
} while (0)

#define LogCharacterDetail(message, ...) do {\
} while (0)

#define LogGroup(message, ...) do {\
} while (0)

#define LogGroupDetail(message, ...) do {\
} while (0)

#define LogCorpse(message, ...) do {\
} while (0)

#define LogCorpseDetail(message, ...) do {\
} while (0)

#define LogBazaar(message, ...) do {\
} while (0)

#define LogBazaarDetail(message, ...) do {\
} while (0)

#define LogDisc(message, ...) do {\
} while (0)

#define LogDiscDetail(message, ...) do {\
} while (0)

#define LogBoats(message, ...) do {\
} while (0)

#define LogBoatsDetail(message, ...) do {\
} while (0)

#define LogPTimers(message, ...) do {\
} while (0)

#define LogPTimersDetail(message, ...) do {\
} while (0)

#define LogNexus(message, ...) do {\
} while (0)

#define LogNexusDetail(message, ...) do {\
} while (0)

#define LogPets(message, ...) do {\
} while (0)

#define LogPetsDetail(message, ...) do {\
} while (0)

#define LogRegen(message, ...) do {\
} while (0)

#define LogRegenDetail(message, ...) do {\
} while (0)

#define LogFocus(message, ...) do {\
} while (0)

#define LogFoscusDetail(message, ...) do {\
} while (0)

#define LogDeath(message, ...) do {\
} while (0)

#define LogDeathDetail(message, ...) do {\
} while (0)

#define Log(debug_level, log_category, message, ...) do {\
} while (0)

#define LogF(debug_level, log_category, message, ...) do {\
} while (0)
#endif

#endif //EQEMU_EQEMU_LOGSYS_LOG_ALIASES_H