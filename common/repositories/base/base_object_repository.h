/**
 * DO NOT MODIFY THIS FILE
 *
 * This repository was automatically generated and is NOT to be modified directly.
 * Any repository modifications are meant to be made to the repository extending the base.
 * Any modifications to base repositories are to be made by the generator only
 *
 * @generator ./utils/scripts/generators/repository-generator.pl
 * @docs https://eqemu.gitbook.io/server/in-development/developer-area/repositories
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

		e.id         = 0;
		e.zoneid     = 0;
		e.xpos       = 0;
		e.ypos       = 0;
		e.zpos       = 0;
		e.heading    = 0;
		e.itemid     = 0;
		e.charges    = 0;
		e.objectname = "";
		e.type       = 0;
		e.icon       = 0;
		e.size       = 0;
		e.solid      = 0;
		e.incline    = 0;

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

			e.id         = static_cast<int32_t>(atoi(row[0]));
			e.zoneid     = static_cast<uint32_t>(strtoul(row[1], nullptr, 10));
			e.xpos       = strtof(row[2], nullptr);
			e.ypos       = strtof(row[3], nullptr);
			e.zpos       = strtof(row[4], nullptr);
			e.heading    = strtof(row[5], nullptr);
			e.itemid     = static_cast<int32_t>(atoi(row[6]));
			e.charges    = static_cast<uint16_t>(strtoul(row[7], nullptr, 10));
			e.objectname = row[8] ? row[8] : "";
			e.type       = static_cast<int32_t>(atoi(row[9]));
			e.icon       = static_cast<int32_t>(atoi(row[10]));
			e.size       = static_cast<int32_t>(atoi(row[11]));
			e.solid      = static_cast<int32_t>(atoi(row[12]));
			e.incline    = static_cast<int32_t>(atoi(row[13]));

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

			e.id         = static_cast<int32_t>(atoi(row[0]));
			e.zoneid     = static_cast<uint32_t>(strtoul(row[1], nullptr, 10));
			e.xpos       = strtof(row[2], nullptr);
			e.ypos       = strtof(row[3], nullptr);
			e.zpos       = strtof(row[4], nullptr);
			e.heading    = strtof(row[5], nullptr);
			e.itemid     = static_cast<int32_t>(atoi(row[6]));
			e.charges    = static_cast<uint16_t>(strtoul(row[7], nullptr, 10));
			e.objectname = row[8] ? row[8] : "";
			e.type       = static_cast<int32_t>(atoi(row[9]));
			e.icon       = static_cast<int32_t>(atoi(row[10]));
			e.size       = static_cast<int32_t>(atoi(row[11]));
			e.solid      = static_cast<int32_t>(atoi(row[12]));
			e.incline    = static_cast<int32_t>(atoi(row[13]));

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

			e.id         = static_cast<int32_t>(atoi(row[0]));
			e.zoneid     = static_cast<uint32_t>(strtoul(row[1], nullptr, 10));
			e.xpos       = strtof(row[2], nullptr);
			e.ypos       = strtof(row[3], nullptr);
			e.zpos       = strtof(row[4], nullptr);
			e.heading    = strtof(row[5], nullptr);
			e.itemid     = static_cast<int32_t>(atoi(row[6]));
			e.charges    = static_cast<uint16_t>(strtoul(row[7], nullptr, 10));
			e.objectname = row[8] ? row[8] : "";
			e.type       = static_cast<int32_t>(atoi(row[9]));
			e.icon       = static_cast<int32_t>(atoi(row[10]));
			e.size       = static_cast<int32_t>(atoi(row[11]));
			e.solid      = static_cast<int32_t>(atoi(row[12]));
			e.incline    = static_cast<int32_t>(atoi(row[13]));

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

};

#endif //EQEMU_BASE_OBJECT_REPOSITORY_H
