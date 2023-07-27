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

#ifndef EQEMU_BASE_AA_EFFECTS_REPOSITORY_H
#define EQEMU_BASE_AA_EFFECTS_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>


class BaseAaEffectsRepository {
public:
	struct AaEffects {
		uint32_t id;
		uint32_t aaid;
		uint8_t  slot;
		uint32_t effectid;
		int32_t  base1;
		int32_t  base2;
	};

	static std::string PrimaryKey()
	{
		return std::string("id");
	}

	static std::vector<std::string> Columns()
	{
		return {
			"id",
			"aaid",
			"slot",
			"effectid",
			"base1",
			"base2",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"id",
			"aaid",
			"slot",
			"effectid",
			"base1",
			"base2",
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
		return std::string("aa_effects");
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

	static AaEffects NewEntity()
	{
		AaEffects e{};

		e.id       = 0;
		e.aaid     = 0;
		e.slot     = 0;
		e.effectid = 0;
		e.base1    = 0;
		e.base2    = 0;

		return e;
	}

	static AaEffects GetAaEffects(
		const std::vector<AaEffects> &aa_effectss,
		int aa_effects_id
	)
	{
		for (auto &aa_effects : aa_effectss) {
			if (aa_effects.id == aa_effects_id) {
				return aa_effects;
			}
		}

		return NewEntity();
	}

	static AaEffects FindOne(
		Database& db,
		int aa_effects_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				aa_effects_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			AaEffects e{};

			e.id       = static_cast<uint32_t>(strtoul(row[0], nullptr, 10));
			e.aaid     = static_cast<uint32_t>(strtoul(row[1], nullptr, 10));
			e.slot     = static_cast<uint8_t>(strtoul(row[2], nullptr, 10));
			e.effectid = static_cast<uint32_t>(strtoul(row[3], nullptr, 10));
			e.base1    = static_cast<int32_t>(atoi(row[4]));
			e.base2    = static_cast<int32_t>(atoi(row[5]));

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int aa_effects_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				aa_effects_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const AaEffects &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[1] + " = " + std::to_string(e.aaid));
		v.push_back(columns[2] + " = " + std::to_string(e.slot));
		v.push_back(columns[3] + " = " + std::to_string(e.effectid));
		v.push_back(columns[4] + " = " + std::to_string(e.base1));
		v.push_back(columns[5] + " = " + std::to_string(e.base2));

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

	static AaEffects InsertOne(
		Database& db,
		AaEffects e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.id));
		v.push_back(std::to_string(e.aaid));
		v.push_back(std::to_string(e.slot));
		v.push_back(std::to_string(e.effectid));
		v.push_back(std::to_string(e.base1));
		v.push_back(std::to_string(e.base2));

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
		const std::vector<AaEffects> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.id));
			v.push_back(std::to_string(e.aaid));
			v.push_back(std::to_string(e.slot));
			v.push_back(std::to_string(e.effectid));
			v.push_back(std::to_string(e.base1));
			v.push_back(std::to_string(e.base2));

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

	static std::vector<AaEffects> All(Database& db)
	{
		std::vector<AaEffects> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			AaEffects e{};

			e.id       = static_cast<uint32_t>(strtoul(row[0], nullptr, 10));
			e.aaid     = static_cast<uint32_t>(strtoul(row[1], nullptr, 10));
			e.slot     = static_cast<uint8_t>(strtoul(row[2], nullptr, 10));
			e.effectid = static_cast<uint32_t>(strtoul(row[3], nullptr, 10));
			e.base1    = static_cast<int32_t>(atoi(row[4]));
			e.base2    = static_cast<int32_t>(atoi(row[5]));

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<AaEffects> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<AaEffects> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			AaEffects e{};

			e.id       = static_cast<uint32_t>(strtoul(row[0], nullptr, 10));
			e.aaid     = static_cast<uint32_t>(strtoul(row[1], nullptr, 10));
			e.slot     = static_cast<uint8_t>(strtoul(row[2], nullptr, 10));
			e.effectid = static_cast<uint32_t>(strtoul(row[3], nullptr, 10));
			e.base1    = static_cast<int32_t>(atoi(row[4]));
			e.base2    = static_cast<int32_t>(atoi(row[5]));

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

#endif //EQEMU_BASE_AA_EFFECTS_REPOSITORY_H
