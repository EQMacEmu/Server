/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2008 EQEMu Development Team (http://eqemulator.net)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include "../common/global_define.h"
#include "../common/eqemu_logsys.h"
#include "../common/strings.h"
#include "database.h"

#include <string.h>
#include <mysqld_error.h>

bool Database::DBSetup()
{
	if (!DBSetup_CheckLegacy())
	{
		return false;
	}
	if (!DBSetup_PlayerAAPurchase())
	{
		return false;
	}
	if (!DBSetup_PlayerDeathBy())
	{
		return false;
	}
	if (!DBSetup_PlayerTSEvents())
	{
		return false;
	}
	if (!DBSetup_PlayerQGlobalUpdates())
	{
		return false;
	}
	if (!DBSetup_PlayerLootRecords())
	{
		return false;
	}
	if (!DBSetup_PlayerItemDesyncs())
	{
		return false;
	}
	return true;
}

bool Database::DBSetup_PlayerAAPurchase()
{
	std::string check_query1 = StringFormat("SHOW TABLES LIKE 'qs_player_aa_purchase_log'");
	auto results1 = QueryDatabase(check_query1);
	if (results1.RowCount() == 0)
	{
		Log(Logs::Detail, Logs::QSServer, "Attempting to create aa purchase table.");
		std::string query2 = StringFormat(
			"CREATE TABLE `qs_player_aa_purchase_log` ( "
			"`char_id` int(11) DEFAULT '0', "
			"`aa_type` varchar(255) DEFAULT '0', "
			"`aa_name` varchar(255) DEFAULT '0', "
			"`aa_id` int(11) DEFAULT '0', "
			"`aa_cost` int(11) DEFAULT '0', "
			"`zone_id` int(11) DEFAULT '0', "
			"`time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP "
			") ENGINE = InnoDB DEFAULT CHARSET = utf8;");
		auto results2 = QueryDatabase(query2);
		if (!results2.Success())
		{
			Log(Logs::General, Logs::QSServer, "Error creating qs_player_aa_purchase_log. \n%s", query2.c_str());
			return false;
		}
	}
	return true;
}

bool Database::DBSetup_PlayerDeathBy()
{
	std::string check_query1 = StringFormat("SHOW TABLES LIKE 'qs_player_killed_by_log'");
	auto results1 = QueryDatabase(check_query1);
	if (results1.RowCount() == 0)
	{
		Log(Logs::Detail, Logs::QSServer, "Attempting to create player deaths table.");
		std::string query2 = StringFormat(
			"CREATE TABLE `qs_player_killed_by_log` ( "
			"`char_id` int(11) DEFAULT '0', "
			"`zone_id` int(11) DEFAULT '0', "
			"`killed_by` varchar(255) DEFAULT NULL, "
			"`spell` int(11) DEFAULT '0', "
			"`damage` int(11) DEFAULT '0', "
			"`time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP "
			") ENGINE = InnoDB DEFAULT CHARSET = utf8;");
		auto results2 = QueryDatabase(query2);
		if (!results2.Success())
		{
			Log(Logs::General, Logs::QSServer, "Error creating qs_player_killed_by_log. \n%s", query2.c_str());
			return false;
		}
	}
	return true;
}

bool Database::DBSetup_PlayerTSEvents()
{
	std::string check_query1 = StringFormat("SHOW TABLES LIKE 'qs_player_ts_event_log'");
	auto results1 = QueryDatabase(check_query1);
	if (results1.RowCount() == 0)
	{
		Log(Logs::Detail, Logs::QSServer, "Attempting to create ts events table.");
		std::string query2 = StringFormat(
			"CREATE TABLE `qs_player_ts_event_log` ( "
			"`char_id` int(11) DEFAULT '0', "
			"`zone_id` int(11) DEFAULT '0', "
			"`results` varchar(255) DEFAULT NULL, "
			"`recipe_id` int(11) DEFAULT '0', "
			"`tradeskill` int(11) DEFAULT '0', "
			"`trivial` int(11) DEFAULT '0', "
			"`chance` float(0) DEFAULT '0', "
			"`time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP "
			") ENGINE = InnoDB DEFAULT CHARSET = utf8;");
		auto results2 = QueryDatabase(query2);
		if (!results2.Success())
		{
			Log(Logs::General, Logs::QSServer, "Error creating qs_player_ts_event_log. \n%s", query2.c_str());
			return false;
		}
	}
	return true;
}

bool Database::DBSetup_PlayerQGlobalUpdates()
{
	std::string check_query1 = StringFormat("SHOW TABLES LIKE 'qs_player_qglobal_updates_log'");
	auto results1 = QueryDatabase(check_query1);
	if (results1.RowCount() == 0)
	{
		Log(Logs::Detail, Logs::QSServer, "Attempting to create qglobal updates table.");
		std::string query2 = StringFormat(
			"CREATE TABLE `qs_player_qglobal_updates_log` ( "
			"`char_id` int(11) DEFAULT '0', "
			"`action` varchar(255) DEFAULT NULL, "
			"`zone_id` int(11) DEFAULT '0', "
			"`varname` varchar(255) DEFAULT NULL, "
			"`newvalue` varchar(255) DEFAULT NULL, "
			"`time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP "
			") ENGINE = InnoDB DEFAULT CHARSET = utf8;");
		auto results2 = QueryDatabase(query2);
		if (!results2.Success())
		{
			Log(Logs::General, Logs::QSServer, "Error creating qs_player_qglobal_updates_log. \n%s", query2.c_str());
			return false;
		}
	}
	return true;
}

bool Database::DBSetup_PlayerLootRecords()
{
	std::string check_query1 = StringFormat("SHOW TABLES LIKE 'qs_player_loot_records_log'");
	auto results1 = QueryDatabase(check_query1);
	if (results1.RowCount() == 0)
	{
		Log(Logs::Detail, Logs::QSServer, "Attempting to create loot records table.");
		std::string query2 = StringFormat(
			"CREATE TABLE `qs_player_loot_records_log` ( "
			"`char_id` int(11) DEFAULT '0', "
			"`corpse_name` varchar(255) DEFAULT NULL, "
			"`type` varchar(255) DEFAULT NULL, "
			"`zone_id` int(11) DEFAULT '0', "
			"`item_id` int(11) DEFAULT '0', "
			"`item_name` varchar(255) DEFAULT NULL, "
			"`charges` mediumint(7) DEFAULT '0', "
			"`platinum` int(11) DEFAULT '0', "
			"`gold` int(11) DEFAULT '0', "
			"`silver` int(11) DEFAULT '0', "
			"`copper` int(11) DEFAULT '0', "
			"`time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP "
			") ENGINE = InnoDB DEFAULT CHARSET = utf8;");
		auto results2 = QueryDatabase(query2);
		if (!results2.Success())
		{
			Log(Logs::General, Logs::QSServer, "Error creating qs_player_loot_records_log. \n%s", query2.c_str());
			return false;
		}
	}
	return true;
}

bool Database::DBSetup_PlayerItemDesyncs()
{
	std::string check_query1 = StringFormat("SHOW TABLES LIKE 'qs_player_item_desyncs_log'");
	auto results1 = QueryDatabase(check_query1);
	if (results1.RowCount() == 0)
	{
		Log(Logs::Detail, Logs::QSServer, "Attempting to create item desyncs table.");
		std::string query2 = StringFormat(
			"CREATE TABLE `qs_player_item_desyncs_log` ( "
			"`char_id` int(11) DEFAULT '0', "
			"`zone_id` int(11) NOT NULL DEFAULT '0', "
			"`error` varchar(512) DEFAULT NULL, "
			"`time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP "
			") ENGINE = InnoDB DEFAULT CHARSET = utf8;");
		auto results2 = QueryDatabase(query2);
		if (!results2.Success())
		{
			Log(Logs::General, Logs::QSServer, "Error creating qs_player_item_desyncs_log. \n%s", query2.c_str());
			return false;
		}
	}
	return true;
}

#pragma region Legacy Setup
#pragma region Trade Tables
bool Database::Check_Trade_Tables()
{
	std::string check_query1 = StringFormat("SHOW TABLES LIKE 'qs_player_trade_record'");
	std::string check_query2 = StringFormat("SHOW TABLES LIKE 'qs_player_trade_record_entries'");
	std::string check_query3 = StringFormat("SHOW TABLES LIKE 'qs_player_trade_log'");
	auto results1 = QueryDatabase(check_query1);
	auto results2 = QueryDatabase(check_query2);
	auto results3 = QueryDatabase(check_query3);
	if (results1.RowCount() == 0 && results2.RowCount() == 0 && results3.RowCount() == 0)
	{
		if (!Create_Trade_Table())
		{
			return false;
		}
	}
	else if (results3.RowCount() == 0)
	{
		if (!Create_Trade_Table())
		{
			return false;
		}
	}

	if (results1.RowCount() != 0 && results2.RowCount() != 0)
	{
		if (Copy_Trade_Record())
		{
			std::string drop_query1 = StringFormat("drop table if exists `qs_player_trade_record`;");
			std::string drop_query2 = StringFormat("drop table if exists `qs_player_trade_record_entries`;");
			auto drop_results1 = QueryDatabase(drop_query1);
			auto drop_results2 = QueryDatabase(drop_query2);
		}
	}
	return true;
}

bool Database::Create_Trade_Table()
{
	Log(Logs::Detail, Logs::QSServer, "Attempting to create trade table.");
	std::string query = StringFormat(
		"CREATE TABLE `qs_player_trade_log` ( "
		"`char1_id` int(11) DEFAULT '0', "
		"`char1_pp` int(11) DEFAULT '0', "
		"`char1_gp` int(11) DEFAULT '0', "
		"`char1_sp` int(11) DEFAULT '0', "
		"`char1_cp` int(11) DEFAULT '0', "
		"`char1_items` mediumint(7) DEFAULT '0', "
		"`char2_id` int(11) DEFAULT '0', "
		"`char2_pp` int(11) DEFAULT '0', "
		"`char2_gp` int(11) DEFAULT '0', "
		"`char2_sp` int(11) DEFAULT '0', "
		"`char2_cp` int(11) DEFAULT '0', "
		"`char2_items` mediumint(7) DEFAULT '0', "
		"`from_id` int(11) DEFAULT '0', "
		"`from_slot` mediumint(7) DEFAULT '0', "
		"`to_id` int(11) DEFAULT '0', "
		"`to_slot` mediumint(7) DEFAULT '0', "
		"`item_id` int(11) DEFAULT '0', "
		"`charges` mediumint(7) DEFAULT '0', "
		"`time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP "
		") ENGINE = InnoDB DEFAULT CHARSET = utf8;");
	auto results = QueryDatabase(query);
	if (!results.Success())
	{
		Log(Logs::General, Logs::QSServer, "Error creating qs_player_trade_log. \n%s", query.c_str());
		return false;
	}
	return true;
}

bool Database::Copy_Trade_Record()
{
	std::string query1 = StringFormat("SELECT * from `qs_player_trade_record`");
	auto results1 = QueryDatabase(query1);
	for (auto row = results1.begin(); row != results1.end(); ++row)
	{
		int32 trade_id = atoi(row[0]);
		std::string time = row[1];
		int32 char1_id = atoi(row[2]);
		int32 char1_pp = atoi(row[3]);
		int32 char1_gp = atoi(row[4]);
		int32 char1_sp = atoi(row[5]);
		int32 char1_cp = atoi(row[6]);
		int32 char1_items = atoi(row[7]);
		int32 char2_id = atoi(row[8]);
		int32 char2_pp = atoi(row[9]);
		int32 char2_gp = atoi(row[10]);
		int32 char2_sp = atoi(row[11]);
		int32 char2_cp = atoi(row[12]);
		int32 char2_items = atoi(row[13]);

		std::string query2 = StringFormat("SELECT * from `qs_player_trade_record_entries` WHERE `event_id` = '%i'", trade_id);
		auto results2 = QueryDatabase(query2);

		auto row2 = results2.begin();
		int32 event_id = atoi(row2[0]);
		int32 from_id = atoi(row2[1]);
		int32 from_slot = atoi(row2[2]);
		int32 to_id = atoi(row2[3]);
		int32 to_slot = atoi(row2[4]);
		int32 item_id = atoi(row2[5]);
		int32 charges = atoi(row2[6]);

		std::string query3 = StringFormat(
			"INSERT into qs_player_trade_log SET "
				"`char1_id` = '%i', "
				"`char1_pp` = '%i', "
				"`char1_gp` = '%i', "
				"`char1_sp` = '%i', "
				"`char1_cp` = '%i', "
				"`char1_items` = '%i', "
				"`char2_id` = '%i', "
				"`char2_pp` = '%i', "
				"`char2_gp` = '%i', "
				"`char2_sp` = '%i', "
				"`char2_cp` = '%i', "
				"`char2_items` = '%i', "
				"`from_id` = '%i', "
				"`from_slot` = '%i', "
				"`to_id` = '%i', "
				"`to_slot` = '%i', "
				"`item_id` = '%i', "
				"`charges` = '%i', "
				"`time` = '%s'",
				char1_id,
				char1_pp,
				char1_gp,
				char1_sp,
				char1_cp,
				char1_items,
				char2_id,
				char2_pp,
				char2_gp,
				char2_sp,
				char2_cp,
				char2_items,
				from_id,
				from_slot,
				to_id,
				to_slot,
				item_id,
				charges,
				time.c_str());

		Log(Logs::Detail, Logs::QSServer, 
			"trade_id: %i time: %s\n"
			"char1_id: %i char1_pp: %i char1_gp: %i char1_sp: %i char1_cp: %i char1_items: %i\n"
			"char2_id: %i char2_pp: %i char2_gp: %i char2_sp: %i char2_cp: %i char2_items: %i\n"
			"event_id: %i from_id: %i from_slot: %i to_id: %i to_slot: %i item_id: %i charges: %i\n",
			trade_id, time.c_str(),
			char1_id, char1_pp, char1_gp, char1_sp, char1_cp, char1_items,
			char2_id, char2_pp, char2_gp, char2_sp, char2_cp, char2_items,
			event_id, from_id, from_slot, to_id, to_slot, item_id, charges);

		auto results3 = QueryDatabase(query3);
		if (!results3.Success())
		{
			Log(Logs::General, Logs::QSServer, "Error copying to qs_player_trade_log: \n%s", query3.c_str());
			return false;
		}
	}
	return true;
}
#pragma endregion

#pragma region Handin Tables
bool Database::Check_Handin_Tables()
{
	std::string check_query1 = StringFormat("SHOW TABLES LIKE 'qs_player_handin_record'");
	std::string check_query2 = StringFormat("SHOW TABLES LIKE 'qs_player_handin_record_entries'");
	std::string check_query3 = StringFormat("SHOW TABLES LIKE 'qs_player_handin_log'");
	auto results1 = QueryDatabase(check_query1);
	auto results2 = QueryDatabase(check_query2);
	auto results3 = QueryDatabase(check_query3);
	if (results1.RowCount() == 0 && results2.RowCount() == 0 && results3.RowCount() == 0)
	{
		if (!Create_Handin_Table())
		{
			return false;
		}
	}
	else if (results3.RowCount() == 0)
	{
		if (!Create_Handin_Table())
		{
			return false;
		}
	}

	if (results1.RowCount() != 0 && results2.RowCount() != 0)
	{
		if (Copy_Handin_Record())
		{
			std::string drop_query1 = StringFormat("drop table if exists `qs_player_handin_record`;");
			std::string drop_query2 = StringFormat("drop table if exists `qs_player_handin_record_entries`;");
			auto drop_results1 = QueryDatabase(drop_query1);
			auto drop_results2 = QueryDatabase(drop_query2);
		}
	}
	return true;
}

bool Database::Create_Handin_Table()
{
	Log(Logs::Detail, Logs::QSServer, "Attempting to create handin table.");
	std::string query = StringFormat(
		"CREATE TABLE `qs_player_handin_log` ( "
		"`char_id` int(11) DEFAULT '0', "
		"`action_type` char(6) DEFAULT 'action', "
		"`quest_id` int(11) DEFAULT '0', "
		"`char_slot` mediumint(7) DEFAULT '0', "
		"`item_id` int(11) DEFAULT '0', "
		"`charges` mediumint(7) DEFAULT '0', "
		"`char_pp` int(11) DEFAULT '0', "
		"`char_gp` int(11) DEFAULT '0', "
		"`char_sp` int(11) DEFAULT '0', "
		"`char_cp` int(11) DEFAULT '0', "
		"`char_items` mediumint(7) DEFAULT '0', "
		"`npc_id` int(11) DEFAULT '0', "
		"`npc_pp` int(11) DEFAULT '0', "
		"`npc_gp` int(11) DEFAULT '0', "
		"`npc_sp` int(11) DEFAULT '0', "
		"`npc_cp` int(11) DEFAULT '0', "
		"`npc_items` mediumint(7) DEFAULT '0', "
		"`time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP "
		") ENGINE = InnoDB DEFAULT CHARSET = utf8;");
	auto results = QueryDatabase(query);
	if (!results.Success())
	{
		Log(Logs::General, Logs::QSServer, "Error creating qs_player_handin_log. \n%s", query.c_str());
		return false;
	}
	return true;
}

bool Database::Copy_Handin_Record()
{
	std::string query1 = StringFormat("SELECT * from `qs_player_handin_record`");
	auto results1 = QueryDatabase(query1);
	for (auto row = results1.begin(); row != results1.end(); ++row)
	{
		int32 handin_id = atoi(row[0]);
		std::string time = row[1];
		int32 quest_id = atoi(row[2]);
		int32 char_id = atoi(row[3]);
		int32 char_pp = atoi(row[4]);
		int32 char_gp = atoi(row[5]);
		int32 char_sp = atoi(row[6]);
		int32 char_cp = atoi(row[7]);
		int32 char_items = atoi(row[8]);
		int32 npc_id = atoi(row[9]);
		int32 npc_pp = atoi(row[10]);
		int32 npc_gp = atoi(row[11]);
		int32 npc_sp = atoi(row[12]);
		int32 npc_cp = atoi(row[13]);
		int32 npc_items = atoi(row[14]);

		std::string query2 = StringFormat("SELECT * from `qs_player_handin_record_entries` WHERE `event_id` = '%i'", handin_id);
		auto results2 = QueryDatabase(query2);

		auto row2 = results2.begin();
		int32 event_id = atoi(row2[0]);
		std::string action_type = row2[1];
		int32 char_slot = atoi(row2[2]);
		int32 item_id = atoi(row2[3]);
		int32 charges = atoi(row2[3]);

		std::string query3 = StringFormat(
			"INSERT into qs_player_handin_log SET "
				"`char_id` = '%i', "
				"`action_type` = '%s', "
				"`quest_id` = '%i', "
				"`char_slot` = '%i', "
				"`item_id` = '%i', "
				"`charges` = '%i', "
				"`char_pp` = '%i', "
				"`char_gp` = '%i', "
				"`char_sp` = '%i', "
				"`char_cp` = '%i', "
				"`char_items` = '%i', "
				"`npc_id` = '%i', "
				"`npc_pp` = '%i', "
				"`npc_gp` = '%i', "
				"`npc_sp` = '%i', "
				"`npc_cp` = '%i', "
				"`npc_items` = '%i', "
				"`time` = '%s'",
				char_id,
				action_type.c_str(),
				quest_id,
				char_slot,
				item_id,
				charges,
				char_pp,
				char_gp,
				char_sp,
				char_cp,
				char_items,
				npc_id,
				npc_pp,
				npc_gp,
				npc_sp,
				npc_cp,
				npc_items,
				time.c_str());

		Log(Logs::Detail, Logs::QSServer, "handin_id: %i time: %s quest_id: %i\n"
			"char_id: %i char_pp: %i char_gp: %i char_sp: %i char_cp: %i char_items: %i\n"
			"npc_id: %i npc_pp: %i npc_gp: %i npc_sp: %i npc_cp: %i npc_items: %i \n"
			"event_id: %i action_type: %s char_slot: %i item_id: %i charges: %i\n",
			handin_id, time.c_str(), quest_id,
			char_id, char_pp, char_gp, char_sp, char_cp, char_items,
			npc_id, npc_pp, npc_gp, npc_sp, npc_cp, npc_items,
			event_id, action_type.c_str(), char_slot, item_id, charges);

		auto results3 = QueryDatabase(query3);
		if (!results3.Success())
		{
			Log(Logs::General, Logs::QSServer, "Error copying to qs_player_handin_log: \n%s", query3.c_str());
			return false;
		}
	}
	return true;
}
#pragma endregion

#pragma region NPCKills
bool Database::Check_NPCKills_Tables()
{
	std::string check_query1 = StringFormat("SHOW TABLES LIKE 'qs_player_npc_kill_record'");
	std::string check_query2 = StringFormat("SHOW TABLES LIKE 'qs_player_npc_kill_record_entries'");
	std::string check_query3 = StringFormat("SHOW TABLES LIKE 'qs_player_npc_kill_log'");
	auto results1 = QueryDatabase(check_query1);
	auto results2 = QueryDatabase(check_query2);
	auto results3 = QueryDatabase(check_query3);
	if (results1.RowCount() == 0 && results2.RowCount() == 0 && results3.RowCount() == 0)
	{
		if (!Create_NPCKills_Table())
		{
			return false;
		}
	}
	else if (results3.RowCount() == 0)
	{
		if (!Create_NPCKills_Table())
		{
			return false;
		}
	}

	if (results1.RowCount() != 0 && results2.RowCount() != 0)
	{
		if (Copy_NPCKills_Record())
		{
			std::string drop_query1 = StringFormat("drop table if exists `qs_player_npc_kill_record`;");
			std::string drop_query2 = StringFormat("drop table if exists `qs_player_npc_kill_record_entries`;");
			auto drop_results1 = QueryDatabase(drop_query1);
			auto drop_results2 = QueryDatabase(drop_query2);
		}
	}
	return true;
}

bool Database::Create_NPCKills_Table()
{
	Log(Logs::Detail, Logs::QSServer, "Attempting to create npc kills table.");
	std::string query = StringFormat(
		"CREATE TABLE `qs_player_npc_kill_log` ( "
		"`char_id` int(11) DEFAULT '0', "
		"`npc_id` int(11) DEFAULT NULL, "
		"`type` int(11) DEFAULT NULL, "
		"`zone_id` int(11) DEFAULT NULL, "
		"`time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP "
		") ENGINE = InnoDB DEFAULT CHARSET = utf8;");
	auto results = QueryDatabase(query);
	if (!results.Success())
	{
		Log(Logs::General, Logs::QSServer, "Error creating qs_player_npc_kill_log. \n%s", query.c_str());
		return false;
	}
	return true;
}

bool Database::Copy_NPCKills_Record()
{
	std::string query1 = StringFormat("SELECT * from `qs_player_npc_kill_record`");
	auto results1 = QueryDatabase(query1);
	for (auto row = results1.begin(); row != results1.end(); ++row)
	{
		int32 fight_id = atoi(row[0]);
		int32 npc_id = atoi(row[1]);
		int32 type = atoi(row[2]);
		int32 zone_id = atoi(row[3]);
		std::string time = row[4];

		std::string query2 = StringFormat("SELECT * from `qs_player_npc_kill_record_entries` WHERE `event_id` = '%i'", fight_id);
		auto results2 = QueryDatabase(query2);

		auto row2 = results2.begin();
		int32 event_id = atoi(row2[0]);
		int32 char_id = atoi(row2[1]);

		std::string query3 = StringFormat(
			"INSERT into qs_player_npc_kill_log SET "
				"`char_id` = '%i', "
				"`npc_id` = '%i', "
				"`type` = '%i', "
				"`zone_id` = '%i', "
				"`time` = '%s'",
				char_id,
				npc_id,
				type,
				zone_id,
				time.c_str());

		Log(Logs::Detail, Logs::QSServer,
			"fight_id: %i npc_id: %i type: %i zone_id: %i time: %s\n"
			"event_id: %i char_id: %i\n",
			fight_id, npc_id, type, zone_id, time.c_str(),
			event_id, char_id);

		auto results3 = QueryDatabase(query3);
		if (!results3.Success())
		{
			Log(Logs::General, Logs::QSServer, "Error copying to qs_player_npc_kill_log: \n%s", query3.c_str());
			return false;
		}
	}
	return true;
}
#pragma endregion

#pragma region Merchant Tables
bool Database::Check_Merchant_Tables()
{
	std::string check_query1 = StringFormat("SHOW TABLES LIKE 'qs_merchant_transaction_record'");
	std::string check_query2 = StringFormat("SHOW TABLES LIKE 'qs_merchant_transaction_record_entries'");
	std::string check_query3 = StringFormat("SHOW TABLES LIKE 'qs_merchant_transaction_log'");
	auto results1 = QueryDatabase(check_query1);
	auto results2 = QueryDatabase(check_query2);
	auto results3 = QueryDatabase(check_query3);
	if (results1.RowCount() == 0 && results2.RowCount() == 0 && results3.RowCount() == 0)
	{
		if (!Create_Merchant_Table())
		{
			return false;
		}
	}
	else if (results3.RowCount() == 0)
	{
		if (!Create_Merchant_Table())
		{
			return false;
		}
	}

	if (results1.RowCount() != 0 && results2.RowCount() != 0)
	{
		if (Copy_Merchant_Record())
		{
			std::string drop_query1 = StringFormat("drop table if exists `qs_merchant_transaction_record`;");
			std::string drop_query2 = StringFormat("drop table if exists `qs_merchant_transaction_record_entries`;");
			auto drop_results1 = QueryDatabase(drop_query1);
			auto drop_results2 = QueryDatabase(drop_query2);
		}
	}
	return true;
}

bool Database::Create_Merchant_Table()
{
	Log(Logs::Detail, Logs::QSServer, "Attempting to create merchant table.");
	std::string query = StringFormat(
		"CREATE TABLE `qs_merchant_transaction_log` ( "
		"`char_id` int(11) DEFAULT '0', "
		"`char_slot` mediumint(7) DEFAULT '0', "
		"`item_id` int(11) DEFAULT '0', "
		"`charges` mediumint(7) DEFAULT '0', "
		"`zone_id` int(11) DEFAULT '0', "
		"`merchant_id` int(11) DEFAULT '0', "
		"`merchant_pp` int(11) DEFAULT '0', "
		"`merchant_gp` int(11) DEFAULT '0', "
		"`merchant_sp` int(11) DEFAULT '0', "
		"`merchant_cp` int(11) DEFAULT '0', "
		"`merchant_items` mediumint(7) DEFAULT '0', "
		"`char_pp` int(11) DEFAULT '0', "
		"`char_gp` int(11) DEFAULT '0', "
		"`char_sp` int(11) DEFAULT '0', "
		"`char_cp` int(11) DEFAULT '0', "
		"`char_items` mediumint(7) DEFAULT '0', "
		"`time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP "
		") ENGINE = InnoDB DEFAULT CHARSET = utf8;");
	auto results = QueryDatabase(query);
	if (!results.Success())
	{
		Log(Logs::General, Logs::QSServer, "Error creating qs_merchant_transaction_log. \n%s", query.c_str());
		return false;
	}
	return true;
}

bool Database::Copy_Merchant_Record()
{
	std::string query1 = StringFormat("SELECT * from `qs_merchant_transaction_record`");
	auto results1 = QueryDatabase(query1);
	for (auto row = results1.begin(); row != results1.end(); ++row)
	{
		int32 transaction_id = atoi(row[0]);
		std::string time = row[1];
		int32 zone_id = atoi(row[2]);
		int32 merchant_id = atoi(row[3]);
		int32 merchant_pp = atoi(row[4]);
		int32 merchant_gp = atoi(row[5]);
		int32 merchant_sp = atoi(row[6]);
		int32 merchant_cp = atoi(row[7]);
		int32 merchant_items = atoi(row[8]);
		int32 char_id = atoi(row[9]);
		int32 char_pp = atoi(row[10]);
		int32 char_gp = atoi(row[11]);
		int32 char_sp = atoi(row[12]);
		int32 char_cp = atoi(row[13]);
		int32 char_items = atoi(row[14]);

		std::string query2 = StringFormat("SELECT * from `qs_merchant_transaction_record_entries` WHERE `event_id` = '%i'", transaction_id);
		auto results2 = QueryDatabase(query2);

		auto row2 = results2.begin();
		int32 event_id = atoi(row2[0]);
		int32 char_slot = atoi(row2[1]);
		int32 item_id = atoi(row2[2]);
		int32 charges = atoi(row2[3]);

		std::string query3 = StringFormat(
			"INSERT into qs_merchant_transaction_log SET "
				"`char_id` = '%i', "
				"`char_slot` = '%i', "
				"`item_id` = '%i', "
				"`charges` = '%i', "
				"`zone_id` = '%i', "
				"`merchant_id` = '%i', "
				"`merchant_pp` = '%i', "
				"`merchant_gp` = '%i', "
				"`merchant_sp` = '%i', "
				"`merchant_cp` = '%i', "
				"`merchant_items` = '%i', "
				"`char_pp` = '%i', "
				"`char_gp` = '%i', "
				"`char_sp` = '%i', "
				"`char_cp` = '%i', "
				"`char_items` = '%i', "
				"`time` = '%s'",
				char_id,
				char_slot,
				item_id,
				charges,
				zone_id,
				merchant_id,
				merchant_pp,
				merchant_gp,
				merchant_sp,
				merchant_cp,
				merchant_items,
				char_pp,
				char_gp,
				char_sp,
				char_cp,
				char_items,
				time.c_str());

		Log(Logs::Detail, Logs::QSServer, "transaction_id: %i time: %s zone_id: %i\n"
			"merchant_id: %i merchant_pp: %i merchant_gp: %i merchant_sp: %i merchant_cp: %i merchant_cp: %i \n"
			"char_id: %i char_pp: %i char_gp: %i char_sp: %i char_cp: %i char_items: %i \n"
			"event_id: %i char_slot: %i item_id: %i charges: %i \n",
			transaction_id, time.c_str(), zone_id,
			merchant_id, merchant_pp, merchant_gp, merchant_sp, merchant_cp, merchant_items,
			char_id, char_pp, char_gp, char_sp, char_cp, char_items,
			event_id, char_slot, item_id, charges);

		auto results3 = QueryDatabase(query3);
		if (!results3.Success())
		{
			Log(Logs::General, Logs::QSServer, "Error copying to qs_merchant_transaction_log: \n%s", query3.c_str());
			return false;
		}
	}
	return true;
}
#pragma endregion

#pragma region Delete Tables
bool Database::Check_Delete_Tables()
{
	std::string check_query1 = StringFormat("SHOW TABLES LIKE 'qs_player_delete_record'");
	std::string check_query2 = StringFormat("SHOW TABLES LIKE 'qs_player_delete_record_entries'");
	std::string check_query3 = StringFormat("SHOW TABLES LIKE 'qs_player_item_delete_log'");
	auto results1 = QueryDatabase(check_query1);
	auto results2 = QueryDatabase(check_query2);
	auto results3 = QueryDatabase(check_query3);
	if (results1.RowCount() == 0 && results2.RowCount() == 0 && results3.RowCount() == 0)
	{
		if (!Create_Delete_Table())
		{
			return false;
		}
	}
	else if (results3.RowCount() == 0)
	{
		if (!Create_Delete_Table())
		{
			return false;
		}
	}

	if (results1.RowCount() != 0 && results2.RowCount() != 0)
	{
		if (Copy_Delete_Record())
		{
			std::string drop_query1 = StringFormat("drop table if exists `qs_player_delete_record`;");
			std::string drop_query2 = StringFormat("drop table if exists `qs_player_delete_record_entries`;");
			auto drop_results1 = QueryDatabase(drop_query1);
			auto drop_results2 = QueryDatabase(drop_query2);
		}
	}
	return true;
}

bool Database::Create_Delete_Table()
{
	Log(Logs::Detail, Logs::QSServer, "Attempting to create delete table.");
	std::string query = StringFormat(
		"CREATE TABLE `qs_player_item_delete_log` ( "
		"`char_id` int(11) DEFAULT '0', "
		"`char_slot` mediumint(7) DEFAULT '0', "
		"`item_id` int(11) DEFAULT '0', "
		"`charges` mediumint(7) DEFAULT '0', "
		"`stack_size` mediumint(7) DEFAULT '0', "
		"`char_items` mediumint(7) DEFAULT '0', "
		"`time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP "
		") ENGINE = InnoDB DEFAULT CHARSET = utf8;");
	auto results = QueryDatabase(query);
	if (!results.Success())
	{
		Log(Logs::General, Logs::QSServer, "Error creating qs_player_item_delete_log. \n%s", query.c_str());
		return false;
	}
	return true;
}

bool Database::Copy_Delete_Record()
{
	std::string query1 = StringFormat("SELECT * from `qs_player_delete_record`");
	auto results1 = QueryDatabase(query1);
	for (auto row = results1.begin(); row != results1.end(); ++row)
	{
		int32 delete_id = atoi(row[0]);
		std::string time = row[1];
		int32 char_id = atoi(row[2]);
		int32 stack_size = atoi(row[3]);
		int32 char_items = atoi(row[4]);

		std::string query2 = StringFormat("SELECT * from `qs_player_delete_record_entries` WHERE `event_id` = '%i'", delete_id);
		auto results2 = QueryDatabase(query2);

		auto row2 = results2.begin();
		int32 event_id = atoi(row2[0]);
		int32 char_slot = atoi(row2[1]);
		int32 item_id = atoi(row2[2]);
		int32 charges = atoi(row2[3]);

		std::string query3 = StringFormat(
			"INSERT into qs_player_item_delete_log SET "
				"`char_id` = '%i', "
				"`char_slot` = '%i', "
				"`item_id` = '%i', "
				"`charges` = '%i', "
				"`stack_size` = '%i', "
				"`char_items` = '%i', "
				"`time` = '%s'",
				char_id,
				char_slot,
				item_id,
				charges,
				stack_size,
				char_items,
				time.c_str());

		Log(Logs::Detail, Logs::QSServer, "delete_id: %i time: %s char_id: %i stack_size: %i char_items: %i \n"
			"event_id: %i char_slot: %i item_id: %i charges: %i \n",
			delete_id, time.c_str(), char_id, stack_size, char_items,
			event_id, char_slot, item_id, charges);

		auto results3 = QueryDatabase(query3);
		if (!results3.Success())
		{
			Log(Logs::General, Logs::QSServer, "Error copying to qs_player_item_delete_log: \n%s", query3.c_str());
			return false;
		}
	}
	return true;
}
#pragma endregion

#pragma region ItemMove Tables
bool Database::Check_ItemMove_Tables()
{
	std::string check_query1 = StringFormat("SHOW TABLES LIKE 'qs_player_move_record'");
	std::string check_query2 = StringFormat("SHOW TABLES LIKE 'qs_player_move_record_entries'");
	std::string check_query3 = StringFormat("SHOW TABLES LIKE 'qs_player_item_move_log'");
	auto results1 = QueryDatabase(check_query1);
	auto results2 = QueryDatabase(check_query2);
	auto results3 = QueryDatabase(check_query3);
	if (results1.RowCount() == 0 && results2.RowCount() == 0 && results3.RowCount() == 0)
	{
		if (!Create_ItemMove_Table())
		{
			return false;
		}
	}
	else if (results3.RowCount() == 0)
	{
		if (!Create_ItemMove_Table())
		{
			return false;
		}
	}

	if (results1.RowCount() != 0 && results2.RowCount() != 0)
	{
		if (Copy_ItemMove_Record())
		{
			std::string drop_query1 = StringFormat("drop table if exists `qs_player_move_record`;");
			std::string drop_query2 = StringFormat("drop table if exists `qs_player_move_record_entries`;");
			auto drop_results1 = QueryDatabase(drop_query1);
			auto drop_results2 = QueryDatabase(drop_query2);
		}
	}
	return true;
}

bool Database::Create_ItemMove_Table()
{
	Log(Logs::Detail, Logs::QSServer, "Attempting to create item move table.");
	std::string query = StringFormat(
		"CREATE TABLE `qs_player_item_move_log` ( "
		"`char_id` int(11) DEFAULT '0', "
		"`from_slot` mediumint(7) DEFAULT '0', "
		"`to_slot` mediumint(7) DEFAULT '0', "
		"`item_id` int(11) DEFAULT '0', "
		"`charges` mediumint(7) DEFAULT '0', "
		"`stack_size` mediumint(7) DEFAULT '0', "
		"`char_items` mediumint(7) DEFAULT '0', "
		"`postaction` tinyint(1) DEFAULT '0', "
		"`time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP "
		") ENGINE = InnoDB DEFAULT CHARSET = utf8;");
	auto results = QueryDatabase(query);
	if (!results.Success())
	{
		Log(Logs::General, Logs::QSServer, "Error creating qs_player_item_move_log. \n%s", query.c_str());
		return false;
	}
	return true;
}

bool Database::Copy_ItemMove_Record()
{
	std::string query1 = StringFormat("SELECT * from `qs_player_move_record`");
	auto results1 = QueryDatabase(query1);
	for (auto row = results1.begin(); row != results1.end(); ++row)
	{
		int32 move_id = atoi(row[0]);
		std::string time = row[1];
		int32 char_id = atoi(row[2]);
		int32 from_slot = atoi(row[3]);
		int32 to_slot = atoi(row[4]);
		int32 stack_size = atoi(row[5]);
		int32 char_items = atoi(row[6]);
		int32 postaction = atoi(row[7]);

		std::string query2 = StringFormat("SELECT * from `qs_player_move_record_entries` WHERE `event_id` = '%i'", move_id);
		auto results2 = QueryDatabase(query2);

		auto row2 = results2.begin();
		int32 event_id = atoi(row2[0]);
		int32 from_slot2 = atoi(row[1]);
		int32 to_slot2 = atoi(row[2]);
		int32 item_id = atoi(row2[3]);
		int32 charges = atoi(row2[4]);

		std::string query3 = StringFormat(
			"INSERT into qs_player_item_move_log SET "
				"`char_id` = '%i', "
				"`from_slot` = '%i', "
				"`to_slot` = '%i', "
				"`item_id` = '%i', "
				"`charges` = '%i', "
				"`stack_size` = '%i', "
				"`char_items` = '%i', "
				"`postaction` = '%i', "
				"`time` = '%s'",
				char_id,
				from_slot,
				to_slot,
				item_id,
				charges,
				stack_size,
				char_items,
				postaction,
				time.c_str());

		Log(Logs::Detail, Logs::QSServer,
			"move_id: %i time: %s char_id: %i from_slot: %i to_slot: %i stack_size: %i char_items: %i postaction: %i\n"
			"event_id: %i from_slot: %i to_slot: %i item_id: %i charges: %i\n",
			move_id, time.c_str(), char_id, from_slot, to_slot, stack_size, char_items, postaction,
			event_id, from_slot2, to_slot2, item_id, charges);

		auto results3 = QueryDatabase(query3);
		if (!results3.Success())
		{
			Log(Logs::General, Logs::QSServer, "Error copying to qs_player_item_move_log: \n%s", query3.c_str());
			return false;
		}
	}
	return true;
}
#pragma endregion

bool Database::DBSetup_CheckLegacy()
{
	if (!Check_Trade_Tables())
	{
		return false;
	}
	if (!Check_Handin_Tables())
	{
		return false;
	}
	if (!Check_NPCKills_Tables())
	{
		return false;
	}
	if (!Check_Merchant_Tables())
	{
		return false;
	}
	if (!Check_Delete_Tables())
	{
		return false;
	}
	if (!Check_ItemMove_Tables())
	{
		return false;
	}
	LogInfo("End of DBSetup_CheckLegacy migration.");
	return true;
}
#pragma endregion
