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

#ifndef CHATSERVER_DATABASE_H
#define CHATSERVER_DATABASE_H

#define AUTHENTICATION_TIMEOUT	60
#define INVALID_ID				0xFFFFFFFF

#include "../common/eqemu_logsys.h"
#include "../common/global_define.h"
#include "../common/types.h"
#include "../common/dbcore.h"
#include "../common/linked_list.h"
#include "../common/servertalk.h"

#include <string>
#include <vector>
#include <map>

//atoi is not uint32 or uint32 safe!!!!
#define atoul(str) strtoul(str, nullptr, 10)

class Database : public DBcore {
public:
	Database();
	~Database();
	Database(const char* host, const char* user, const char* passwd, const char* database, uint32 port);
	bool Connect(const char* host, const char* user, const char* passwd, const char* database, uint32 port);

	void LogPlayerAARateHourly(QSPlayerAARateHourly_Struct* QS, uint32 items);
	void LogPlayerAAPurchase(QSPlayerAAPurchase_Struct* QS, uint32 items);
	void LogPlayerQGlobalUpdates(QSPlayerQGlobalUpdate_Struct* QS, uint32 items);
	void LogPlayerTSEvents(QSPlayerTSEvents_Struct* QS, uint32 items);

	void LogMerchantTransaction(QSMerchantLogTransaction_Struct* QS, uint32 Items);

	void LogPlayerItemDelete(QSPlayerLogItemDelete_Struct* QS, uint32 Items);
	void LogPlayerItemMove(QSPlayerLogItemMove_Struct* QS, uint32 Items);
	void LogPlayerLootRecords(QSPlayerLootRecords_struct* QS, uint32 Items);

	void GeneralQueryReceive(ServerPacket *pack);
	void LoadLogSettings(EQEmuLogSys::LogSettings* log_settings);

	/*
	 * Database Setup for bootstraps only.
	 */
	bool DBSetup();
	bool DBSetup_CheckLegacy();
	bool DBSetup_PlayerAAPurchase();
	bool DBSetup_PlayerDeathBy();
	bool DBSetup_PlayerTSEvents();
	bool DBSetup_PlayerQGlobalUpdates();
	bool DBSetup_PlayerLootRecords();
	bool DBSetup_PlayerItemDesyncs();

	bool Check_Trade_Tables();
	bool Create_Trade_Table();
	bool Copy_Trade_Record();

	bool Check_Handin_Tables();
	bool Create_Handin_Table();
	bool Copy_Handin_Record();

	bool Check_NPCKills_Tables();
	bool Create_NPCKills_Table();
	bool Copy_NPCKills_Record();

	bool Check_Merchant_Tables();
	bool Create_Merchant_Table();
	bool Copy_Merchant_Record();

	bool Check_Delete_Tables();
	bool Create_Delete_Table();
	bool Copy_Delete_Record();

	bool Check_ItemMove_Tables();
	bool Create_ItemMove_Table();
	bool Copy_ItemMove_Record();

protected:
	void HandleMysqlError(uint32 errnum);
private:
	void DBInitVars();

};

#endif

