/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2013 EQEMu Development Team (http://eqemulator.net)

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

#include "../../common/eqemu_logsys.h"
#include "../../common/global_define.h"
#include "../../common/shareddb.h"
#include "../../common/eqemu_config.h"
#include "../../common/platform.h"
#include "../../common/crash.h"
#include "../../common/rulesys.h"
#include "../../common/strings.h"
#include "../../common/content/world_content_service.h"
#include "../../common/zone_store.h"
#include "../../common/path_manager.h"

EQEmuLogSys LogSys;
WorldContentService content_service;
ZoneStore zone_store;
PathManager path;

void ImportSpells(SharedDatabase *db);
void ImportSkillCaps(SharedDatabase *db);

int main(int argc, char **argv) {
	RegisterExecutablePlatform(ExePlatformClientImport);
	LogSys.LoadLogSettingsDefaults();
	set_exception_handler();

	path.LoadPaths();

	LogInfo("Client Files Import Utility");
	if(!EQEmuConfig::LoadConfig()) {
		LogError("Unable to load configuration file");
		return 1;
	}

	auto Config = EQEmuConfig::get();

	SharedDatabase database;
	Log(Logs::General, Logs::Status, "Connecting to database...");
	if(!database.Connect(Config->DatabaseHost.c_str(), Config->DatabaseUsername.c_str(),
		Config->DatabasePassword.c_str(), Config->DatabaseDB.c_str(), Config->DatabasePort)) {
		Log(Logs::General, Logs::Error, "Unable to connect to the database, cannot continue without a "
			"database connection");
		return 1;
	}

	LogSys.SetDatabase(&database)
		->SetLogPath(path.GetLogPath())
		->LoadLogDatabaseSettings()
		->StartFileLogs();

	ImportSpells(&database);
	ImportSkillCaps(&database);

	LogSys.CloseFileLogs();
	
	return 0;
}

int GetSpellColumns(SharedDatabase *db) {

	const std::string query = "DESCRIBE spells_new";
	auto results = db->QueryDatabase(query);
	if(!results.Success()) {
        return 0;
    }

	return results.RowCount();
}

bool IsStringField(int i) {
	switch(i)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		return true;
		break;
	default:
		return false;
	}
}

void ImportSpells(SharedDatabase *db) {
	Log(Logs::General, Logs::Status, "Importing Spells...");
	std::string file = fmt::format("{}/import/spells_us.txt", path.GetServerPath());
	FILE *f = fopen(file.c_str(), "r");
	if(!f) {
		LogError("Unable to open {} to read, skipping.", file);
		return;
	}

	std::string query = "DELETE FROM spells_new";
	db->QueryDatabase(query);

	int columns = GetSpellColumns(db);
	int spells_imported = 0;

	char buffer[2048];
	while(fgets(buffer, 2048, f)) {
		for(int i = 0; i < 2048; ++i) {
			if(buffer[i] == '\n') {
				buffer[i] = 0;
				break;
			}
		}

		std::string escaped = ::Strings::Escape(buffer);
		auto split = Strings::Split(escaped, '^');
		int line_columns = (int)split.size();

		std::string sql;
		if(line_columns >= columns) {
			sql = "INSERT INTO spells_new VALUES(";
			for(int i = 0; i < columns; ++i) {
				if(i != 0) {
					sql += ", '";
				} else {
					sql += "'";
				}

				if(split[i].compare("") == 0 && !IsStringField(i)) {
					sql += "0";
				}
				else {
					sql += split[i];
				}
				sql += "'";
			}

			sql += ");";
		} else {
			int i = 0;
			sql = "INSERT INTO spells_new VALUES(";
			for(; i < line_columns; ++i) {
				if(i != 0) {
					sql += ", '";
				} else {
					sql += "'";
				}

				if(split[i].compare("") == 0 && !IsStringField(i)) {
					sql += "0";
				} else {
					sql += split[i];
				}

				sql += "'";
			}

			for(; i < columns; ++i) {
				sql += ", '0'";
			}

			sql += ");";
		}

		db->QueryDatabase(sql);

		spells_imported++;
		if(spells_imported % 1000 == 0) {
			Log(Logs::General, Logs::Status, "%d spells imported.", spells_imported);
		}
	}

	if(spells_imported % 1000 != 0) {
		Log(Logs::General, Logs::Status, "%d spells imported.", spells_imported);
	}

	fclose(f);
}

void ImportSkillCaps(SharedDatabase *db) {
	Log(Logs::General, Logs::Status, "Importing Skill Caps...");

	std::string file = fmt::format("{}/import/SkillCaps.txt", path.GetServerPath());
	FILE *f = fopen(file.c_str(), "r");
	if(!f) {
		LogError("Unable to open {} to read, skipping.", file);
		return;
	}

	std::string delete_sql = "DELETE FROM skill_caps";
	db->QueryDatabase(delete_sql);

	char buffer[2048];
	while(fgets(buffer, 2048, f)) {
		auto split = Strings::Split(buffer, '^');

		if(split.size() < 4) {
			continue;
		}


		int class_id, skill_id, level, cap;
		class_id = atoi(split[0].c_str());
		skill_id = atoi(split[1].c_str());
		level = atoi(split[2].c_str());
		cap = atoi(split[3].c_str());

		std::string sql = StringFormat("INSERT INTO skill_caps(class, skillID, level, cap) VALUES(%d, %d, %d, %d)",
			class_id, skill_id, level, cap);

		db->QueryDatabase(sql);
	}

	fclose(f);
}

