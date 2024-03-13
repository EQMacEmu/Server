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

#ifndef EQEMU_BASE_OBJECT_REPOSITORY_H
#define EQEMU_BASE_OBJECT_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>

class BaseObjectRepository {
public:
	struct Object {
		int32_t     id;
		uint32_t    zoneid;
		float       xpos;
		float       ypos;
		float       zpos;
		float       heading;
		int32_t     itemid;
		uint16_t    charges;
		std::string objectname;
		int32_t     type;
		int32_t     icon;
		int32_t     size;
		int32_t     solid;
		int32_t     incline;
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
			"zoneid",
			"xpos",
			"ypos",
			"zpos",
			"heading",
			"itemid",
			"charges",
			"objectname",
			"type",
			"icon",
			"size",
			"solid",
			"incline",
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
			"zoneid",
			"xpos",
			"ypos",
			"zpos",
			"heading",
			"itemid",
			"charges",
			"objectname",
			"type",
			"icon",
			"size",
			"solid",
			"incline",
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
		return std::string("object");
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

	static Object NewEntity()
	{
		Object e{};

		e.id                     = 0;
		e.zoneid                 = 0;
		e.xpos                   = 0;
		e.ypos                   = 0;
		e.zpos                   = 0;
		e.heading                = 0;
		e.itemid                 = 0;
		e.charges                = 0;
		e.objectname             = "";
		e.type                   = 0;
		e.icon                   = 0;
		e.size                   = 0;
		e.solid                  = 0;
		e.incline                = 0;
		e.min_expansion          = -1;
		e.max_expansion          = -1;
		e.content_flags          = "";
		e.content_flags_disabled = "";

		return e;
	}

	static Object GetObject(
		const std::vector<Object> &objects,
		int object_id
	)
	{
		for (auto &object : objects) {
			if (object.id == object_id) {
				return object;
			}
		}

		return NewEntity();
	}

	static Object FindOne(
		Database& db,
		int object_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				object_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			Object e{};

			e.id                     = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.zoneid                 = row[1] ? static_cast<uint32_t>(strtoul(row[1], nullptr, 10)) : 0;
			e.xpos                   = row[2] ? strtof(row[2], nullptr) : 0;
			e.ypos                   = row[3] ? strtof(row[3], nullptr) : 0;
			e.zpos                   = row[4] ? strtof(row[4], nullptr) : 0;
			e.heading                = row[5] ? strtof(row[5], nullptr) : 0;
			e.itemid                 = row[6] ? static_cast<int32_t>(atoi(row[6])) : 0;
			e.charges                = row[7] ? static_cast<uint16_t>(strtoul(row[7], nullptr, 10)) : 0;
			e.objectname             = row[8] ? row[8] : "";
			e.type                   = row[9] ? static_cast<int32_t>(atoi(row[9])) : 0;
			e.icon                   = row[10] ? static_cast<int32_t>(atoi(row[10])) : 0;
			e.size                   = row[11] ? static_cast<int32_t>(atoi(row[11])) : 0;
			e.solid                  = row[12] ? static_cast<int32_t>(atoi(row[12])) : 0;
			e.incline                = row[13] ? static_cast<int32_t>(atoi(row[13])) : 0;
			e.min_expansion          = row[14] ? static_cast<int8_t>(atoi(row[14])) : -1;
			e.max_expansion          = row[15] ? static_cast<int8_t>(atoi(row[15])) : -1;
			e.content_flags          = row[16] ? row[16] : "";
			e.content_flags_disabled = row[17] ? row[17] : "";

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int object_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				object_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const Object &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[1] + " = " + std::to_string(e.zoneid));
		v.push_back(columns[2] + " = " + std::to_string(e.xpos));
		v.push_back(columns[3] + " = " + std::to_string(e.ypos));
		v.push_back(columns[4] + " = " + std::to_string(e.zpos));
		v.push_back(columns[5] + " = " + std::to_string(e.heading));
		v.push_back(columns[6] + " = " + std::to_string(e.itemid));
		v.push_back(columns[7] + " = " + std::to_string(e.charges));
		v.push_back(columns[8] + " = '" + Strings::Escape(e.objectname) + "'");
		v.push_back(columns[9] + " = " + std::to_string(e.type));
		v.push_back(columns[10] + " = " + std::to_string(e.icon));
		v.push_back(columns[11] + " = " + std::to_string(e.size));
		v.push_back(columns[12] + " = " + std::to_string(e.solid));
		v.push_back(columns[13] + " = " + std::to_string(e.incline));
		v.push_back(columns[14] + " = " + std::to_string(e.min_expansion));
		v.push_back(columns[15] + " = " + std::to_string(e.max_expansion));
		v.push_back(columns[16] + " = '" + Strings::Escape(e.content_flags) + "'");
		v.push_back(columns[17] + " = '" + Strings::Escape(e.content_flags_disabled) + "'");

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

	static Object InsertOne(
		Database& db,
		Object e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.id));
		v.push_back(std::to_string(e.zoneid));
		v.push_back(std::to_string(e.xpos));
		v.push_back(std::to_string(e.ypos));
		v.push_back(std::to_string(e.zpos));
		v.push_back(std::to_string(e.heading));
		v.push_back(std::to_string(e.itemid));
		v.push_back(std::to_string(e.charges));
		v.push_back("'" + Strings::Escape(e.objectname) + "'");
		v.push_back(std::to_string(e.type));
		v.push_back(std::to_string(e.icon));
		v.push_back(std::to_string(e.size));
		v.push_back(std::to_string(e.solid));
		v.push_back(std::to_string(e.incline));
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
		const std::vector<Object> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.id));
			v.push_back(std::to_string(e.zoneid));
			v.push_back(std::to_string(e.xpos));
			v.push_back(std::to_string(e.ypos));
			v.push_back(std::to_string(e.zpos));
			v.push_back(std::to_string(e.heading));
			v.push_back(std::to_string(e.itemid));
			v.push_back(std::to_string(e.charges));
			v.push_back("'" + Strings::Escape(e.objectname) + "'");
			v.push_back(std::to_string(e.type));
			v.push_back(std::to_string(e.icon));
			v.push_back(std::to_string(e.size));
			v.push_back(std::to_string(e.solid));
			v.push_back(std::to_string(e.incline));
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

	static std::vector<Object> All(Database& db)
	{
		std::vector<Object> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			Object e{};

			e.id                     = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.zoneid                 = row[1] ? static_cast<uint32_t>(strtoul(row[1], nullptr, 10)) : 0;
			e.xpos                   = row[2] ? strtof(row[2], nullptr) : 0;
			e.ypos                   = row[3] ? strtof(row[3], nullptr) : 0;
			e.zpos                   = row[4] ? strtof(row[4], nullptr) : 0;
			e.heading                = row[5] ? strtof(row[5], nullptr) : 0;
			e.itemid                 = row[6] ? static_cast<int32_t>(atoi(row[6])) : 0;
			e.charges                = row[7] ? static_cast<uint16_t>(strtoul(row[7], nullptr, 10)) : 0;
			e.objectname             = row[8] ? row[8] : "";
			e.type                   = row[9] ? static_cast<int32_t>(atoi(row[9])) : 0;
			e.icon                   = row[10] ? static_cast<int32_t>(atoi(row[10])) : 0;
			e.size                   = row[11] ? static_cast<int32_t>(atoi(row[11])) : 0;
			e.solid                  = row[12] ? static_cast<int32_t>(atoi(row[12])) : 0;
			e.incline                = row[13] ? static_cast<int32_t>(atoi(row[13])) : 0;
			e.min_expansion          = row[14] ? static_cast<int8_t>(atoi(row[14])) : -1;
			e.max_expansion          = row[15] ? static_cast<int8_t>(atoi(row[15])) : -1;
			e.content_flags          = row[16] ? row[16] : "";
			e.content_flags_disabled = row[17] ? row[17] : "";

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<Object> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<Object> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			Object e{};

			e.id                     = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.zoneid                 = row[1] ? static_cast<uint32_t>(strtoul(row[1], nullptr, 10)) : 0;
			e.xpos                   = row[2] ? strtof(row[2], nullptr) : 0;
			e.ypos                   = row[3] ? strtof(row[3], nullptr) : 0;
			e.zpos                   = row[4] ? strtof(row[4], nullptr) : 0;
			e.heading                = row[5] ? strtof(row[5], nullptr) : 0;
			e.itemid                 = row[6] ? static_cast<int32_t>(atoi(row[6])) : 0;
			e.charges                = row[7] ? static_cast<uint16_t>(strtoul(row[7], nullptr, 10)) : 0;
			e.objectname             = row[8] ? row[8] : "";
			e.type                   = row[9] ? static_cast<int32_t>(atoi(row[9])) : 0;
			e.icon                   = row[10] ? static_cast<int32_t>(atoi(row[10])) : 0;
			e.size                   = row[11] ? static_cast<int32_t>(atoi(row[11])) : 0;
			e.solid                  = row[12] ? static_cast<int32_t>(atoi(row[12])) : 0;
			e.incline                = row[13] ? static_cast<int32_t>(atoi(row[13])) : 0;
			e.min_expansion          = row[14] ? static_cast<int8_t>(atoi(row[14])) : -1;
			e.max_expansion          = row[15] ? static_cast<int8_t>(atoi(row[15])) : -1;
			e.content_flags          = row[16] ? row[16] : "";
			e.content_flags_disabled = row[17] ? row[17] : "";

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
		const Object &e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.id));
		v.push_back(std::to_string(e.zoneid));
		v.push_back(std::to_string(e.xpos));
		v.push_back(std::to_string(e.ypos));
		v.push_back(std::to_string(e.zpos));
		v.push_back(std::to_string(e.heading));
		v.push_back(std::to_string(e.itemid));
		v.push_back(std::to_string(e.charges));
		v.push_back("'" + Strings::Escape(e.objectname) + "'");
		v.push_back(std::to_string(e.type));
		v.push_back(std::to_string(e.icon));
		v.push_back(std::to_string(e.size));
		v.push_back(std::to_string(e.solid));
		v.push_back(std::to_string(e.incline));
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
		const std::vector<Object> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.id));
			v.push_back(std::to_string(e.zoneid));
			v.push_back(std::to_string(e.xpos));
			v.push_back(std::to_string(e.ypos));
			v.push_back(std::to_string(e.zpos));
			v.push_back(std::to_string(e.heading));
			v.push_back(std::to_string(e.itemid));
			v.push_back(std::to_string(e.charges));
			v.push_back("'" + Strings::Escape(e.objectname) + "'");
			v.push_back(std::to_string(e.type));
			v.push_back(std::to_string(e.icon));
			v.push_back(std::to_string(e.size));
			v.push_back(std::to_string(e.solid));
			v.push_back(std::to_string(e.incline));
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

#endif //EQEMU_BASE_OBJECT_REPOSITORY_H
