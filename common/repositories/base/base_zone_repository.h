/**
 * DO NOT MODIFY THIS FILE
 *
 * This repository was automatically generated and is NOT to be modified directly.
 * Any repository modifications are meant to be made to the repository extending the base.
 * Any modifications to base repositories are to be made by the generator only
 *
 * @generator ./utils/scripts/generators/repository-generator.pl
 * @docs https://docs.eqemu.io/developer/repositories
 */

#ifndef EQEMU_BASE_ZONE_REPOSITORY_H
#define EQEMU_BASE_ZONE_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>

class BaseZoneRepository {
public:
	struct Zone {
		std::string short_name;
		int32_t     id;
		std::string file_name;
		std::string long_name;
		std::string map_file_name;
		float       safe_x;
		float       safe_y;
		float       safe_z;
		float       safe_heading;
		float       graveyard_id;
		uint8_t     min_level;
		uint8_t     min_status;
		int32_t     zoneidnumber;
		int32_t     timezone;
		int32_t     maxclients;
		uint32_t    ruleset;
		std::string note;
		float       underworld;
		float       minclip;
		float       maxclip;
		float       fog_minclip;
		float       fog_maxclip;
		uint8_t     fog_blue;
		uint8_t     fog_red;
		uint8_t     fog_green;
		uint8_t     sky;
		uint8_t     ztype;
		float       zone_exp_multiplier;
		float       gravity;
		uint8_t     time_type;
		uint8_t     fog_red1;
		uint8_t     fog_green1;
		uint8_t     fog_blue1;
		float       fog_minclip1;
		float       fog_maxclip1;
		uint8_t     fog_red2;
		uint8_t     fog_green2;
		uint8_t     fog_blue2;
		float       fog_minclip2;
		float       fog_maxclip2;
		uint8_t     fog_red3;
		uint8_t     fog_green3;
		uint8_t     fog_blue3;
		float       fog_minclip3;
		float       fog_maxclip3;
		uint8_t     fog_red4;
		uint8_t     fog_green4;
		uint8_t     fog_blue4;
		float       fog_minclip4;
		float       fog_maxclip4;
		float       fog_density;
		std::string flag_needed;
		int8_t      canbind;
		int8_t      cancombat;
		int8_t      canlevitate;
		int8_t      castoutdoor;
		uint8_t     hotzone;
		uint64_t    shutdowndelay;
		int8_t      peqzone;
		int8_t      expansion;
		uint8_t     suspendbuffs;
		int32_t     rain_chance1;
		int32_t     rain_chance2;
		int32_t     rain_chance3;
		int32_t     rain_chance4;
		int32_t     rain_duration1;
		int32_t     rain_duration2;
		int32_t     rain_duration3;
		int32_t     rain_duration4;
		int32_t     snow_chance1;
		int32_t     snow_chance2;
		int32_t     snow_chance3;
		int32_t     snow_chance4;
		int32_t     snow_duration1;
		int32_t     snow_duration2;
		int32_t     snow_duration3;
		int32_t     snow_duration4;
		int32_t     type;
		int8_t      skylock;
		int8_t      skip_los;
		int8_t      music;
		int8_t      random_loc;
		int8_t      dragaggro;
		int8_t      never_idle;
		int8_t      castdungeon;
		uint16_t    pull_limit;
		int32_t     graveyard_time;
		float       max_z;
		int8_t      min_expansion;
		int8_t      max_expansion;
		std::string content_flags;
		std::string content_flags_disabled;
	};

	static std::string PrimaryKey()
	{
		return std::string("id");
	}

	static std::vector<std::string> Columns()
	{
		return {
			"short_name",
			"id",
			"file_name",
			"long_name",
			"map_file_name",
			"safe_x",
			"safe_y",
			"safe_z",
			"safe_heading",
			"graveyard_id",
			"min_level",
			"min_status",
			"zoneidnumber",
			"timezone",
			"maxclients",
			"ruleset",
			"note",
			"underworld",
			"minclip",
			"maxclip",
			"fog_minclip",
			"fog_maxclip",
			"fog_blue",
			"fog_red",
			"fog_green",
			"sky",
			"ztype",
			"zone_exp_multiplier",
			"gravity",
			"time_type",
			"fog_red1",
			"fog_green1",
			"fog_blue1",
			"fog_minclip1",
			"fog_maxclip1",
			"fog_red2",
			"fog_green2",
			"fog_blue2",
			"fog_minclip2",
			"fog_maxclip2",
			"fog_red3",
			"fog_green3",
			"fog_blue3",
			"fog_minclip3",
			"fog_maxclip3",
			"fog_red4",
			"fog_green4",
			"fog_blue4",
			"fog_minclip4",
			"fog_maxclip4",
			"fog_density",
			"flag_needed",
			"canbind",
			"cancombat",
			"canlevitate",
			"castoutdoor",
			"hotzone",
			"shutdowndelay",
			"peqzone",
			"expansion",
			"suspendbuffs",
			"rain_chance1",
			"rain_chance2",
			"rain_chance3",
			"rain_chance4",
			"rain_duration1",
			"rain_duration2",
			"rain_duration3",
			"rain_duration4",
			"snow_chance1",
			"snow_chance2",
			"snow_chance3",
			"snow_chance4",
			"snow_duration1",
			"snow_duration2",
			"snow_duration3",
			"snow_duration4",
			"type",
			"skylock",
			"skip_los",
			"music",
			"random_loc",
			"dragaggro",
			"never_idle",
			"castdungeon",
			"pull_limit",
			"graveyard_time",
			"max_z",
			"min_expansion",
			"max_expansion",
			"content_flags",
			"content_flags_disabled",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"short_name",
			"id",
			"file_name",
			"long_name",
			"map_file_name",
			"safe_x",
			"safe_y",
			"safe_z",
			"safe_heading",
			"graveyard_id",
			"min_level",
			"min_status",
			"zoneidnumber",
			"timezone",
			"maxclients",
			"ruleset",
			"note",
			"underworld",
			"minclip",
			"maxclip",
			"fog_minclip",
			"fog_maxclip",
			"fog_blue",
			"fog_red",
			"fog_green",
			"sky",
			"ztype",
			"zone_exp_multiplier",
			"gravity",
			"time_type",
			"fog_red1",
			"fog_green1",
			"fog_blue1",
			"fog_minclip1",
			"fog_maxclip1",
			"fog_red2",
			"fog_green2",
			"fog_blue2",
			"fog_minclip2",
			"fog_maxclip2",
			"fog_red3",
			"fog_green3",
			"fog_blue3",
			"fog_minclip3",
			"fog_maxclip3",
			"fog_red4",
			"fog_green4",
			"fog_blue4",
			"fog_minclip4",
			"fog_maxclip4",
			"fog_density",
			"flag_needed",
			"canbind",
			"cancombat",
			"canlevitate",
			"castoutdoor",
			"hotzone",
			"shutdowndelay",
			"peqzone",
			"expansion",
			"suspendbuffs",
			"rain_chance1",
			"rain_chance2",
			"rain_chance3",
			"rain_chance4",
			"rain_duration1",
			"rain_duration2",
			"rain_duration3",
			"rain_duration4",
			"snow_chance1",
			"snow_chance2",
			"snow_chance3",
			"snow_chance4",
			"snow_duration1",
			"snow_duration2",
			"snow_duration3",
			"snow_duration4",
			"type",
			"skylock",
			"skip_los",
			"music",
			"random_loc",
			"dragaggro",
			"never_idle",
			"castdungeon",
			"pull_limit",
			"graveyard_time",
			"max_z",
			"min_expansion",
			"max_expansion",
			"content_flags",
			"content_flags_disabled",
		};
	}

	static std::string ColumnsRaw()
	{
		return std::string(Strings::Implode(", ", Columns()));
	}

	static std::string SelectColumnsRaw()
	{
		return std::string(Strings::Implode(", ", SelectColumns()));
	}

	static std::string TableName()
	{
		return std::string("zone");
	}

	static std::string BaseSelect()
	{
		return fmt::format(
			"SELECT {} FROM {}",
			SelectColumnsRaw(),
			TableName()
		);
	}

	static std::string BaseInsert()
	{
		return fmt::format(
			"INSERT INTO {} ({}) ",
			TableName(),
			ColumnsRaw()
		);
	}

	static Zone NewEntity()
	{
		Zone e{};

		e.short_name             = "";
		e.id                     = 0;
		e.file_name              = "";
		e.long_name              = "";
		e.map_file_name          = "";
		e.safe_x                 = 0;
		e.safe_y                 = 0;
		e.safe_z                 = 0;
		e.safe_heading           = 0;
		e.graveyard_id           = 0;
		e.min_level              = 0;
		e.min_status             = 0;
		e.zoneidnumber           = 0;
		e.timezone               = 0;
		e.maxclients             = 0;
		e.ruleset                = 0;
		e.note                   = "";
		e.underworld             = 0;
		e.minclip                = 450;
		e.maxclip                = 450;
		e.fog_minclip            = 450;
		e.fog_maxclip            = 450;
		e.fog_blue               = 0;
		e.fog_red                = 0;
		e.fog_green              = 0;
		e.sky                    = 1;
		e.ztype                  = 1;
		e.zone_exp_multiplier    = 0.00;
		e.gravity                = 0.4;
		e.time_type              = 2;
		e.fog_red1               = 0;
		e.fog_green1             = 0;
		e.fog_blue1              = 0;
		e.fog_minclip1           = 450;
		e.fog_maxclip1           = 450;
		e.fog_red2               = 0;
		e.fog_green2             = 0;
		e.fog_blue2              = 0;
		e.fog_minclip2           = 450;
		e.fog_maxclip2           = 450;
		e.fog_red3               = 0;
		e.fog_green3             = 0;
		e.fog_blue3              = 0;
		e.fog_minclip3           = 450;
		e.fog_maxclip3           = 450;
		e.fog_red4               = 0;
		e.fog_green4             = 0;
		e.fog_blue4              = 0;
		e.fog_minclip4           = 450;
		e.fog_maxclip4           = 450;
		e.fog_density            = 0;
		e.flag_needed            = "";
		e.canbind                = 1;
		e.cancombat              = 1;
		e.canlevitate            = 1;
		e.castoutdoor            = 1;
		e.hotzone                = 0;
		e.shutdowndelay          = 5000;
		e.peqzone                = 1;
		e.expansion              = 0;
		e.suspendbuffs           = 0;
		e.rain_chance1           = 0;
		e.rain_chance2           = 0;
		e.rain_chance3           = 0;
		e.rain_chance4           = 0;
		e.rain_duration1         = 0;
		e.rain_duration2         = 0;
		e.rain_duration3         = 0;
		e.rain_duration4         = 0;
		e.snow_chance1           = 0;
		e.snow_chance2           = 0;
		e.snow_chance3           = 0;
		e.snow_chance4           = 0;
		e.snow_duration1         = 0;
		e.snow_duration2         = 0;
		e.snow_duration3         = 0;
		e.snow_duration4         = 0;
		e.type                   = 0;
		e.skylock                = 0;
		e.skip_los               = 0;
		e.music                  = 0;
		e.random_loc             = 3;
		e.dragaggro              = 0;
		e.never_idle             = 0;
		e.castdungeon            = 0;
		e.pull_limit             = 80;
		e.graveyard_time         = 1;
		e.max_z                  = 10000;
		e.min_expansion          = -1;
		e.max_expansion          = -1;
		e.content_flags          = "";
		e.content_flags_disabled = "";

		return e;
	}

	static Zone GetZone(
		const std::vector<Zone> &zones,
		int zone_id
	)
	{
		for (auto &zone : zones) {
			if (zone.id == zone_id) {
				return zone;
			}
		}

		return NewEntity();
	}

	static Zone FindOne(
		Database& db,
		int zone_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				zone_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			Zone e{};

			e.short_name             = row[0] ? row[0] : "";
			e.id                     = row[1] ? static_cast<int32_t>(atoi(row[1])) : 0;
			e.file_name              = row[2] ? row[2] : "";
			e.long_name              = row[3] ? row[3] : "";
			e.map_file_name          = row[4] ? row[4] : "";
			e.safe_x                 = row[5] ? strtof(row[5], nullptr) : 0;
			e.safe_y                 = row[6] ? strtof(row[6], nullptr) : 0;
			e.safe_z                 = row[7] ? strtof(row[7], nullptr) : 0;
			e.safe_heading           = row[8] ? strtof(row[8], nullptr) : 0;
			e.graveyard_id           = row[9] ? strtof(row[9], nullptr) : 0;
			e.min_level              = row[10] ? static_cast<uint8_t>(strtoul(row[10], nullptr, 10)) : 0;
			e.min_status             = row[11] ? static_cast<uint8_t>(strtoul(row[11], nullptr, 10)) : 0;
			e.zoneidnumber           = row[12] ? static_cast<int32_t>(atoi(row[12])) : 0;
			e.timezone               = row[13] ? static_cast<int32_t>(atoi(row[13])) : 0;
			e.maxclients             = row[14] ? static_cast<int32_t>(atoi(row[14])) : 0;
			e.ruleset                = row[15] ? static_cast<uint32_t>(strtoul(row[15], nullptr, 10)) : 0;
			e.note                   = row[16] ? row[16] : "";
			e.underworld             = row[17] ? strtof(row[17], nullptr) : 0;
			e.minclip                = row[18] ? strtof(row[18], nullptr) : 450;
			e.maxclip                = row[19] ? strtof(row[19], nullptr) : 450;
			e.fog_minclip            = row[20] ? strtof(row[20], nullptr) : 450;
			e.fog_maxclip            = row[21] ? strtof(row[21], nullptr) : 450;
			e.fog_blue               = row[22] ? static_cast<uint8_t>(strtoul(row[22], nullptr, 10)) : 0;
			e.fog_red                = row[23] ? static_cast<uint8_t>(strtoul(row[23], nullptr, 10)) : 0;
			e.fog_green              = row[24] ? static_cast<uint8_t>(strtoul(row[24], nullptr, 10)) : 0;
			e.sky                    = row[25] ? static_cast<uint8_t>(strtoul(row[25], nullptr, 10)) : 1;
			e.ztype                  = row[26] ? static_cast<uint8_t>(strtoul(row[26], nullptr, 10)) : 1;
			e.zone_exp_multiplier    = row[27] ? strtof(row[27], nullptr) : 0.00;
			e.gravity                = row[28] ? strtof(row[28], nullptr) : 0.4;
			e.time_type              = row[29] ? static_cast<uint8_t>(strtoul(row[29], nullptr, 10)) : 2;
			e.fog_red1               = row[30] ? static_cast<uint8_t>(strtoul(row[30], nullptr, 10)) : 0;
			e.fog_green1             = row[31] ? static_cast<uint8_t>(strtoul(row[31], nullptr, 10)) : 0;
			e.fog_blue1              = row[32] ? static_cast<uint8_t>(strtoul(row[32], nullptr, 10)) : 0;
			e.fog_minclip1           = row[33] ? strtof(row[33], nullptr) : 450;
			e.fog_maxclip1           = row[34] ? strtof(row[34], nullptr) : 450;
			e.fog_red2               = row[35] ? static_cast<uint8_t>(strtoul(row[35], nullptr, 10)) : 0;
			e.fog_green2             = row[36] ? static_cast<uint8_t>(strtoul(row[36], nullptr, 10)) : 0;
			e.fog_blue2              = row[37] ? static_cast<uint8_t>(strtoul(row[37], nullptr, 10)) : 0;
			e.fog_minclip2           = row[38] ? strtof(row[38], nullptr) : 450;
			e.fog_maxclip2           = row[39] ? strtof(row[39], nullptr) : 450;
			e.fog_red3               = row[40] ? static_cast<uint8_t>(strtoul(row[40], nullptr, 10)) : 0;
			e.fog_green3             = row[41] ? static_cast<uint8_t>(strtoul(row[41], nullptr, 10)) : 0;
			e.fog_blue3              = row[42] ? static_cast<uint8_t>(strtoul(row[42], nullptr, 10)) : 0;
			e.fog_minclip3           = row[43] ? strtof(row[43], nullptr) : 450;
			e.fog_maxclip3           = row[44] ? strtof(row[44], nullptr) : 450;
			e.fog_red4               = row[45] ? static_cast<uint8_t>(strtoul(row[45], nullptr, 10)) : 0;
			e.fog_green4             = row[46] ? static_cast<uint8_t>(strtoul(row[46], nullptr, 10)) : 0;
			e.fog_blue4              = row[47] ? static_cast<uint8_t>(strtoul(row[47], nullptr, 10)) : 0;
			e.fog_minclip4           = row[48] ? strtof(row[48], nullptr) : 450;
			e.fog_maxclip4           = row[49] ? strtof(row[49], nullptr) : 450;
			e.fog_density            = row[50] ? strtof(row[50], nullptr) : 0;
			e.flag_needed            = row[51] ? row[51] : "";
			e.canbind                = row[52] ? static_cast<int8_t>(atoi(row[52])) : 1;
			e.cancombat              = row[53] ? static_cast<int8_t>(atoi(row[53])) : 1;
			e.canlevitate            = row[54] ? static_cast<int8_t>(atoi(row[54])) : 1;
			e.castoutdoor            = row[55] ? static_cast<int8_t>(atoi(row[55])) : 1;
			e.hotzone                = row[56] ? static_cast<uint8_t>(strtoul(row[56], nullptr, 10)) : 0;
			e.shutdowndelay          = row[57] ? strtoull(row[57], nullptr, 10) : 5000;
			e.peqzone                = row[58] ? static_cast<int8_t>(atoi(row[58])) : 1;
			e.expansion              = row[59] ? static_cast<int8_t>(atoi(row[59])) : 0;
			e.suspendbuffs           = row[60] ? static_cast<uint8_t>(strtoul(row[60], nullptr, 10)) : 0;
			e.rain_chance1           = row[61] ? static_cast<int32_t>(atoi(row[61])) : 0;
			e.rain_chance2           = row[62] ? static_cast<int32_t>(atoi(row[62])) : 0;
			e.rain_chance3           = row[63] ? static_cast<int32_t>(atoi(row[63])) : 0;
			e.rain_chance4           = row[64] ? static_cast<int32_t>(atoi(row[64])) : 0;
			e.rain_duration1         = row[65] ? static_cast<int32_t>(atoi(row[65])) : 0;
			e.rain_duration2         = row[66] ? static_cast<int32_t>(atoi(row[66])) : 0;
			e.rain_duration3         = row[67] ? static_cast<int32_t>(atoi(row[67])) : 0;
			e.rain_duration4         = row[68] ? static_cast<int32_t>(atoi(row[68])) : 0;
			e.snow_chance1           = row[69] ? static_cast<int32_t>(atoi(row[69])) : 0;
			e.snow_chance2           = row[70] ? static_cast<int32_t>(atoi(row[70])) : 0;
			e.snow_chance3           = row[71] ? static_cast<int32_t>(atoi(row[71])) : 0;
			e.snow_chance4           = row[72] ? static_cast<int32_t>(atoi(row[72])) : 0;
			e.snow_duration1         = row[73] ? static_cast<int32_t>(atoi(row[73])) : 0;
			e.snow_duration2         = row[74] ? static_cast<int32_t>(atoi(row[74])) : 0;
			e.snow_duration3         = row[75] ? static_cast<int32_t>(atoi(row[75])) : 0;
			e.snow_duration4         = row[76] ? static_cast<int32_t>(atoi(row[76])) : 0;
			e.type                   = row[77] ? static_cast<int32_t>(atoi(row[77])) : 0;
			e.skylock                = row[78] ? static_cast<int8_t>(atoi(row[78])) : 0;
			e.skip_los               = row[79] ? static_cast<int8_t>(atoi(row[79])) : 0;
			e.music                  = row[80] ? static_cast<int8_t>(atoi(row[80])) : 0;
			e.random_loc             = row[81] ? static_cast<int8_t>(atoi(row[81])) : 3;
			e.dragaggro              = row[82] ? static_cast<int8_t>(atoi(row[82])) : 0;
			e.never_idle             = row[83] ? static_cast<int8_t>(atoi(row[83])) : 0;
			e.castdungeon            = row[84] ? static_cast<int8_t>(atoi(row[84])) : 0;
			e.pull_limit             = row[85] ? static_cast<uint16_t>(strtoul(row[85], nullptr, 10)) : 80;
			e.graveyard_time         = row[86] ? static_cast<int32_t>(atoi(row[86])) : 1;
			e.max_z                  = row[87] ? strtof(row[87], nullptr) : 10000;
			e.min_expansion          = row[88] ? static_cast<int8_t>(atoi(row[88])) : -1;
			e.max_expansion          = row[89] ? static_cast<int8_t>(atoi(row[89])) : -1;
			e.content_flags          = row[90] ? row[90] : "";
			e.content_flags_disabled = row[91] ? row[91] : "";

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int zone_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				zone_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const Zone &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[0] + " = '" + Strings::Escape(e.short_name) + "'");
		v.push_back(columns[2] + " = '" + Strings::Escape(e.file_name) + "'");
		v.push_back(columns[3] + " = '" + Strings::Escape(e.long_name) + "'");
		v.push_back(columns[4] + " = '" + Strings::Escape(e.map_file_name) + "'");
		v.push_back(columns[5] + " = " + std::to_string(e.safe_x));
		v.push_back(columns[6] + " = " + std::to_string(e.safe_y));
		v.push_back(columns[7] + " = " + std::to_string(e.safe_z));
		v.push_back(columns[8] + " = " + std::to_string(e.safe_heading));
		v.push_back(columns[9] + " = " + std::to_string(e.graveyard_id));
		v.push_back(columns[10] + " = " + std::to_string(e.min_level));
		v.push_back(columns[11] + " = " + std::to_string(e.min_status));
		v.push_back(columns[12] + " = " + std::to_string(e.zoneidnumber));
		v.push_back(columns[13] + " = " + std::to_string(e.timezone));
		v.push_back(columns[14] + " = " + std::to_string(e.maxclients));
		v.push_back(columns[15] + " = " + std::to_string(e.ruleset));
		v.push_back(columns[16] + " = '" + Strings::Escape(e.note) + "'");
		v.push_back(columns[17] + " = " + std::to_string(e.underworld));
		v.push_back(columns[18] + " = " + std::to_string(e.minclip));
		v.push_back(columns[19] + " = " + std::to_string(e.maxclip));
		v.push_back(columns[20] + " = " + std::to_string(e.fog_minclip));
		v.push_back(columns[21] + " = " + std::to_string(e.fog_maxclip));
		v.push_back(columns[22] + " = " + std::to_string(e.fog_blue));
		v.push_back(columns[23] + " = " + std::to_string(e.fog_red));
		v.push_back(columns[24] + " = " + std::to_string(e.fog_green));
		v.push_back(columns[25] + " = " + std::to_string(e.sky));
		v.push_back(columns[26] + " = " + std::to_string(e.ztype));
		v.push_back(columns[27] + " = " + std::to_string(e.zone_exp_multiplier));
		v.push_back(columns[28] + " = " + std::to_string(e.gravity));
		v.push_back(columns[29] + " = " + std::to_string(e.time_type));
		v.push_back(columns[30] + " = " + std::to_string(e.fog_red1));
		v.push_back(columns[31] + " = " + std::to_string(e.fog_green1));
		v.push_back(columns[32] + " = " + std::to_string(e.fog_blue1));
		v.push_back(columns[33] + " = " + std::to_string(e.fog_minclip1));
		v.push_back(columns[34] + " = " + std::to_string(e.fog_maxclip1));
		v.push_back(columns[35] + " = " + std::to_string(e.fog_red2));
		v.push_back(columns[36] + " = " + std::to_string(e.fog_green2));
		v.push_back(columns[37] + " = " + std::to_string(e.fog_blue2));
		v.push_back(columns[38] + " = " + std::to_string(e.fog_minclip2));
		v.push_back(columns[39] + " = " + std::to_string(e.fog_maxclip2));
		v.push_back(columns[40] + " = " + std::to_string(e.fog_red3));
		v.push_back(columns[41] + " = " + std::to_string(e.fog_green3));
		v.push_back(columns[42] + " = " + std::to_string(e.fog_blue3));
		v.push_back(columns[43] + " = " + std::to_string(e.fog_minclip3));
		v.push_back(columns[44] + " = " + std::to_string(e.fog_maxclip3));
		v.push_back(columns[45] + " = " + std::to_string(e.fog_red4));
		v.push_back(columns[46] + " = " + std::to_string(e.fog_green4));
		v.push_back(columns[47] + " = " + std::to_string(e.fog_blue4));
		v.push_back(columns[48] + " = " + std::to_string(e.fog_minclip4));
		v.push_back(columns[49] + " = " + std::to_string(e.fog_maxclip4));
		v.push_back(columns[50] + " = " + std::to_string(e.fog_density));
		v.push_back(columns[51] + " = '" + Strings::Escape(e.flag_needed) + "'");
		v.push_back(columns[52] + " = " + std::to_string(e.canbind));
		v.push_back(columns[53] + " = " + std::to_string(e.cancombat));
		v.push_back(columns[54] + " = " + std::to_string(e.canlevitate));
		v.push_back(columns[55] + " = " + std::to_string(e.castoutdoor));
		v.push_back(columns[56] + " = " + std::to_string(e.hotzone));
		v.push_back(columns[57] + " = " + std::to_string(e.shutdowndelay));
		v.push_back(columns[58] + " = " + std::to_string(e.peqzone));
		v.push_back(columns[59] + " = " + std::to_string(e.expansion));
		v.push_back(columns[60] + " = " + std::to_string(e.suspendbuffs));
		v.push_back(columns[61] + " = " + std::to_string(e.rain_chance1));
		v.push_back(columns[62] + " = " + std::to_string(e.rain_chance2));
		v.push_back(columns[63] + " = " + std::to_string(e.rain_chance3));
		v.push_back(columns[64] + " = " + std::to_string(e.rain_chance4));
		v.push_back(columns[65] + " = " + std::to_string(e.rain_duration1));
		v.push_back(columns[66] + " = " + std::to_string(e.rain_duration2));
		v.push_back(columns[67] + " = " + std::to_string(e.rain_duration3));
		v.push_back(columns[68] + " = " + std::to_string(e.rain_duration4));
		v.push_back(columns[69] + " = " + std::to_string(e.snow_chance1));
		v.push_back(columns[70] + " = " + std::to_string(e.snow_chance2));
		v.push_back(columns[71] + " = " + std::to_string(e.snow_chance3));
		v.push_back(columns[72] + " = " + std::to_string(e.snow_chance4));
		v.push_back(columns[73] + " = " + std::to_string(e.snow_duration1));
		v.push_back(columns[74] + " = " + std::to_string(e.snow_duration2));
		v.push_back(columns[75] + " = " + std::to_string(e.snow_duration3));
		v.push_back(columns[76] + " = " + std::to_string(e.snow_duration4));
		v.push_back(columns[77] + " = " + std::to_string(e.type));
		v.push_back(columns[78] + " = " + std::to_string(e.skylock));
		v.push_back(columns[79] + " = " + std::to_string(e.skip_los));
		v.push_back(columns[80] + " = " + std::to_string(e.music));
		v.push_back(columns[81] + " = " + std::to_string(e.random_loc));
		v.push_back(columns[82] + " = " + std::to_string(e.dragaggro));
		v.push_back(columns[83] + " = " + std::to_string(e.never_idle));
		v.push_back(columns[84] + " = " + std::to_string(e.castdungeon));
		v.push_back(columns[85] + " = " + std::to_string(e.pull_limit));
		v.push_back(columns[86] + " = " + std::to_string(e.graveyard_time));
		v.push_back(columns[87] + " = " + std::to_string(e.max_z));
		v.push_back(columns[88] + " = " + std::to_string(e.min_expansion));
		v.push_back(columns[89] + " = " + std::to_string(e.max_expansion));
		v.push_back(columns[90] + " = '" + Strings::Escape(e.content_flags) + "'");
		v.push_back(columns[91] + " = '" + Strings::Escape(e.content_flags_disabled) + "'");

		auto results = db.QueryDatabase(
			fmt::format(
				"UPDATE {} SET {} WHERE {} = {}",
				TableName(),
				Strings::Implode(", ", v),
				PrimaryKey(),
				e.id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static Zone InsertOne(
		Database& db,
		Zone e
	)
	{
		std::vector<std::string> v;

		v.push_back("'" + Strings::Escape(e.short_name) + "'");
		v.push_back(std::to_string(e.id));
		v.push_back("'" + Strings::Escape(e.file_name) + "'");
		v.push_back("'" + Strings::Escape(e.long_name) + "'");
		v.push_back("'" + Strings::Escape(e.map_file_name) + "'");
		v.push_back(std::to_string(e.safe_x));
		v.push_back(std::to_string(e.safe_y));
		v.push_back(std::to_string(e.safe_z));
		v.push_back(std::to_string(e.safe_heading));
		v.push_back(std::to_string(e.graveyard_id));
		v.push_back(std::to_string(e.min_level));
		v.push_back(std::to_string(e.min_status));
		v.push_back(std::to_string(e.zoneidnumber));
		v.push_back(std::to_string(e.timezone));
		v.push_back(std::to_string(e.maxclients));
		v.push_back(std::to_string(e.ruleset));
		v.push_back("'" + Strings::Escape(e.note) + "'");
		v.push_back(std::to_string(e.underworld));
		v.push_back(std::to_string(e.minclip));
		v.push_back(std::to_string(e.maxclip));
		v.push_back(std::to_string(e.fog_minclip));
		v.push_back(std::to_string(e.fog_maxclip));
		v.push_back(std::to_string(e.fog_blue));
		v.push_back(std::to_string(e.fog_red));
		v.push_back(std::to_string(e.fog_green));
		v.push_back(std::to_string(e.sky));
		v.push_back(std::to_string(e.ztype));
		v.push_back(std::to_string(e.zone_exp_multiplier));
		v.push_back(std::to_string(e.gravity));
		v.push_back(std::to_string(e.time_type));
		v.push_back(std::to_string(e.fog_red1));
		v.push_back(std::to_string(e.fog_green1));
		v.push_back(std::to_string(e.fog_blue1));
		v.push_back(std::to_string(e.fog_minclip1));
		v.push_back(std::to_string(e.fog_maxclip1));
		v.push_back(std::to_string(e.fog_red2));
		v.push_back(std::to_string(e.fog_green2));
		v.push_back(std::to_string(e.fog_blue2));
		v.push_back(std::to_string(e.fog_minclip2));
		v.push_back(std::to_string(e.fog_maxclip2));
		v.push_back(std::to_string(e.fog_red3));
		v.push_back(std::to_string(e.fog_green3));
		v.push_back(std::to_string(e.fog_blue3));
		v.push_back(std::to_string(e.fog_minclip3));
		v.push_back(std::to_string(e.fog_maxclip3));
		v.push_back(std::to_string(e.fog_red4));
		v.push_back(std::to_string(e.fog_green4));
		v.push_back(std::to_string(e.fog_blue4));
		v.push_back(std::to_string(e.fog_minclip4));
		v.push_back(std::to_string(e.fog_maxclip4));
		v.push_back(std::to_string(e.fog_density));
		v.push_back("'" + Strings::Escape(e.flag_needed) + "'");
		v.push_back(std::to_string(e.canbind));
		v.push_back(std::to_string(e.cancombat));
		v.push_back(std::to_string(e.canlevitate));
		v.push_back(std::to_string(e.castoutdoor));
		v.push_back(std::to_string(e.hotzone));
		v.push_back(std::to_string(e.shutdowndelay));
		v.push_back(std::to_string(e.peqzone));
		v.push_back(std::to_string(e.expansion));
		v.push_back(std::to_string(e.suspendbuffs));
		v.push_back(std::to_string(e.rain_chance1));
		v.push_back(std::to_string(e.rain_chance2));
		v.push_back(std::to_string(e.rain_chance3));
		v.push_back(std::to_string(e.rain_chance4));
		v.push_back(std::to_string(e.rain_duration1));
		v.push_back(std::to_string(e.rain_duration2));
		v.push_back(std::to_string(e.rain_duration3));
		v.push_back(std::to_string(e.rain_duration4));
		v.push_back(std::to_string(e.snow_chance1));
		v.push_back(std::to_string(e.snow_chance2));
		v.push_back(std::to_string(e.snow_chance3));
		v.push_back(std::to_string(e.snow_chance4));
		v.push_back(std::to_string(e.snow_duration1));
		v.push_back(std::to_string(e.snow_duration2));
		v.push_back(std::to_string(e.snow_duration3));
		v.push_back(std::to_string(e.snow_duration4));
		v.push_back(std::to_string(e.type));
		v.push_back(std::to_string(e.skylock));
		v.push_back(std::to_string(e.skip_los));
		v.push_back(std::to_string(e.music));
		v.push_back(std::to_string(e.random_loc));
		v.push_back(std::to_string(e.dragaggro));
		v.push_back(std::to_string(e.never_idle));
		v.push_back(std::to_string(e.castdungeon));
		v.push_back(std::to_string(e.pull_limit));
		v.push_back(std::to_string(e.graveyard_time));
		v.push_back(std::to_string(e.max_z));
		v.push_back(std::to_string(e.min_expansion));
		v.push_back(std::to_string(e.max_expansion));
		v.push_back("'" + Strings::Escape(e.content_flags) + "'");
		v.push_back("'" + Strings::Escape(e.content_flags_disabled) + "'");

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES ({})",
				BaseInsert(),
				Strings::Implode(",", v)
			)
		);

		if (results.Success()) {
			e.id = results.LastInsertedID();
			return e;
		}

		e = NewEntity();

		return e;
	}

	static int InsertMany(
		Database& db,
		const std::vector<Zone> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back("'" + Strings::Escape(e.short_name) + "'");
			v.push_back(std::to_string(e.id));
			v.push_back("'" + Strings::Escape(e.file_name) + "'");
			v.push_back("'" + Strings::Escape(e.long_name) + "'");
			v.push_back("'" + Strings::Escape(e.map_file_name) + "'");
			v.push_back(std::to_string(e.safe_x));
			v.push_back(std::to_string(e.safe_y));
			v.push_back(std::to_string(e.safe_z));
			v.push_back(std::to_string(e.safe_heading));
			v.push_back(std::to_string(e.graveyard_id));
			v.push_back(std::to_string(e.min_level));
			v.push_back(std::to_string(e.min_status));
			v.push_back(std::to_string(e.zoneidnumber));
			v.push_back(std::to_string(e.timezone));
			v.push_back(std::to_string(e.maxclients));
			v.push_back(std::to_string(e.ruleset));
			v.push_back("'" + Strings::Escape(e.note) + "'");
			v.push_back(std::to_string(e.underworld));
			v.push_back(std::to_string(e.minclip));
			v.push_back(std::to_string(e.maxclip));
			v.push_back(std::to_string(e.fog_minclip));
			v.push_back(std::to_string(e.fog_maxclip));
			v.push_back(std::to_string(e.fog_blue));
			v.push_back(std::to_string(e.fog_red));
			v.push_back(std::to_string(e.fog_green));
			v.push_back(std::to_string(e.sky));
			v.push_back(std::to_string(e.ztype));
			v.push_back(std::to_string(e.zone_exp_multiplier));
			v.push_back(std::to_string(e.gravity));
			v.push_back(std::to_string(e.time_type));
			v.push_back(std::to_string(e.fog_red1));
			v.push_back(std::to_string(e.fog_green1));
			v.push_back(std::to_string(e.fog_blue1));
			v.push_back(std::to_string(e.fog_minclip1));
			v.push_back(std::to_string(e.fog_maxclip1));
			v.push_back(std::to_string(e.fog_red2));
			v.push_back(std::to_string(e.fog_green2));
			v.push_back(std::to_string(e.fog_blue2));
			v.push_back(std::to_string(e.fog_minclip2));
			v.push_back(std::to_string(e.fog_maxclip2));
			v.push_back(std::to_string(e.fog_red3));
			v.push_back(std::to_string(e.fog_green3));
			v.push_back(std::to_string(e.fog_blue3));
			v.push_back(std::to_string(e.fog_minclip3));
			v.push_back(std::to_string(e.fog_maxclip3));
			v.push_back(std::to_string(e.fog_red4));
			v.push_back(std::to_string(e.fog_green4));
			v.push_back(std::to_string(e.fog_blue4));
			v.push_back(std::to_string(e.fog_minclip4));
			v.push_back(std::to_string(e.fog_maxclip4));
			v.push_back(std::to_string(e.fog_density));
			v.push_back("'" + Strings::Escape(e.flag_needed) + "'");
			v.push_back(std::to_string(e.canbind));
			v.push_back(std::to_string(e.cancombat));
			v.push_back(std::to_string(e.canlevitate));
			v.push_back(std::to_string(e.castoutdoor));
			v.push_back(std::to_string(e.hotzone));
			v.push_back(std::to_string(e.shutdowndelay));
			v.push_back(std::to_string(e.peqzone));
			v.push_back(std::to_string(e.expansion));
			v.push_back(std::to_string(e.suspendbuffs));
			v.push_back(std::to_string(e.rain_chance1));
			v.push_back(std::to_string(e.rain_chance2));
			v.push_back(std::to_string(e.rain_chance3));
			v.push_back(std::to_string(e.rain_chance4));
			v.push_back(std::to_string(e.rain_duration1));
			v.push_back(std::to_string(e.rain_duration2));
			v.push_back(std::to_string(e.rain_duration3));
			v.push_back(std::to_string(e.rain_duration4));
			v.push_back(std::to_string(e.snow_chance1));
			v.push_back(std::to_string(e.snow_chance2));
			v.push_back(std::to_string(e.snow_chance3));
			v.push_back(std::to_string(e.snow_chance4));
			v.push_back(std::to_string(e.snow_duration1));
			v.push_back(std::to_string(e.snow_duration2));
			v.push_back(std::to_string(e.snow_duration3));
			v.push_back(std::to_string(e.snow_duration4));
			v.push_back(std::to_string(e.type));
			v.push_back(std::to_string(e.skylock));
			v.push_back(std::to_string(e.skip_los));
			v.push_back(std::to_string(e.music));
			v.push_back(std::to_string(e.random_loc));
			v.push_back(std::to_string(e.dragaggro));
			v.push_back(std::to_string(e.never_idle));
			v.push_back(std::to_string(e.castdungeon));
			v.push_back(std::to_string(e.pull_limit));
			v.push_back(std::to_string(e.graveyard_time));
			v.push_back(std::to_string(e.max_z));
			v.push_back(std::to_string(e.min_expansion));
			v.push_back(std::to_string(e.max_expansion));
			v.push_back("'" + Strings::Escape(e.content_flags) + "'");
			v.push_back("'" + Strings::Escape(e.content_flags_disabled) + "'");

			insert_chunks.push_back("(" + Strings::Implode(",", v) + ")");
		}

		std::vector<std::string> v;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES {}",
				BaseInsert(),
				Strings::Implode(",", insert_chunks)
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static std::vector<Zone> All(Database& db)
	{
		std::vector<Zone> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			Zone e{};

			e.short_name             = row[0] ? row[0] : "";
			e.id                     = row[1] ? static_cast<int32_t>(atoi(row[1])) : 0;
			e.file_name              = row[2] ? row[2] : "";
			e.long_name              = row[3] ? row[3] : "";
			e.map_file_name          = row[4] ? row[4] : "";
			e.safe_x                 = row[5] ? strtof(row[5], nullptr) : 0;
			e.safe_y                 = row[6] ? strtof(row[6], nullptr) : 0;
			e.safe_z                 = row[7] ? strtof(row[7], nullptr) : 0;
			e.safe_heading           = row[8] ? strtof(row[8], nullptr) : 0;
			e.graveyard_id           = row[9] ? strtof(row[9], nullptr) : 0;
			e.min_level              = row[10] ? static_cast<uint8_t>(strtoul(row[10], nullptr, 10)) : 0;
			e.min_status             = row[11] ? static_cast<uint8_t>(strtoul(row[11], nullptr, 10)) : 0;
			e.zoneidnumber           = row[12] ? static_cast<int32_t>(atoi(row[12])) : 0;
			e.timezone               = row[13] ? static_cast<int32_t>(atoi(row[13])) : 0;
			e.maxclients             = row[14] ? static_cast<int32_t>(atoi(row[14])) : 0;
			e.ruleset                = row[15] ? static_cast<uint32_t>(strtoul(row[15], nullptr, 10)) : 0;
			e.note                   = row[16] ? row[16] : "";
			e.underworld             = row[17] ? strtof(row[17], nullptr) : 0;
			e.minclip                = row[18] ? strtof(row[18], nullptr) : 450;
			e.maxclip                = row[19] ? strtof(row[19], nullptr) : 450;
			e.fog_minclip            = row[20] ? strtof(row[20], nullptr) : 450;
			e.fog_maxclip            = row[21] ? strtof(row[21], nullptr) : 450;
			e.fog_blue               = row[22] ? static_cast<uint8_t>(strtoul(row[22], nullptr, 10)) : 0;
			e.fog_red                = row[23] ? static_cast<uint8_t>(strtoul(row[23], nullptr, 10)) : 0;
			e.fog_green              = row[24] ? static_cast<uint8_t>(strtoul(row[24], nullptr, 10)) : 0;
			e.sky                    = row[25] ? static_cast<uint8_t>(strtoul(row[25], nullptr, 10)) : 1;
			e.ztype                  = row[26] ? static_cast<uint8_t>(strtoul(row[26], nullptr, 10)) : 1;
			e.zone_exp_multiplier    = row[27] ? strtof(row[27], nullptr) : 0.00;
			e.gravity                = row[28] ? strtof(row[28], nullptr) : 0.4;
			e.time_type              = row[29] ? static_cast<uint8_t>(strtoul(row[29], nullptr, 10)) : 2;
			e.fog_red1               = row[30] ? static_cast<uint8_t>(strtoul(row[30], nullptr, 10)) : 0;
			e.fog_green1             = row[31] ? static_cast<uint8_t>(strtoul(row[31], nullptr, 10)) : 0;
			e.fog_blue1              = row[32] ? static_cast<uint8_t>(strtoul(row[32], nullptr, 10)) : 0;
			e.fog_minclip1           = row[33] ? strtof(row[33], nullptr) : 450;
			e.fog_maxclip1           = row[34] ? strtof(row[34], nullptr) : 450;
			e.fog_red2               = row[35] ? static_cast<uint8_t>(strtoul(row[35], nullptr, 10)) : 0;
			e.fog_green2             = row[36] ? static_cast<uint8_t>(strtoul(row[36], nullptr, 10)) : 0;
			e.fog_blue2              = row[37] ? static_cast<uint8_t>(strtoul(row[37], nullptr, 10)) : 0;
			e.fog_minclip2           = row[38] ? strtof(row[38], nullptr) : 450;
			e.fog_maxclip2           = row[39] ? strtof(row[39], nullptr) : 450;
			e.fog_red3               = row[40] ? static_cast<uint8_t>(strtoul(row[40], nullptr, 10)) : 0;
			e.fog_green3             = row[41] ? static_cast<uint8_t>(strtoul(row[41], nullptr, 10)) : 0;
			e.fog_blue3              = row[42] ? static_cast<uint8_t>(strtoul(row[42], nullptr, 10)) : 0;
			e.fog_minclip3           = row[43] ? strtof(row[43], nullptr) : 450;
			e.fog_maxclip3           = row[44] ? strtof(row[44], nullptr) : 450;
			e.fog_red4               = row[45] ? static_cast<uint8_t>(strtoul(row[45], nullptr, 10)) : 0;
			e.fog_green4             = row[46] ? static_cast<uint8_t>(strtoul(row[46], nullptr, 10)) : 0;
			e.fog_blue4              = row[47] ? static_cast<uint8_t>(strtoul(row[47], nullptr, 10)) : 0;
			e.fog_minclip4           = row[48] ? strtof(row[48], nullptr) : 450;
			e.fog_maxclip4           = row[49] ? strtof(row[49], nullptr) : 450;
			e.fog_density            = row[50] ? strtof(row[50], nullptr) : 0;
			e.flag_needed            = row[51] ? row[51] : "";
			e.canbind                = row[52] ? static_cast<int8_t>(atoi(row[52])) : 1;
			e.cancombat              = row[53] ? static_cast<int8_t>(atoi(row[53])) : 1;
			e.canlevitate            = row[54] ? static_cast<int8_t>(atoi(row[54])) : 1;
			e.castoutdoor            = row[55] ? static_cast<int8_t>(atoi(row[55])) : 1;
			e.hotzone                = row[56] ? static_cast<uint8_t>(strtoul(row[56], nullptr, 10)) : 0;
			e.shutdowndelay          = row[57] ? strtoull(row[57], nullptr, 10) : 5000;
			e.peqzone                = row[58] ? static_cast<int8_t>(atoi(row[58])) : 1;
			e.expansion              = row[59] ? static_cast<int8_t>(atoi(row[59])) : 0;
			e.suspendbuffs           = row[60] ? static_cast<uint8_t>(strtoul(row[60], nullptr, 10)) : 0;
			e.rain_chance1           = row[61] ? static_cast<int32_t>(atoi(row[61])) : 0;
			e.rain_chance2           = row[62] ? static_cast<int32_t>(atoi(row[62])) : 0;
			e.rain_chance3           = row[63] ? static_cast<int32_t>(atoi(row[63])) : 0;
			e.rain_chance4           = row[64] ? static_cast<int32_t>(atoi(row[64])) : 0;
			e.rain_duration1         = row[65] ? static_cast<int32_t>(atoi(row[65])) : 0;
			e.rain_duration2         = row[66] ? static_cast<int32_t>(atoi(row[66])) : 0;
			e.rain_duration3         = row[67] ? static_cast<int32_t>(atoi(row[67])) : 0;
			e.rain_duration4         = row[68] ? static_cast<int32_t>(atoi(row[68])) : 0;
			e.snow_chance1           = row[69] ? static_cast<int32_t>(atoi(row[69])) : 0;
			e.snow_chance2           = row[70] ? static_cast<int32_t>(atoi(row[70])) : 0;
			e.snow_chance3           = row[71] ? static_cast<int32_t>(atoi(row[71])) : 0;
			e.snow_chance4           = row[72] ? static_cast<int32_t>(atoi(row[72])) : 0;
			e.snow_duration1         = row[73] ? static_cast<int32_t>(atoi(row[73])) : 0;
			e.snow_duration2         = row[74] ? static_cast<int32_t>(atoi(row[74])) : 0;
			e.snow_duration3         = row[75] ? static_cast<int32_t>(atoi(row[75])) : 0;
			e.snow_duration4         = row[76] ? static_cast<int32_t>(atoi(row[76])) : 0;
			e.type                   = row[77] ? static_cast<int32_t>(atoi(row[77])) : 0;
			e.skylock                = row[78] ? static_cast<int8_t>(atoi(row[78])) : 0;
			e.skip_los               = row[79] ? static_cast<int8_t>(atoi(row[79])) : 0;
			e.music                  = row[80] ? static_cast<int8_t>(atoi(row[80])) : 0;
			e.random_loc             = row[81] ? static_cast<int8_t>(atoi(row[81])) : 3;
			e.dragaggro              = row[82] ? static_cast<int8_t>(atoi(row[82])) : 0;
			e.never_idle             = row[83] ? static_cast<int8_t>(atoi(row[83])) : 0;
			e.castdungeon            = row[84] ? static_cast<int8_t>(atoi(row[84])) : 0;
			e.pull_limit             = row[85] ? static_cast<uint16_t>(strtoul(row[85], nullptr, 10)) : 80;
			e.graveyard_time         = row[86] ? static_cast<int32_t>(atoi(row[86])) : 1;
			e.max_z                  = row[87] ? strtof(row[87], nullptr) : 10000;
			e.min_expansion          = row[88] ? static_cast<int8_t>(atoi(row[88])) : -1;
			e.max_expansion          = row[89] ? static_cast<int8_t>(atoi(row[89])) : -1;
			e.content_flags          = row[90] ? row[90] : "";
			e.content_flags_disabled = row[91] ? row[91] : "";

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<Zone> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<Zone> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			Zone e{};

			e.short_name             = row[0] ? row[0] : "";
			e.id                     = row[1] ? static_cast<int32_t>(atoi(row[1])) : 0;
			e.file_name              = row[2] ? row[2] : "";
			e.long_name              = row[3] ? row[3] : "";
			e.map_file_name          = row[4] ? row[4] : "";
			e.safe_x                 = row[5] ? strtof(row[5], nullptr) : 0;
			e.safe_y                 = row[6] ? strtof(row[6], nullptr) : 0;
			e.safe_z                 = row[7] ? strtof(row[7], nullptr) : 0;
			e.safe_heading           = row[8] ? strtof(row[8], nullptr) : 0;
			e.graveyard_id           = row[9] ? strtof(row[9], nullptr) : 0;
			e.min_level              = row[10] ? static_cast<uint8_t>(strtoul(row[10], nullptr, 10)) : 0;
			e.min_status             = row[11] ? static_cast<uint8_t>(strtoul(row[11], nullptr, 10)) : 0;
			e.zoneidnumber           = row[12] ? static_cast<int32_t>(atoi(row[12])) : 0;
			e.timezone               = row[13] ? static_cast<int32_t>(atoi(row[13])) : 0;
			e.maxclients             = row[14] ? static_cast<int32_t>(atoi(row[14])) : 0;
			e.ruleset                = row[15] ? static_cast<uint32_t>(strtoul(row[15], nullptr, 10)) : 0;
			e.note                   = row[16] ? row[16] : "";
			e.underworld             = row[17] ? strtof(row[17], nullptr) : 0;
			e.minclip                = row[18] ? strtof(row[18], nullptr) : 450;
			e.maxclip                = row[19] ? strtof(row[19], nullptr) : 450;
			e.fog_minclip            = row[20] ? strtof(row[20], nullptr) : 450;
			e.fog_maxclip            = row[21] ? strtof(row[21], nullptr) : 450;
			e.fog_blue               = row[22] ? static_cast<uint8_t>(strtoul(row[22], nullptr, 10)) : 0;
			e.fog_red                = row[23] ? static_cast<uint8_t>(strtoul(row[23], nullptr, 10)) : 0;
			e.fog_green              = row[24] ? static_cast<uint8_t>(strtoul(row[24], nullptr, 10)) : 0;
			e.sky                    = row[25] ? static_cast<uint8_t>(strtoul(row[25], nullptr, 10)) : 1;
			e.ztype                  = row[26] ? static_cast<uint8_t>(strtoul(row[26], nullptr, 10)) : 1;
			e.zone_exp_multiplier    = row[27] ? strtof(row[27], nullptr) : 0.00;
			e.gravity                = row[28] ? strtof(row[28], nullptr) : 0.4;
			e.time_type              = row[29] ? static_cast<uint8_t>(strtoul(row[29], nullptr, 10)) : 2;
			e.fog_red1               = row[30] ? static_cast<uint8_t>(strtoul(row[30], nullptr, 10)) : 0;
			e.fog_green1             = row[31] ? static_cast<uint8_t>(strtoul(row[31], nullptr, 10)) : 0;
			e.fog_blue1              = row[32] ? static_cast<uint8_t>(strtoul(row[32], nullptr, 10)) : 0;
			e.fog_minclip1           = row[33] ? strtof(row[33], nullptr) : 450;
			e.fog_maxclip1           = row[34] ? strtof(row[34], nullptr) : 450;
			e.fog_red2               = row[35] ? static_cast<uint8_t>(strtoul(row[35], nullptr, 10)) : 0;
			e.fog_green2             = row[36] ? static_cast<uint8_t>(strtoul(row[36], nullptr, 10)) : 0;
			e.fog_blue2              = row[37] ? static_cast<uint8_t>(strtoul(row[37], nullptr, 10)) : 0;
			e.fog_minclip2           = row[38] ? strtof(row[38], nullptr) : 450;
			e.fog_maxclip2           = row[39] ? strtof(row[39], nullptr) : 450;
			e.fog_red3               = row[40] ? static_cast<uint8_t>(strtoul(row[40], nullptr, 10)) : 0;
			e.fog_green3             = row[41] ? static_cast<uint8_t>(strtoul(row[41], nullptr, 10)) : 0;
			e.fog_blue3              = row[42] ? static_cast<uint8_t>(strtoul(row[42], nullptr, 10)) : 0;
			e.fog_minclip3           = row[43] ? strtof(row[43], nullptr) : 450;
			e.fog_maxclip3           = row[44] ? strtof(row[44], nullptr) : 450;
			e.fog_red4               = row[45] ? static_cast<uint8_t>(strtoul(row[45], nullptr, 10)) : 0;
			e.fog_green4             = row[46] ? static_cast<uint8_t>(strtoul(row[46], nullptr, 10)) : 0;
			e.fog_blue4              = row[47] ? static_cast<uint8_t>(strtoul(row[47], nullptr, 10)) : 0;
			e.fog_minclip4           = row[48] ? strtof(row[48], nullptr) : 450;
			e.fog_maxclip4           = row[49] ? strtof(row[49], nullptr) : 450;
			e.fog_density            = row[50] ? strtof(row[50], nullptr) : 0;
			e.flag_needed            = row[51] ? row[51] : "";
			e.canbind                = row[52] ? static_cast<int8_t>(atoi(row[52])) : 1;
			e.cancombat              = row[53] ? static_cast<int8_t>(atoi(row[53])) : 1;
			e.canlevitate            = row[54] ? static_cast<int8_t>(atoi(row[54])) : 1;
			e.castoutdoor            = row[55] ? static_cast<int8_t>(atoi(row[55])) : 1;
			e.hotzone                = row[56] ? static_cast<uint8_t>(strtoul(row[56], nullptr, 10)) : 0;
			e.shutdowndelay          = row[57] ? strtoull(row[57], nullptr, 10) : 5000;
			e.peqzone                = row[58] ? static_cast<int8_t>(atoi(row[58])) : 1;
			e.expansion              = row[59] ? static_cast<int8_t>(atoi(row[59])) : 0;
			e.suspendbuffs           = row[60] ? static_cast<uint8_t>(strtoul(row[60], nullptr, 10)) : 0;
			e.rain_chance1           = row[61] ? static_cast<int32_t>(atoi(row[61])) : 0;
			e.rain_chance2           = row[62] ? static_cast<int32_t>(atoi(row[62])) : 0;
			e.rain_chance3           = row[63] ? static_cast<int32_t>(atoi(row[63])) : 0;
			e.rain_chance4           = row[64] ? static_cast<int32_t>(atoi(row[64])) : 0;
			e.rain_duration1         = row[65] ? static_cast<int32_t>(atoi(row[65])) : 0;
			e.rain_duration2         = row[66] ? static_cast<int32_t>(atoi(row[66])) : 0;
			e.rain_duration3         = row[67] ? static_cast<int32_t>(atoi(row[67])) : 0;
			e.rain_duration4         = row[68] ? static_cast<int32_t>(atoi(row[68])) : 0;
			e.snow_chance1           = row[69] ? static_cast<int32_t>(atoi(row[69])) : 0;
			e.snow_chance2           = row[70] ? static_cast<int32_t>(atoi(row[70])) : 0;
			e.snow_chance3           = row[71] ? static_cast<int32_t>(atoi(row[71])) : 0;
			e.snow_chance4           = row[72] ? static_cast<int32_t>(atoi(row[72])) : 0;
			e.snow_duration1         = row[73] ? static_cast<int32_t>(atoi(row[73])) : 0;
			e.snow_duration2         = row[74] ? static_cast<int32_t>(atoi(row[74])) : 0;
			e.snow_duration3         = row[75] ? static_cast<int32_t>(atoi(row[75])) : 0;
			e.snow_duration4         = row[76] ? static_cast<int32_t>(atoi(row[76])) : 0;
			e.type                   = row[77] ? static_cast<int32_t>(atoi(row[77])) : 0;
			e.skylock                = row[78] ? static_cast<int8_t>(atoi(row[78])) : 0;
			e.skip_los               = row[79] ? static_cast<int8_t>(atoi(row[79])) : 0;
			e.music                  = row[80] ? static_cast<int8_t>(atoi(row[80])) : 0;
			e.random_loc             = row[81] ? static_cast<int8_t>(atoi(row[81])) : 3;
			e.dragaggro              = row[82] ? static_cast<int8_t>(atoi(row[82])) : 0;
			e.never_idle             = row[83] ? static_cast<int8_t>(atoi(row[83])) : 0;
			e.castdungeon            = row[84] ? static_cast<int8_t>(atoi(row[84])) : 0;
			e.pull_limit             = row[85] ? static_cast<uint16_t>(strtoul(row[85], nullptr, 10)) : 80;
			e.graveyard_time         = row[86] ? static_cast<int32_t>(atoi(row[86])) : 1;
			e.max_z                  = row[87] ? strtof(row[87], nullptr) : 10000;
			e.min_expansion          = row[88] ? static_cast<int8_t>(atoi(row[88])) : -1;
			e.max_expansion          = row[89] ? static_cast<int8_t>(atoi(row[89])) : -1;
			e.content_flags          = row[90] ? row[90] : "";
			e.content_flags_disabled = row[91] ? row[91] : "";

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static int DeleteWhere(Database& db, const std::string &where_filter)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {}",
				TableName(),
				where_filter
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int Truncate(Database& db)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"TRUNCATE TABLE {}",
				TableName()
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int64 GetMaxId(Database& db)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"SELECT COALESCE(MAX({}), 0) FROM {}",
				PrimaryKey(),
				TableName()
			)
		);

		return (results.Success() && results.begin()[0] ? strtoll(results.begin()[0], nullptr, 10) : 0);
	}

	static int64 Count(Database& db, const std::string &where_filter = "")
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"SELECT COUNT(*) FROM {} {}",
				TableName(),
				(where_filter.empty() ? "" : "WHERE " + where_filter)
			)
		);

		return (results.Success() && results.begin()[0] ? strtoll(results.begin()[0], nullptr, 10) : 0);
	}

	static std::string BaseReplace()
	{
		return fmt::format(
			"REPLACE INTO {} ({}) ",
			TableName(),
			ColumnsRaw()
		);
	}

	static int ReplaceOne(
		Database& db,
		const Zone &e
	)
	{
		std::vector<std::string> v;

		v.push_back("'" + Strings::Escape(e.short_name) + "'");
		v.push_back(std::to_string(e.id));
		v.push_back("'" + Strings::Escape(e.file_name) + "'");
		v.push_back("'" + Strings::Escape(e.long_name) + "'");
		v.push_back("'" + Strings::Escape(e.map_file_name) + "'");
		v.push_back(std::to_string(e.safe_x));
		v.push_back(std::to_string(e.safe_y));
		v.push_back(std::to_string(e.safe_z));
		v.push_back(std::to_string(e.safe_heading));
		v.push_back(std::to_string(e.graveyard_id));
		v.push_back(std::to_string(e.min_level));
		v.push_back(std::to_string(e.min_status));
		v.push_back(std::to_string(e.zoneidnumber));
		v.push_back(std::to_string(e.timezone));
		v.push_back(std::to_string(e.maxclients));
		v.push_back(std::to_string(e.ruleset));
		v.push_back("'" + Strings::Escape(e.note) + "'");
		v.push_back(std::to_string(e.underworld));
		v.push_back(std::to_string(e.minclip));
		v.push_back(std::to_string(e.maxclip));
		v.push_back(std::to_string(e.fog_minclip));
		v.push_back(std::to_string(e.fog_maxclip));
		v.push_back(std::to_string(e.fog_blue));
		v.push_back(std::to_string(e.fog_red));
		v.push_back(std::to_string(e.fog_green));
		v.push_back(std::to_string(e.sky));
		v.push_back(std::to_string(e.ztype));
		v.push_back(std::to_string(e.zone_exp_multiplier));
		v.push_back(std::to_string(e.gravity));
		v.push_back(std::to_string(e.time_type));
		v.push_back(std::to_string(e.fog_red1));
		v.push_back(std::to_string(e.fog_green1));
		v.push_back(std::to_string(e.fog_blue1));
		v.push_back(std::to_string(e.fog_minclip1));
		v.push_back(std::to_string(e.fog_maxclip1));
		v.push_back(std::to_string(e.fog_red2));
		v.push_back(std::to_string(e.fog_green2));
		v.push_back(std::to_string(e.fog_blue2));
		v.push_back(std::to_string(e.fog_minclip2));
		v.push_back(std::to_string(e.fog_maxclip2));
		v.push_back(std::to_string(e.fog_red3));
		v.push_back(std::to_string(e.fog_green3));
		v.push_back(std::to_string(e.fog_blue3));
		v.push_back(std::to_string(e.fog_minclip3));
		v.push_back(std::to_string(e.fog_maxclip3));
		v.push_back(std::to_string(e.fog_red4));
		v.push_back(std::to_string(e.fog_green4));
		v.push_back(std::to_string(e.fog_blue4));
		v.push_back(std::to_string(e.fog_minclip4));
		v.push_back(std::to_string(e.fog_maxclip4));
		v.push_back(std::to_string(e.fog_density));
		v.push_back("'" + Strings::Escape(e.flag_needed) + "'");
		v.push_back(std::to_string(e.canbind));
		v.push_back(std::to_string(e.cancombat));
		v.push_back(std::to_string(e.canlevitate));
		v.push_back(std::to_string(e.castoutdoor));
		v.push_back(std::to_string(e.hotzone));
		v.push_back(std::to_string(e.shutdowndelay));
		v.push_back(std::to_string(e.peqzone));
		v.push_back(std::to_string(e.expansion));
		v.push_back(std::to_string(e.suspendbuffs));
		v.push_back(std::to_string(e.rain_chance1));
		v.push_back(std::to_string(e.rain_chance2));
		v.push_back(std::to_string(e.rain_chance3));
		v.push_back(std::to_string(e.rain_chance4));
		v.push_back(std::to_string(e.rain_duration1));
		v.push_back(std::to_string(e.rain_duration2));
		v.push_back(std::to_string(e.rain_duration3));
		v.push_back(std::to_string(e.rain_duration4));
		v.push_back(std::to_string(e.snow_chance1));
		v.push_back(std::to_string(e.snow_chance2));
		v.push_back(std::to_string(e.snow_chance3));
		v.push_back(std::to_string(e.snow_chance4));
		v.push_back(std::to_string(e.snow_duration1));
		v.push_back(std::to_string(e.snow_duration2));
		v.push_back(std::to_string(e.snow_duration3));
		v.push_back(std::to_string(e.snow_duration4));
		v.push_back(std::to_string(e.type));
		v.push_back(std::to_string(e.skylock));
		v.push_back(std::to_string(e.skip_los));
		v.push_back(std::to_string(e.music));
		v.push_back(std::to_string(e.random_loc));
		v.push_back(std::to_string(e.dragaggro));
		v.push_back(std::to_string(e.never_idle));
		v.push_back(std::to_string(e.castdungeon));
		v.push_back(std::to_string(e.pull_limit));
		v.push_back(std::to_string(e.graveyard_time));
		v.push_back(std::to_string(e.max_z));
		v.push_back(std::to_string(e.min_expansion));
		v.push_back(std::to_string(e.max_expansion));
		v.push_back("'" + Strings::Escape(e.content_flags) + "'");
		v.push_back("'" + Strings::Escape(e.content_flags_disabled) + "'");

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES ({})",
				BaseReplace(),
				Strings::Implode(",", v)
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int ReplaceMany(
		Database& db,
		const std::vector<Zone> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back("'" + Strings::Escape(e.short_name) + "'");
			v.push_back(std::to_string(e.id));
			v.push_back("'" + Strings::Escape(e.file_name) + "'");
			v.push_back("'" + Strings::Escape(e.long_name) + "'");
			v.push_back("'" + Strings::Escape(e.map_file_name) + "'");
			v.push_back(std::to_string(e.safe_x));
			v.push_back(std::to_string(e.safe_y));
			v.push_back(std::to_string(e.safe_z));
			v.push_back(std::to_string(e.safe_heading));
			v.push_back(std::to_string(e.graveyard_id));
			v.push_back(std::to_string(e.min_level));
			v.push_back(std::to_string(e.min_status));
			v.push_back(std::to_string(e.zoneidnumber));
			v.push_back(std::to_string(e.timezone));
			v.push_back(std::to_string(e.maxclients));
			v.push_back(std::to_string(e.ruleset));
			v.push_back("'" + Strings::Escape(e.note) + "'");
			v.push_back(std::to_string(e.underworld));
			v.push_back(std::to_string(e.minclip));
			v.push_back(std::to_string(e.maxclip));
			v.push_back(std::to_string(e.fog_minclip));
			v.push_back(std::to_string(e.fog_maxclip));
			v.push_back(std::to_string(e.fog_blue));
			v.push_back(std::to_string(e.fog_red));
			v.push_back(std::to_string(e.fog_green));
			v.push_back(std::to_string(e.sky));
			v.push_back(std::to_string(e.ztype));
			v.push_back(std::to_string(e.zone_exp_multiplier));
			v.push_back(std::to_string(e.gravity));
			v.push_back(std::to_string(e.time_type));
			v.push_back(std::to_string(e.fog_red1));
			v.push_back(std::to_string(e.fog_green1));
			v.push_back(std::to_string(e.fog_blue1));
			v.push_back(std::to_string(e.fog_minclip1));
			v.push_back(std::to_string(e.fog_maxclip1));
			v.push_back(std::to_string(e.fog_red2));
			v.push_back(std::to_string(e.fog_green2));
			v.push_back(std::to_string(e.fog_blue2));
			v.push_back(std::to_string(e.fog_minclip2));
			v.push_back(std::to_string(e.fog_maxclip2));
			v.push_back(std::to_string(e.fog_red3));
			v.push_back(std::to_string(e.fog_green3));
			v.push_back(std::to_string(e.fog_blue3));
			v.push_back(std::to_string(e.fog_minclip3));
			v.push_back(std::to_string(e.fog_maxclip3));
			v.push_back(std::to_string(e.fog_red4));
			v.push_back(std::to_string(e.fog_green4));
			v.push_back(std::to_string(e.fog_blue4));
			v.push_back(std::to_string(e.fog_minclip4));
			v.push_back(std::to_string(e.fog_maxclip4));
			v.push_back(std::to_string(e.fog_density));
			v.push_back("'" + Strings::Escape(e.flag_needed) + "'");
			v.push_back(std::to_string(e.canbind));
			v.push_back(std::to_string(e.cancombat));
			v.push_back(std::to_string(e.canlevitate));
			v.push_back(std::to_string(e.castoutdoor));
			v.push_back(std::to_string(e.hotzone));
			v.push_back(std::to_string(e.shutdowndelay));
			v.push_back(std::to_string(e.peqzone));
			v.push_back(std::to_string(e.expansion));
			v.push_back(std::to_string(e.suspendbuffs));
			v.push_back(std::to_string(e.rain_chance1));
			v.push_back(std::to_string(e.rain_chance2));
			v.push_back(std::to_string(e.rain_chance3));
			v.push_back(std::to_string(e.rain_chance4));
			v.push_back(std::to_string(e.rain_duration1));
			v.push_back(std::to_string(e.rain_duration2));
			v.push_back(std::to_string(e.rain_duration3));
			v.push_back(std::to_string(e.rain_duration4));
			v.push_back(std::to_string(e.snow_chance1));
			v.push_back(std::to_string(e.snow_chance2));
			v.push_back(std::to_string(e.snow_chance3));
			v.push_back(std::to_string(e.snow_chance4));
			v.push_back(std::to_string(e.snow_duration1));
			v.push_back(std::to_string(e.snow_duration2));
			v.push_back(std::to_string(e.snow_duration3));
			v.push_back(std::to_string(e.snow_duration4));
			v.push_back(std::to_string(e.type));
			v.push_back(std::to_string(e.skylock));
			v.push_back(std::to_string(e.skip_los));
			v.push_back(std::to_string(e.music));
			v.push_back(std::to_string(e.random_loc));
			v.push_back(std::to_string(e.dragaggro));
			v.push_back(std::to_string(e.never_idle));
			v.push_back(std::to_string(e.castdungeon));
			v.push_back(std::to_string(e.pull_limit));
			v.push_back(std::to_string(e.graveyard_time));
			v.push_back(std::to_string(e.max_z));
			v.push_back(std::to_string(e.min_expansion));
			v.push_back(std::to_string(e.max_expansion));
			v.push_back("'" + Strings::Escape(e.content_flags) + "'");
			v.push_back("'" + Strings::Escape(e.content_flags_disabled) + "'");

			insert_chunks.push_back("(" + Strings::Implode(",", v) + ")");
		}

		std::vector<std::string> v;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES {}",
				BaseReplace(),
				Strings::Implode(",", insert_chunks)
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}
};

#endif //EQEMU_BASE_ZONE_REPOSITORY_H
