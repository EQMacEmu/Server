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

#ifndef EQEMU_BASE_FACTION_LIST_REPOSITORY_H
#define EQEMU_BASE_FACTION_LIST_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>


class BaseFactionListRepository {
public:
	struct FactionList {
		int32_t     id;
		std::string name;
		int16_t     base;
		int8_t      see_illusion;
		int16_t     min_cap;
		int16_t     max_cap;
	};

	static std::string PrimaryKey()
	{
		return std::string("id");
	}

	static std::vector<std::string> Columns()
	{
		return {
			"id",
			"name",
			"base",
			"see_illusion",
			"min_cap",
			"max_cap",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"id",
			"name",
			"base",
			"see_illusion",
			"min_cap",
			"max_cap",
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
		return std::string("faction_list");
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

	static FactionList NewEntity()
	{
		FactionList e{};

		e.id           = 0;
		e.name         = "";
		e.base         = 0;
		e.see_illusion = 1;
		e.min_cap      = 0;
		e.max_cap      = 0;

		return e;
	}

	static FactionList GetFactionList(
		const std::vector<FactionList> &faction_lists,
		int faction_list_id
	)
	{
		for (auto &faction_list : faction_lists) {
			if (faction_list.id == faction_list_id) {
				return faction_list;
			}
		}

		return NewEntity();
	}

	static FactionList FindOne(
		Database& db,
		int faction_list_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				faction_list_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			FactionList e{};

			e.id           = static_cast<int32_t>(atoi(row[0]));
			e.name         = row[1] ? row[1] : "";
			e.base         = static_cast<int16_t>(atoi(row[2]));
			e.see_illusion = static_cast<int8_t>(atoi(row[3]));
			e.min_cap      = static_cast<int16_t>(atoi(row[4]));
			e.max_cap      = static_cast<int16_t>(atoi(row[5]));

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int faction_list_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				faction_list_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const FactionList &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[0] + " = " + std::to_string(e.id));
		v.push_back(columns[1] + " = '" + Strings::Escape(e.name) + "'");
		v.push_back(columns[2] + " = " + std::to_string(e.base));
		v.push_back(columns[3] + " = " + std::to_string(e.see_illusion));
		v.push_back(columns[4] + " = " + std::to_string(e.min_cap));
		v.push_back(columns[5] + " = " + std::to_string(e.max_cap));

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

	static FactionList InsertOne(
		Database& db,
		FactionList e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.id));
		v.push_back("'" + Strings::Escape(e.name) + "'");
		v.push_back(std::to_string(e.base));
		v.push_back(std::to_string(e.see_illusion));
		v.push_back(std::to_string(e.min_cap));
		v.push_back(std::to_string(e.max_cap));

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
		const std::vector<FactionList> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.id));
			v.push_back("'" + Strings::Escape(e.name) + "'");
			v.push_back(std::to_string(e.base));
			v.push_back(std::to_string(e.see_illusion));
			v.push_back(std::to_string(e.min_cap));
			v.push_back(std::to_string(e.max_cap));

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

	static std::vector<FactionList> All(Database& db)
	{
		std::vector<FactionList> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			FactionList e{};

			e.id           = static_cast<int32_t>(atoi(row[0]));
			e.name         = row[1] ? row[1] : "";
			e.base         = static_cast<int16_t>(atoi(row[2]));
			e.see_illusion = static_cast<int8_t>(atoi(row[3]));
			e.min_cap      = static_cast<int16_t>(atoi(row[4]));
			e.max_cap      = static_cast<int16_t>(atoi(row[5]));

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<FactionList> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<FactionList> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			FactionList e{};

			e.id           = static_cast<int32_t>(atoi(row[0]));
			e.name         = row[1] ? row[1] : "";
			e.base         = static_cast<int16_t>(atoi(row[2]));
			e.see_illusion = static_cast<int8_t>(atoi(row[3]));
			e.min_cap      = static_cast<int16_t>(atoi(row[4]));
			e.max_cap      = static_cast<int16_t>(atoi(row[5]));

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

#endif //EQEMU_BASE_FACTION_LIST_REPOSITORY_H
