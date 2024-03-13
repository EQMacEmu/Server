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

#ifndef EQEMU_BASE_DOORS_REPOSITORY_H
#define EQEMU_BASE_DOORS_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>

class BaseDoorsRepository {
public:
	struct Doors {
		int32_t     id;
		int16_t     doorid;
		std::string zone;
		std::string name;
		float       pos_y;
		float       pos_x;
		float       pos_z;
		float       heading;
		int16_t     opentype;
		int16_t     lockpick;
		int32_t     keyitem;
		int32_t     altkeyitem;
		int8_t      nokeyring;
		int16_t     triggerdoor;
		int16_t     triggertype;
		int16_t     doorisopen;
		int32_t     door_param;
		std::string dest_zone;
		float       dest_x;
		float       dest_y;
		float       dest_z;
		float       dest_heading;
		int32_t     invert_state;
		int32_t     incline;
		uint16_t    size;
		uint32_t    client_version_mask;
		int8_t      islift;
		int32_t     close_time;
		int8_t      can_open;
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
			"id",
			"doorid",
			"zone",
			"name",
			"pos_y",
			"pos_x",
			"pos_z",
			"heading",
			"opentype",
			"lockpick",
			"keyitem",
			"altkeyitem",
			"nokeyring",
			"triggerdoor",
			"triggertype",
			"doorisopen",
			"door_param",
			"dest_zone",
			"dest_x",
			"dest_y",
			"dest_z",
			"dest_heading",
			"invert_state",
			"incline",
			"size",
			"client_version_mask",
			"islift",
			"close_time",
			"can_open",
			"min_expansion",
			"max_expansion",
			"content_flags",
			"content_flags_disabled",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"id",
			"doorid",
			"zone",
			"name",
			"pos_y",
			"pos_x",
			"pos_z",
			"heading",
			"opentype",
			"lockpick",
			"keyitem",
			"altkeyitem",
			"nokeyring",
			"triggerdoor",
			"triggertype",
			"doorisopen",
			"door_param",
			"dest_zone",
			"dest_x",
			"dest_y",
			"dest_z",
			"dest_heading",
			"invert_state",
			"incline",
			"size",
			"client_version_mask",
			"islift",
			"close_time",
			"can_open",
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
		return std::string("doors");
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

	static Doors NewEntity()
	{
		Doors e{};

		e.id                     = 0;
		e.doorid                 = 0;
		e.zone                   = "";
		e.name                   = "";
		e.pos_y                  = 0;
		e.pos_x                  = 0;
		e.pos_z                  = 0;
		e.heading                = 0;
		e.opentype               = 0;
		e.lockpick               = 0;
		e.keyitem                = 0;
		e.altkeyitem             = 0;
		e.nokeyring              = 1;
		e.triggerdoor            = 0;
		e.triggertype            = 0;
		e.doorisopen             = 0;
		e.door_param             = 0;
		e.dest_zone              = "NONE";
		e.dest_x                 = 0;
		e.dest_y                 = 0;
		e.dest_z                 = 0;
		e.dest_heading           = 0;
		e.invert_state           = 0;
		e.incline                = 0;
		e.size                   = 100;
		e.client_version_mask    = 4294967295;
		e.islift                 = 0;
		e.close_time             = 5;
		e.can_open               = 1;
		e.min_expansion          = -1;
		e.max_expansion          = -1;
		e.content_flags          = "";
		e.content_flags_disabled = "";

		return e;
	}

	static Doors GetDoors(
		const std::vector<Doors> &doorss,
		int doors_id
	)
	{
		for (auto &doors : doorss) {
			if (doors.id == doors_id) {
				return doors;
			}
		}

		return NewEntity();
	}

	static Doors FindOne(
		Database& db,
		int doors_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				doors_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			Doors e{};

			e.id                     = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.doorid                 = row[1] ? static_cast<int16_t>(atoi(row[1])) : 0;
			e.zone                   = row[2] ? row[2] : "";
			e.name                   = row[3] ? row[3] : "";
			e.pos_y                  = row[4] ? strtof(row[4], nullptr) : 0;
			e.pos_x                  = row[5] ? strtof(row[5], nullptr) : 0;
			e.pos_z                  = row[6] ? strtof(row[6], nullptr) : 0;
			e.heading                = row[7] ? strtof(row[7], nullptr) : 0;
			e.opentype               = row[8] ? static_cast<int16_t>(atoi(row[8])) : 0;
			e.lockpick               = row[9] ? static_cast<int16_t>(atoi(row[9])) : 0;
			e.keyitem                = row[10] ? static_cast<int32_t>(atoi(row[10])) : 0;
			e.altkeyitem             = row[11] ? static_cast<int32_t>(atoi(row[11])) : 0;
			e.nokeyring              = row[12] ? static_cast<int8_t>(atoi(row[12])) : 1;
			e.triggerdoor            = row[13] ? static_cast<int16_t>(atoi(row[13])) : 0;
			e.triggertype            = row[14] ? static_cast<int16_t>(atoi(row[14])) : 0;
			e.doorisopen             = row[15] ? static_cast<int16_t>(atoi(row[15])) : 0;
			e.door_param             = row[16] ? static_cast<int32_t>(atoi(row[16])) : 0;
			e.dest_zone              = row[17] ? row[17] : "NONE";
			e.dest_x                 = row[18] ? strtof(row[18], nullptr) : 0;
			e.dest_y                 = row[19] ? strtof(row[19], nullptr) : 0;
			e.dest_z                 = row[20] ? strtof(row[20], nullptr) : 0;
			e.dest_heading           = row[21] ? strtof(row[21], nullptr) : 0;
			e.invert_state           = row[22] ? static_cast<int32_t>(atoi(row[22])) : 0;
			e.incline                = row[23] ? static_cast<int32_t>(atoi(row[23])) : 0;
			e.size                   = row[24] ? static_cast<uint16_t>(strtoul(row[24], nullptr, 10)) : 100;
			e.client_version_mask    = row[25] ? static_cast<uint32_t>(strtoul(row[25], nullptr, 10)) : 4294967295;
			e.islift                 = row[26] ? static_cast<int8_t>(atoi(row[26])) : 0;
			e.close_time             = row[27] ? static_cast<int32_t>(atoi(row[27])) : 5;
			e.can_open               = row[28] ? static_cast<int8_t>(atoi(row[28])) : 1;
			e.min_expansion          = row[29] ? static_cast<int8_t>(atoi(row[29])) : -1;
			e.max_expansion          = row[30] ? static_cast<int8_t>(atoi(row[30])) : -1;
			e.content_flags          = row[31] ? row[31] : "";
			e.content_flags_disabled = row[32] ? row[32] : "";

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int doors_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				doors_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const Doors &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[1] + " = " + std::to_string(e.doorid));
		v.push_back(columns[2] + " = '" + Strings::Escape(e.zone) + "'");
		v.push_back(columns[3] + " = '" + Strings::Escape(e.name) + "'");
		v.push_back(columns[4] + " = " + std::to_string(e.pos_y));
		v.push_back(columns[5] + " = " + std::to_string(e.pos_x));
		v.push_back(columns[6] + " = " + std::to_string(e.pos_z));
		v.push_back(columns[7] + " = " + std::to_string(e.heading));
		v.push_back(columns[8] + " = " + std::to_string(e.opentype));
		v.push_back(columns[9] + " = " + std::to_string(e.lockpick));
		v.push_back(columns[10] + " = " + std::to_string(e.keyitem));
		v.push_back(columns[11] + " = " + std::to_string(e.altkeyitem));
		v.push_back(columns[12] + " = " + std::to_string(e.nokeyring));
		v.push_back(columns[13] + " = " + std::to_string(e.triggerdoor));
		v.push_back(columns[14] + " = " + std::to_string(e.triggertype));
		v.push_back(columns[15] + " = " + std::to_string(e.doorisopen));
		v.push_back(columns[16] + " = " + std::to_string(e.door_param));
		v.push_back(columns[17] + " = '" + Strings::Escape(e.dest_zone) + "'");
		v.push_back(columns[18] + " = " + std::to_string(e.dest_x));
		v.push_back(columns[19] + " = " + std::to_string(e.dest_y));
		v.push_back(columns[20] + " = " + std::to_string(e.dest_z));
		v.push_back(columns[21] + " = " + std::to_string(e.dest_heading));
		v.push_back(columns[22] + " = " + std::to_string(e.invert_state));
		v.push_back(columns[23] + " = " + std::to_string(e.incline));
		v.push_back(columns[24] + " = " + std::to_string(e.size));
		v.push_back(columns[25] + " = " + std::to_string(e.client_version_mask));
		v.push_back(columns[26] + " = " + std::to_string(e.islift));
		v.push_back(columns[27] + " = " + std::to_string(e.close_time));
		v.push_back(columns[28] + " = " + std::to_string(e.can_open));
		v.push_back(columns[29] + " = " + std::to_string(e.min_expansion));
		v.push_back(columns[30] + " = " + std::to_string(e.max_expansion));
		v.push_back(columns[31] + " = '" + Strings::Escape(e.content_flags) + "'");
		v.push_back(columns[32] + " = '" + Strings::Escape(e.content_flags_disabled) + "'");

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

	static Doors InsertOne(
		Database& db,
		Doors e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.id));
		v.push_back(std::to_string(e.doorid));
		v.push_back("'" + Strings::Escape(e.zone) + "'");
		v.push_back("'" + Strings::Escape(e.name) + "'");
		v.push_back(std::to_string(e.pos_y));
		v.push_back(std::to_string(e.pos_x));
		v.push_back(std::to_string(e.pos_z));
		v.push_back(std::to_string(e.heading));
		v.push_back(std::to_string(e.opentype));
		v.push_back(std::to_string(e.lockpick));
		v.push_back(std::to_string(e.keyitem));
		v.push_back(std::to_string(e.altkeyitem));
		v.push_back(std::to_string(e.nokeyring));
		v.push_back(std::to_string(e.triggerdoor));
		v.push_back(std::to_string(e.triggertype));
		v.push_back(std::to_string(e.doorisopen));
		v.push_back(std::to_string(e.door_param));
		v.push_back("'" + Strings::Escape(e.dest_zone) + "'");
		v.push_back(std::to_string(e.dest_x));
		v.push_back(std::to_string(e.dest_y));
		v.push_back(std::to_string(e.dest_z));
		v.push_back(std::to_string(e.dest_heading));
		v.push_back(std::to_string(e.invert_state));
		v.push_back(std::to_string(e.incline));
		v.push_back(std::to_string(e.size));
		v.push_back(std::to_string(e.client_version_mask));
		v.push_back(std::to_string(e.islift));
		v.push_back(std::to_string(e.close_time));
		v.push_back(std::to_string(e.can_open));
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
		const std::vector<Doors> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.id));
			v.push_back(std::to_string(e.doorid));
			v.push_back("'" + Strings::Escape(e.zone) + "'");
			v.push_back("'" + Strings::Escape(e.name) + "'");
			v.push_back(std::to_string(e.pos_y));
			v.push_back(std::to_string(e.pos_x));
			v.push_back(std::to_string(e.pos_z));
			v.push_back(std::to_string(e.heading));
			v.push_back(std::to_string(e.opentype));
			v.push_back(std::to_string(e.lockpick));
			v.push_back(std::to_string(e.keyitem));
			v.push_back(std::to_string(e.altkeyitem));
			v.push_back(std::to_string(e.nokeyring));
			v.push_back(std::to_string(e.triggerdoor));
			v.push_back(std::to_string(e.triggertype));
			v.push_back(std::to_string(e.doorisopen));
			v.push_back(std::to_string(e.door_param));
			v.push_back("'" + Strings::Escape(e.dest_zone) + "'");
			v.push_back(std::to_string(e.dest_x));
			v.push_back(std::to_string(e.dest_y));
			v.push_back(std::to_string(e.dest_z));
			v.push_back(std::to_string(e.dest_heading));
			v.push_back(std::to_string(e.invert_state));
			v.push_back(std::to_string(e.incline));
			v.push_back(std::to_string(e.size));
			v.push_back(std::to_string(e.client_version_mask));
			v.push_back(std::to_string(e.islift));
			v.push_back(std::to_string(e.close_time));
			v.push_back(std::to_string(e.can_open));
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

	static std::vector<Doors> All(Database& db)
	{
		std::vector<Doors> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			Doors e{};

			e.id                     = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.doorid                 = row[1] ? static_cast<int16_t>(atoi(row[1])) : 0;
			e.zone                   = row[2] ? row[2] : "";
			e.name                   = row[3] ? row[3] : "";
			e.pos_y                  = row[4] ? strtof(row[4], nullptr) : 0;
			e.pos_x                  = row[5] ? strtof(row[5], nullptr) : 0;
			e.pos_z                  = row[6] ? strtof(row[6], nullptr) : 0;
			e.heading                = row[7] ? strtof(row[7], nullptr) : 0;
			e.opentype               = row[8] ? static_cast<int16_t>(atoi(row[8])) : 0;
			e.lockpick               = row[9] ? static_cast<int16_t>(atoi(row[9])) : 0;
			e.keyitem                = row[10] ? static_cast<int32_t>(atoi(row[10])) : 0;
			e.altkeyitem             = row[11] ? static_cast<int32_t>(atoi(row[11])) : 0;
			e.nokeyring              = row[12] ? static_cast<int8_t>(atoi(row[12])) : 1;
			e.triggerdoor            = row[13] ? static_cast<int16_t>(atoi(row[13])) : 0;
			e.triggertype            = row[14] ? static_cast<int16_t>(atoi(row[14])) : 0;
			e.doorisopen             = row[15] ? static_cast<int16_t>(atoi(row[15])) : 0;
			e.door_param             = row[16] ? static_cast<int32_t>(atoi(row[16])) : 0;
			e.dest_zone              = row[17] ? row[17] : "NONE";
			e.dest_x                 = row[18] ? strtof(row[18], nullptr) : 0;
			e.dest_y                 = row[19] ? strtof(row[19], nullptr) : 0;
			e.dest_z                 = row[20] ? strtof(row[20], nullptr) : 0;
			e.dest_heading           = row[21] ? strtof(row[21], nullptr) : 0;
			e.invert_state           = row[22] ? static_cast<int32_t>(atoi(row[22])) : 0;
			e.incline                = row[23] ? static_cast<int32_t>(atoi(row[23])) : 0;
			e.size                   = row[24] ? static_cast<uint16_t>(strtoul(row[24], nullptr, 10)) : 100;
			e.client_version_mask    = row[25] ? static_cast<uint32_t>(strtoul(row[25], nullptr, 10)) : 4294967295;
			e.islift                 = row[26] ? static_cast<int8_t>(atoi(row[26])) : 0;
			e.close_time             = row[27] ? static_cast<int32_t>(atoi(row[27])) : 5;
			e.can_open               = row[28] ? static_cast<int8_t>(atoi(row[28])) : 1;
			e.min_expansion          = row[29] ? static_cast<int8_t>(atoi(row[29])) : -1;
			e.max_expansion          = row[30] ? static_cast<int8_t>(atoi(row[30])) : -1;
			e.content_flags          = row[31] ? row[31] : "";
			e.content_flags_disabled = row[32] ? row[32] : "";

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<Doors> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<Doors> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			Doors e{};

			e.id                     = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.doorid                 = row[1] ? static_cast<int16_t>(atoi(row[1])) : 0;
			e.zone                   = row[2] ? row[2] : "";
			e.name                   = row[3] ? row[3] : "";
			e.pos_y                  = row[4] ? strtof(row[4], nullptr) : 0;
			e.pos_x                  = row[5] ? strtof(row[5], nullptr) : 0;
			e.pos_z                  = row[6] ? strtof(row[6], nullptr) : 0;
			e.heading                = row[7] ? strtof(row[7], nullptr) : 0;
			e.opentype               = row[8] ? static_cast<int16_t>(atoi(row[8])) : 0;
			e.lockpick               = row[9] ? static_cast<int16_t>(atoi(row[9])) : 0;
			e.keyitem                = row[10] ? static_cast<int32_t>(atoi(row[10])) : 0;
			e.altkeyitem             = row[11] ? static_cast<int32_t>(atoi(row[11])) : 0;
			e.nokeyring              = row[12] ? static_cast<int8_t>(atoi(row[12])) : 1;
			e.triggerdoor            = row[13] ? static_cast<int16_t>(atoi(row[13])) : 0;
			e.triggertype            = row[14] ? static_cast<int16_t>(atoi(row[14])) : 0;
			e.doorisopen             = row[15] ? static_cast<int16_t>(atoi(row[15])) : 0;
			e.door_param             = row[16] ? static_cast<int32_t>(atoi(row[16])) : 0;
			e.dest_zone              = row[17] ? row[17] : "NONE";
			e.dest_x                 = row[18] ? strtof(row[18], nullptr) : 0;
			e.dest_y                 = row[19] ? strtof(row[19], nullptr) : 0;
			e.dest_z                 = row[20] ? strtof(row[20], nullptr) : 0;
			e.dest_heading           = row[21] ? strtof(row[21], nullptr) : 0;
			e.invert_state           = row[22] ? static_cast<int32_t>(atoi(row[22])) : 0;
			e.incline                = row[23] ? static_cast<int32_t>(atoi(row[23])) : 0;
			e.size                   = row[24] ? static_cast<uint16_t>(strtoul(row[24], nullptr, 10)) : 100;
			e.client_version_mask    = row[25] ? static_cast<uint32_t>(strtoul(row[25], nullptr, 10)) : 4294967295;
			e.islift                 = row[26] ? static_cast<int8_t>(atoi(row[26])) : 0;
			e.close_time             = row[27] ? static_cast<int32_t>(atoi(row[27])) : 5;
			e.can_open               = row[28] ? static_cast<int8_t>(atoi(row[28])) : 1;
			e.min_expansion          = row[29] ? static_cast<int8_t>(atoi(row[29])) : -1;
			e.max_expansion          = row[30] ? static_cast<int8_t>(atoi(row[30])) : -1;
			e.content_flags          = row[31] ? row[31] : "";
			e.content_flags_disabled = row[32] ? row[32] : "";

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
		const Doors &e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.id));
		v.push_back(std::to_string(e.doorid));
		v.push_back("'" + Strings::Escape(e.zone) + "'");
		v.push_back("'" + Strings::Escape(e.name) + "'");
		v.push_back(std::to_string(e.pos_y));
		v.push_back(std::to_string(e.pos_x));
		v.push_back(std::to_string(e.pos_z));
		v.push_back(std::to_string(e.heading));
		v.push_back(std::to_string(e.opentype));
		v.push_back(std::to_string(e.lockpick));
		v.push_back(std::to_string(e.keyitem));
		v.push_back(std::to_string(e.altkeyitem));
		v.push_back(std::to_string(e.nokeyring));
		v.push_back(std::to_string(e.triggerdoor));
		v.push_back(std::to_string(e.triggertype));
		v.push_back(std::to_string(e.doorisopen));
		v.push_back(std::to_string(e.door_param));
		v.push_back("'" + Strings::Escape(e.dest_zone) + "'");
		v.push_back(std::to_string(e.dest_x));
		v.push_back(std::to_string(e.dest_y));
		v.push_back(std::to_string(e.dest_z));
		v.push_back(std::to_string(e.dest_heading));
		v.push_back(std::to_string(e.invert_state));
		v.push_back(std::to_string(e.incline));
		v.push_back(std::to_string(e.size));
		v.push_back(std::to_string(e.client_version_mask));
		v.push_back(std::to_string(e.islift));
		v.push_back(std::to_string(e.close_time));
		v.push_back(std::to_string(e.can_open));
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
		const std::vector<Doors> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.id));
			v.push_back(std::to_string(e.doorid));
			v.push_back("'" + Strings::Escape(e.zone) + "'");
			v.push_back("'" + Strings::Escape(e.name) + "'");
			v.push_back(std::to_string(e.pos_y));
			v.push_back(std::to_string(e.pos_x));
			v.push_back(std::to_string(e.pos_z));
			v.push_back(std::to_string(e.heading));
			v.push_back(std::to_string(e.opentype));
			v.push_back(std::to_string(e.lockpick));
			v.push_back(std::to_string(e.keyitem));
			v.push_back(std::to_string(e.altkeyitem));
			v.push_back(std::to_string(e.nokeyring));
			v.push_back(std::to_string(e.triggerdoor));
			v.push_back(std::to_string(e.triggertype));
			v.push_back(std::to_string(e.doorisopen));
			v.push_back(std::to_string(e.door_param));
			v.push_back("'" + Strings::Escape(e.dest_zone) + "'");
			v.push_back(std::to_string(e.dest_x));
			v.push_back(std::to_string(e.dest_y));
			v.push_back(std::to_string(e.dest_z));
			v.push_back(std::to_string(e.dest_heading));
			v.push_back(std::to_string(e.invert_state));
			v.push_back(std::to_string(e.incline));
			v.push_back(std::to_string(e.size));
			v.push_back(std::to_string(e.client_version_mask));
			v.push_back(std::to_string(e.islift));
			v.push_back(std::to_string(e.close_time));
			v.push_back(std::to_string(e.can_open));
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

#endif //EQEMU_BASE_DOORS_REPOSITORY_H
