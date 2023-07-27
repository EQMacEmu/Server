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

#ifndef EQEMU_BASE_AA_REQUIRED_LEVEL_COST_REPOSITORY_H
#define EQEMU_BASE_AA_REQUIRED_LEVEL_COST_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>


class BaseAaRequiredLevelCostRepository {
public:
	struct AaRequiredLevelCost {
		uint32_t    skill_id;
		uint32_t    level;
		uint32_t    cost;
		std::string description;
		int32_t     eqmacid;
	};

	static std::string PrimaryKey()
	{
		return std::string("skill_id");
	}

	static std::vector<std::string> Columns()
	{
		return {
			"skill_id",
			"level",
			"cost",
			"description",
			"eqmacid",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"skill_id",
			"level",
			"cost",
			"description",
			"eqmacid",
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
		return std::string("aa_required_level_cost");
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

	static AaRequiredLevelCost NewEntity()
	{
		AaRequiredLevelCost e{};

		e.skill_id    = 0;
		e.level       = 0;
		e.cost        = 0;
		e.description = "";
		e.eqmacid     = 0;

		return e;
	}

	static AaRequiredLevelCost GetAaRequiredLevelCost(
		const std::vector<AaRequiredLevelCost> &aa_required_level_costs,
		int aa_required_level_cost_id
	)
	{
		for (auto &aa_required_level_cost : aa_required_level_costs) {
			if (aa_required_level_cost.skill_id == aa_required_level_cost_id) {
				return aa_required_level_cost;
			}
		}

		return NewEntity();
	}

	static AaRequiredLevelCost FindOne(
		Database& db,
		int aa_required_level_cost_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				aa_required_level_cost_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			AaRequiredLevelCost e{};

			e.skill_id    = static_cast<uint32_t>(strtoul(row[0], nullptr, 10));
			e.level       = static_cast<uint32_t>(strtoul(row[1], nullptr, 10));
			e.cost        = static_cast<uint32_t>(strtoul(row[2], nullptr, 10));
			e.description = row[3] ? row[3] : "";
			e.eqmacid     = static_cast<int32_t>(atoi(row[4]));

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int aa_required_level_cost_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				aa_required_level_cost_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const AaRequiredLevelCost &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[0] + " = " + std::to_string(e.skill_id));
		v.push_back(columns[1] + " = " + std::to_string(e.level));
		v.push_back(columns[2] + " = " + std::to_string(e.cost));
		v.push_back(columns[3] + " = '" + Strings::Escape(e.description) + "'");
		v.push_back(columns[4] + " = " + std::to_string(e.eqmacid));

		auto results = db.QueryDatabase(
			fmt::format(
				"UPDATE {} SET {} WHERE {} = {}",
				TableName(),
				Strings::Implode(", ", v),
				PrimaryKey(),
				e.skill_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static AaRequiredLevelCost InsertOne(
		Database& db,
		AaRequiredLevelCost e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.skill_id));
		v.push_back(std::to_string(e.level));
		v.push_back(std::to_string(e.cost));
		v.push_back("'" + Strings::Escape(e.description) + "'");
		v.push_back(std::to_string(e.eqmacid));

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES ({})",
				BaseInsert(),
				Strings::Implode(",", v)
			)
		);

		if (results.Success()) {
			e.skill_id = results.LastInsertedID();
			return e;
		}

		e = NewEntity();

		return e;
	}

	static int InsertMany(
		Database& db,
		const std::vector<AaRequiredLevelCost> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.skill_id));
			v.push_back(std::to_string(e.level));
			v.push_back(std::to_string(e.cost));
			v.push_back("'" + Strings::Escape(e.description) + "'");
			v.push_back(std::to_string(e.eqmacid));

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

	static std::vector<AaRequiredLevelCost> All(Database& db)
	{
		std::vector<AaRequiredLevelCost> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			AaRequiredLevelCost e{};

			e.skill_id    = static_cast<uint32_t>(strtoul(row[0], nullptr, 10));
			e.level       = static_cast<uint32_t>(strtoul(row[1], nullptr, 10));
			e.cost        = static_cast<uint32_t>(strtoul(row[2], nullptr, 10));
			e.description = row[3] ? row[3] : "";
			e.eqmacid     = static_cast<int32_t>(atoi(row[4]));

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<AaRequiredLevelCost> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<AaRequiredLevelCost> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			AaRequiredLevelCost e{};

			e.skill_id    = static_cast<uint32_t>(strtoul(row[0], nullptr, 10));
			e.level       = static_cast<uint32_t>(strtoul(row[1], nullptr, 10));
			e.cost        = static_cast<uint32_t>(strtoul(row[2], nullptr, 10));
			e.description = row[3] ? row[3] : "";
			e.eqmacid     = static_cast<int32_t>(atoi(row[4]));

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

#endif //EQEMU_BASE_AA_REQUIRED_LEVEL_COST_REPOSITORY_H
