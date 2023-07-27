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

#ifndef EQEMU_BASE_ALTADV_VARS_REPOSITORY_H
#define EQEMU_BASE_ALTADV_VARS_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>


class BaseAltadvVarsRepository {
public:
	struct AltadvVars {
		int32_t     skill_id;
		std::string name;
		int32_t     cost;
		int32_t     max_level;
		uint8_t     type;
		uint32_t    spellid;
		uint32_t    prereq_skill;
		uint32_t    prereq_minpoints;
		uint32_t    spell_type;
		uint32_t    spell_refresh;
		uint32_t    classes;
		uint32_t    class_type;
		int8_t      cost_inc;
		uint16_t    aa_expansion;
		uint32_t    special_category;
		uint32_t    account_time_required;
		uint8_t     level_inc;
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
			"name",
			"cost",
			"max_level",
			"type",
			"spellid",
			"prereq_skill",
			"prereq_minpoints",
			"spell_type",
			"spell_refresh",
			"classes",
			"class_type",
			"cost_inc",
			"aa_expansion",
			"special_category",
			"account_time_required",
			"level_inc",
			"eqmacid",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"skill_id",
			"name",
			"cost",
			"max_level",
			"type",
			"spellid",
			"prereq_skill",
			"prereq_minpoints",
			"spell_type",
			"spell_refresh",
			"classes",
			"class_type",
			"cost_inc",
			"aa_expansion",
			"special_category",
			"account_time_required",
			"level_inc",
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
		return std::string("altadv_vars");
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

	static AltadvVars NewEntity()
	{
		AltadvVars e{};

		e.skill_id              = 0;
		e.name                  = "";
		e.cost                  = 0;
		e.max_level             = 0;
		e.type                  = 1;
		e.spellid               = 0;
		e.prereq_skill          = 0;
		e.prereq_minpoints      = 0;
		e.spell_type            = 0;
		e.spell_refresh         = 0;
		e.classes               = 65534;
		e.class_type            = 0;
		e.cost_inc              = 0;
		e.aa_expansion          = 3;
		e.special_category      = 4294967295;
		e.account_time_required = 0;
		e.level_inc             = 0;
		e.eqmacid               = 0;

		return e;
	}

	static AltadvVars GetAltadvVars(
		const std::vector<AltadvVars> &altadv_varss,
		int altadv_vars_id
	)
	{
		for (auto &altadv_vars : altadv_varss) {
			if (altadv_vars.skill_id == altadv_vars_id) {
				return altadv_vars;
			}
		}

		return NewEntity();
	}

	static AltadvVars FindOne(
		Database& db,
		int altadv_vars_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				altadv_vars_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			AltadvVars e{};

			e.skill_id              = static_cast<int32_t>(atoi(row[0]));
			e.name                  = row[1] ? row[1] : "";
			e.cost                  = static_cast<int32_t>(atoi(row[2]));
			e.max_level             = static_cast<int32_t>(atoi(row[3]));
			e.type                  = static_cast<uint8_t>(strtoul(row[4], nullptr, 10));
			e.spellid               = static_cast<uint32_t>(strtoul(row[5], nullptr, 10));
			e.prereq_skill          = static_cast<uint32_t>(strtoul(row[6], nullptr, 10));
			e.prereq_minpoints      = static_cast<uint32_t>(strtoul(row[7], nullptr, 10));
			e.spell_type            = static_cast<uint32_t>(strtoul(row[8], nullptr, 10));
			e.spell_refresh         = static_cast<uint32_t>(strtoul(row[9], nullptr, 10));
			e.classes               = static_cast<uint32_t>(strtoul(row[10], nullptr, 10));
			e.class_type            = static_cast<uint32_t>(strtoul(row[11], nullptr, 10));
			e.cost_inc              = static_cast<int8_t>(atoi(row[12]));
			e.aa_expansion          = static_cast<uint16_t>(strtoul(row[13], nullptr, 10));
			e.special_category      = static_cast<uint32_t>(strtoul(row[14], nullptr, 10));
			e.account_time_required = static_cast<uint32_t>(strtoul(row[15], nullptr, 10));
			e.level_inc             = static_cast<uint8_t>(strtoul(row[16], nullptr, 10));
			e.eqmacid               = static_cast<int32_t>(atoi(row[17]));

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int altadv_vars_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				altadv_vars_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const AltadvVars &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[0] + " = " + std::to_string(e.skill_id));
		v.push_back(columns[1] + " = '" + Strings::Escape(e.name) + "'");
		v.push_back(columns[2] + " = " + std::to_string(e.cost));
		v.push_back(columns[3] + " = " + std::to_string(e.max_level));
		v.push_back(columns[4] + " = " + std::to_string(e.type));
		v.push_back(columns[5] + " = " + std::to_string(e.spellid));
		v.push_back(columns[6] + " = " + std::to_string(e.prereq_skill));
		v.push_back(columns[7] + " = " + std::to_string(e.prereq_minpoints));
		v.push_back(columns[8] + " = " + std::to_string(e.spell_type));
		v.push_back(columns[9] + " = " + std::to_string(e.spell_refresh));
		v.push_back(columns[10] + " = " + std::to_string(e.classes));
		v.push_back(columns[11] + " = " + std::to_string(e.class_type));
		v.push_back(columns[12] + " = " + std::to_string(e.cost_inc));
		v.push_back(columns[13] + " = " + std::to_string(e.aa_expansion));
		v.push_back(columns[14] + " = " + std::to_string(e.special_category));
		v.push_back(columns[15] + " = " + std::to_string(e.account_time_required));
		v.push_back(columns[16] + " = " + std::to_string(e.level_inc));
		v.push_back(columns[17] + " = " + std::to_string(e.eqmacid));

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

	static AltadvVars InsertOne(
		Database& db,
		AltadvVars e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.skill_id));
		v.push_back("'" + Strings::Escape(e.name) + "'");
		v.push_back(std::to_string(e.cost));
		v.push_back(std::to_string(e.max_level));
		v.push_back(std::to_string(e.type));
		v.push_back(std::to_string(e.spellid));
		v.push_back(std::to_string(e.prereq_skill));
		v.push_back(std::to_string(e.prereq_minpoints));
		v.push_back(std::to_string(e.spell_type));
		v.push_back(std::to_string(e.spell_refresh));
		v.push_back(std::to_string(e.classes));
		v.push_back(std::to_string(e.class_type));
		v.push_back(std::to_string(e.cost_inc));
		v.push_back(std::to_string(e.aa_expansion));
		v.push_back(std::to_string(e.special_category));
		v.push_back(std::to_string(e.account_time_required));
		v.push_back(std::to_string(e.level_inc));
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
		const std::vector<AltadvVars> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.skill_id));
			v.push_back("'" + Strings::Escape(e.name) + "'");
			v.push_back(std::to_string(e.cost));
			v.push_back(std::to_string(e.max_level));
			v.push_back(std::to_string(e.type));
			v.push_back(std::to_string(e.spellid));
			v.push_back(std::to_string(e.prereq_skill));
			v.push_back(std::to_string(e.prereq_minpoints));
			v.push_back(std::to_string(e.spell_type));
			v.push_back(std::to_string(e.spell_refresh));
			v.push_back(std::to_string(e.classes));
			v.push_back(std::to_string(e.class_type));
			v.push_back(std::to_string(e.cost_inc));
			v.push_back(std::to_string(e.aa_expansion));
			v.push_back(std::to_string(e.special_category));
			v.push_back(std::to_string(e.account_time_required));
			v.push_back(std::to_string(e.level_inc));
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

	static std::vector<AltadvVars> All(Database& db)
	{
		std::vector<AltadvVars> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			AltadvVars e{};

			e.skill_id              = static_cast<int32_t>(atoi(row[0]));
			e.name                  = row[1] ? row[1] : "";
			e.cost                  = static_cast<int32_t>(atoi(row[2]));
			e.max_level             = static_cast<int32_t>(atoi(row[3]));
			e.type                  = static_cast<uint8_t>(strtoul(row[4], nullptr, 10));
			e.spellid               = static_cast<uint32_t>(strtoul(row[5], nullptr, 10));
			e.prereq_skill          = static_cast<uint32_t>(strtoul(row[6], nullptr, 10));
			e.prereq_minpoints      = static_cast<uint32_t>(strtoul(row[7], nullptr, 10));
			e.spell_type            = static_cast<uint32_t>(strtoul(row[8], nullptr, 10));
			e.spell_refresh         = static_cast<uint32_t>(strtoul(row[9], nullptr, 10));
			e.classes               = static_cast<uint32_t>(strtoul(row[10], nullptr, 10));
			e.class_type            = static_cast<uint32_t>(strtoul(row[11], nullptr, 10));
			e.cost_inc              = static_cast<int8_t>(atoi(row[12]));
			e.aa_expansion          = static_cast<uint16_t>(strtoul(row[13], nullptr, 10));
			e.special_category      = static_cast<uint32_t>(strtoul(row[14], nullptr, 10));
			e.account_time_required = static_cast<uint32_t>(strtoul(row[15], nullptr, 10));
			e.level_inc             = static_cast<uint8_t>(strtoul(row[16], nullptr, 10));
			e.eqmacid               = static_cast<int32_t>(atoi(row[17]));

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<AltadvVars> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<AltadvVars> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			AltadvVars e{};

			e.skill_id              = static_cast<int32_t>(atoi(row[0]));
			e.name                  = row[1] ? row[1] : "";
			e.cost                  = static_cast<int32_t>(atoi(row[2]));
			e.max_level             = static_cast<int32_t>(atoi(row[3]));
			e.type                  = static_cast<uint8_t>(strtoul(row[4], nullptr, 10));
			e.spellid               = static_cast<uint32_t>(strtoul(row[5], nullptr, 10));
			e.prereq_skill          = static_cast<uint32_t>(strtoul(row[6], nullptr, 10));
			e.prereq_minpoints      = static_cast<uint32_t>(strtoul(row[7], nullptr, 10));
			e.spell_type            = static_cast<uint32_t>(strtoul(row[8], nullptr, 10));
			e.spell_refresh         = static_cast<uint32_t>(strtoul(row[9], nullptr, 10));
			e.classes               = static_cast<uint32_t>(strtoul(row[10], nullptr, 10));
			e.class_type            = static_cast<uint32_t>(strtoul(row[11], nullptr, 10));
			e.cost_inc              = static_cast<int8_t>(atoi(row[12]));
			e.aa_expansion          = static_cast<uint16_t>(strtoul(row[13], nullptr, 10));
			e.special_category      = static_cast<uint32_t>(strtoul(row[14], nullptr, 10));
			e.account_time_required = static_cast<uint32_t>(strtoul(row[15], nullptr, 10));
			e.level_inc             = static_cast<uint8_t>(strtoul(row[16], nullptr, 10));
			e.eqmacid               = static_cast<int32_t>(atoi(row[17]));

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

#endif //EQEMU_BASE_ALTADV_VARS_REPOSITORY_H
