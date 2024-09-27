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

#include <stdio.h>

#include "../../common/eqemu_logsys.h"
#include "../../common/global_define.h"
#include "../../common/shareddb.h"
#include "../../common/eqemu_config.h"
#include "../../common/platform.h"
#include "../../common/crash.h"
#include "../../common/rulesys.h"
#include "../../common/strings.h"
#include "../../common/content/world_content_service.h"
#include "../../common/path_manager.h"
#include "../../common/zone_store.h"
#include "../../common/repositories/skill_caps_repository.h"
#include "../../common/file.h"
#include "../../common/skill_caps.h"

EQEmuLogSys LogSys;
WorldContentService content_service;
ZoneStore zone_store;
PathManager path;

void ExportSpells(SharedDatabase *db);
void ExportSkillCaps(SharedDatabase *db);

int main(int argc, char **argv) {
	RegisterExecutablePlatform(ExePlatformClientExport);
	LogSys.LoadLogSettingsDefaults();
	set_exception_handler();

	path.LoadPaths();

	LogInfo("Client Files Export Utility");
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

	ExportSpells(&database);
	ExportSkillCaps(&database);

	LogSys.CloseFileLogs();

	return 0;
}

void ExportSpells(SharedDatabase *db) {
	Log(Logs::General, Logs::Status, "Exporting Spells...");

	std::string file = fmt::format("{}/export/spells_us.txt", path.GetServerPath());
	FILE *f = fopen(file.c_str(), "w");
	if(!f) {
		Log(Logs::General, Logs::Error, "Unable to open export/spells_us.txt to write, skipping.");
		return;
	}

	const std::string query = "SELECT * FROM spells_new ORDER BY id";
	auto results = db->QueryDatabase(query);

	if(results.Success()) {
        for (auto row = results.begin(); row != results.end(); ++row) {
			std::string line;
			unsigned int fields = results.ColumnCount();
			for(unsigned int i = 0; i < fields; ++i) {
				if(i != 0) {
					line.push_back('^');
				}

				if(row[i] != nullptr) {
					line += row[i];
				}
			}

			fprintf(f, "%s\n", line.c_str());
		}
	} else {
	}

	fclose(f);
}

bool SkillUsable(SharedDatabase *db, int skill_id, int class_id) 
{
	const auto& l = SkillCapsRepository::GetWhere(
		*db,
		fmt::format(
			"`class_id` = {} AND `skill_id` = {} ORDER BY `cap` DESC LIMIT 1",
			class_id,
			skill_id
		)
	);

	return !l.empty();
}

uint32 GetSkill(SharedDatabase* db, int skill_id, int class_id, int level) 
{
	const auto& l = SkillCapsRepository::GetWhere(
		*db,
		fmt::format(
			"`class_id` = {} AND `skill_id` = {} AND `level` = {}",
			class_id,
			skill_id,
			level
		)
	);

	if (l.empty()) {
		return 0;
	}

	auto e = l.front();

	return e.cap;
}

void ExportSkillCaps(SharedDatabase* db) 
{
	LogInfo("Exporting Skill Caps");

	std::ofstream file(fmt::format("{}/export/SkillCaps.txt", path.GetServerPath()));
	if (!file || !file.is_open()) {
		LogError("Unable to open export/SkillCaps.txt to write, skipping.");
		return;
	}

	for (uint8 class_id = Class::Warrior; class_id <= Class::Beastlord; class_id++) {
		for (uint8 skill_id = EQ::skills::Skill1HBlunt; skill_id <= EQ::skills::SkillTaunt; skill_id++) {
			if (SkillUsable(db, skill_id, class_id)) {
				uint32 previous_cap = 0;
				for (
					uint8 level = 1;
					level <= SkillCaps::GetSkillCapMaxLevel(class_id, static_cast<EQ::skills::SkillType>(skill_id));
					level++
					) {
					uint32 cap = GetSkill(db, skill_id, class_id, level);
					if(cap < previous_cap) {
						cap = previous_cap;
					}

					file << fmt::format("{}^{}^{}^{}^0", class_id, skill_id, level, cap) << std::endl;
					previous_cap = cap;
				}
			}
		}
	}

	file.close();
}
