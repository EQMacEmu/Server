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

#ifndef EQEMU_BASE_AA_ACTIONS_REPOSITORY_H
#define EQEMU_BASE_AA_ACTIONS_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>


class BaseAaActionsRepository {
public:
	struct AaActions {
		uint32_t aaid;
		uint8_t  rank;
		uint32_t reuse_time;
		uint32_t spell_id;
		uint8_t  target;
		uint8_t  nonspell_action;
		uint32_t nonspell_mana;
		uint32_t nonspell_duration;
		uint32_t redux_aa;
		int8_t   redux_rate;
		uint32_t redux_aa2;
		int8_t   redux_rate2;
	};

	static std::string PrimaryKey()
	{
		return std::string("aaid");
	}

	static std::vector<std::string> Columns()
	{
		return {
			"aaid",
			"rank",
			"reuse_time",
			"spell_id",
			"target",
			"nonspell_action",
			"nonspell_mana",
			"nonspell_duration",
			"redux_aa",
			"redux_rate",
			"redux_aa2",
			"redux_rate2",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"aaid",
			"rank",
			"reuse_time",
			"spell_id",
			"target",
			"nonspell_action",
			"nonspell_mana",
			"nonspell_duration",
			"redux_aa",
			"redux_rate",
			"redux_aa2",
			"redux_rate2",
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
		return std::string("aa_actions");
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

	static AaActions NewEntity()
	{
		AaActions e{};

		e.aaid              = 0;
		e.rank              = 0;
		e.reuse_time        = 0;
		e.spell_id          = 0;
		e.target            = 0;
		e.nonspell_action   = 0;
		e.nonspell_mana     = 0;
		e.nonspell_duration = 0;
		e.redux_aa          = 0;
		e.redux_rate        = 0;
		e.redux_aa2         = 0;
		e.redux_rate2       = 0;

		return e;
	}

	static AaActions GetAaActions(
		const std::vector<AaActions> &aa_actionss,
		int aa_actions_id
	)
	{
		for (auto &aa_actions : aa_actionss) {
			if (aa_actions.aaid == aa_actions_id) {
				return aa_actions;
			}
		}

		return NewEntity();
	}

	static AaActions FindOne(
		Database& db,
		int aa_actions_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				aa_actions_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			AaActions e{};

			e.aaid              = static_cast<uint32_t>(strtoul(row[0], nullptr, 10));
			e.rank              = static_cast<uint8_t>(strtoul(row[1], nullptr, 10));
			e.reuse_time        = static_cast<uint32_t>(strtoul(row[2], nullptr, 10));
			e.spell_id          = static_cast<uint32_t>(strtoul(row[3], nullptr, 10));
			e.target            = static_cast<uint8_t>(strtoul(row[4], nullptr, 10));
			e.nonspell_action   = static_cast<uint8_t>(strtoul(row[5], nullptr, 10));
			e.nonspell_mana     = static_cast<uint32_t>(strtoul(row[6], nullptr, 10));
			e.nonspell_duration = static_cast<uint32_t>(strtoul(row[7], nullptr, 10));
			e.redux_aa          = static_cast<uint32_t>(strtoul(row[8], nullptr, 10));
			e.redux_rate        = static_cast<int8_t>(atoi(row[9]));
			e.redux_aa2         = static_cast<uint32_t>(strtoul(row[10], nullptr, 10));
			e.redux_rate2       = static_cast<int8_t>(atoi(row[11]));

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int aa_actions_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				aa_actions_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const AaActions &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[0] + " = " + std::to_string(e.aaid));
		v.push_back(columns[1] + " = " + std::to_string(e.rank));
		v.push_back(columns[2] + " = " + std::to_string(e.reuse_time));
		v.push_back(columns[3] + " = " + std::to_string(e.spell_id));
		v.push_back(columns[4] + " = " + std::to_string(e.target));
		v.push_back(columns[5] + " = " + std::to_string(e.nonspell_action));
		v.push_back(columns[6] + " = " + std::to_string(e.nonspell_mana));
		v.push_back(columns[7] + " = " + std::to_string(e.nonspell_duration));
		v.push_back(columns[8] + " = " + std::to_string(e.redux_aa));
		v.push_back(columns[9] + " = " + std::to_string(e.redux_rate));
		v.push_back(columns[10] + " = " + std::to_string(e.redux_aa2));
		v.push_back(columns[11] + " = " + std::to_string(e.redux_rate2));

		auto results = db.QueryDatabase(
			fmt::format(
				"UPDATE {} SET {} WHERE {} = {}",
				TableName(),
				Strings::Implode(", ", v),
				PrimaryKey(),
				e.aaid
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static AaActions InsertOne(
		Database& db,
		AaActions e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.aaid));
		v.push_back(std::to_string(e.rank));
		v.push_back(std::to_string(e.reuse_time));
		v.push_back(std::to_string(e.spell_id));
		v.push_back(std::to_string(e.target));
		v.push_back(std::to_string(e.nonspell_action));
		v.push_back(std::to_string(e.nonspell_mana));
		v.push_back(std::to_string(e.nonspell_duration));
		v.push_back(std::to_string(e.redux_aa));
		v.push_back(std::to_string(e.redux_rate));
		v.push_back(std::to_string(e.redux_aa2));
		v.push_back(std::to_string(e.redux_rate2));

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES ({})",
				BaseInsert(),
				Strings::Implode(",", v)
			)
		);

		if (results.Success()) {
			e.aaid = results.LastInsertedID();
			return e;
		}

		e = NewEntity();

		return e;
	}

	static int InsertMany(
		Database& db,
		const std::vector<AaActions> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.aaid));
			v.push_back(std::to_string(e.rank));
			v.push_back(std::to_string(e.reuse_time));
			v.push_back(std::to_string(e.spell_id));
			v.push_back(std::to_string(e.target));
			v.push_back(std::to_string(e.nonspell_action));
			v.push_back(std::to_string(e.nonspell_mana));
			v.push_back(std::to_string(e.nonspell_duration));
			v.push_back(std::to_string(e.redux_aa));
			v.push_back(std::to_string(e.redux_rate));
			v.push_back(std::to_string(e.redux_aa2));
			v.push_back(std::to_string(e.redux_rate2));

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

	static std::vector<AaActions> All(Database& db)
	{
		std::vector<AaActions> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			AaActions e{};

			e.aaid              = static_cast<uint32_t>(strtoul(row[0], nullptr, 10));
			e.rank              = static_cast<uint8_t>(strtoul(row[1], nullptr, 10));
			e.reuse_time        = static_cast<uint32_t>(strtoul(row[2], nullptr, 10));
			e.spell_id          = static_cast<uint32_t>(strtoul(row[3], nullptr, 10));
			e.target            = static_cast<uint8_t>(strtoul(row[4], nullptr, 10));
			e.nonspell_action   = static_cast<uint8_t>(strtoul(row[5], nullptr, 10));
			e.nonspell_mana     = static_cast<uint32_t>(strtoul(row[6], nullptr, 10));
			e.nonspell_duration = static_cast<uint32_t>(strtoul(row[7], nullptr, 10));
			e.redux_aa          = static_cast<uint32_t>(strtoul(row[8], nullptr, 10));
			e.redux_rate        = static_cast<int8_t>(atoi(row[9]));
			e.redux_aa2         = static_cast<uint32_t>(strtoul(row[10], nullptr, 10));
			e.redux_rate2       = static_cast<int8_t>(atoi(row[11]));

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<AaActions> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<AaActions> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			AaActions e{};

			e.aaid              = static_cast<uint32_t>(strtoul(row[0], nullptr, 10));
			e.rank              = static_cast<uint8_t>(strtoul(row[1], nullptr, 10));
			e.reuse_time        = static_cast<uint32_t>(strtoul(row[2], nullptr, 10));
			e.spell_id          = static_cast<uint32_t>(strtoul(row[3], nullptr, 10));
			e.target            = static_cast<uint8_t>(strtoul(row[4], nullptr, 10));
			e.nonspell_action   = static_cast<uint8_t>(strtoul(row[5], nullptr, 10));
			e.nonspell_mana     = static_cast<uint32_t>(strtoul(row[6], nullptr, 10));
			e.nonspell_duration = static_cast<uint32_t>(strtoul(row[7], nullptr, 10));
			e.redux_aa          = static_cast<uint32_t>(strtoul(row[8], nullptr, 10));
			e.redux_rate        = static_cast<int8_t>(atoi(row[9]));
			e.redux_aa2         = static_cast<uint32_t>(strtoul(row[10], nullptr, 10));
			e.redux_rate2       = static_cast<int8_t>(atoi(row[11]));

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

#endif //EQEMU_BASE_AA_ACTIONS_REPOSITORY_H
