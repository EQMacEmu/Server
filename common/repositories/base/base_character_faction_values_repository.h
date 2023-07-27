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

#ifndef EQEMU_BASE_CHARACTER_FACTION_VALUES_REPOSITORY_H
#define EQEMU_BASE_CHARACTER_FACTION_VALUES_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>


class BaseCharacterFactionValuesRepository {
public:
	struct CharacterFactionValues {
		int32_t id;
		int32_t faction_id;
		int16_t current_value;
		int8_t  temp;
	};

	static std::string PrimaryKey()
	{
		return std::string("id");
	}

	static std::vector<std::string> Columns()
	{
		return {
			"id",
			"faction_id",
			"current_value",
			"temp",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"id",
			"faction_id",
			"current_value",
			"temp",
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
		return std::string("character_faction_values");
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

	static CharacterFactionValues NewEntity()
	{
		CharacterFactionValues e{};

		e.id            = 0;
		e.faction_id    = 0;
		e.current_value = 0;
		e.temp          = 0;

		return e;
	}

	static CharacterFactionValues GetCharacterFactionValues(
		const std::vector<CharacterFactionValues> &character_faction_valuess,
		int character_faction_values_id
	)
	{
		for (auto &character_faction_values : character_faction_valuess) {
			if (character_faction_values.id == character_faction_values_id) {
				return character_faction_values;
			}
		}

		return NewEntity();
	}

	static CharacterFactionValues FindOne(
		Database& db,
		int character_faction_values_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				character_faction_values_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			CharacterFactionValues e{};

			e.id            = static_cast<int32_t>(atoi(row[0]));
			e.faction_id    = static_cast<int32_t>(atoi(row[1]));
			e.current_value = static_cast<int16_t>(atoi(row[2]));
			e.temp          = static_cast<int8_t>(atoi(row[3]));

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int character_faction_values_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				character_faction_values_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const CharacterFactionValues &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[0] + " = " + std::to_string(e.id));
		v.push_back(columns[1] + " = " + std::to_string(e.faction_id));
		v.push_back(columns[2] + " = " + std::to_string(e.current_value));
		v.push_back(columns[3] + " = " + std::to_string(e.temp));

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

	static CharacterFactionValues InsertOne(
		Database& db,
		CharacterFactionValues e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.id));
		v.push_back(std::to_string(e.faction_id));
		v.push_back(std::to_string(e.current_value));
		v.push_back(std::to_string(e.temp));

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
		const std::vector<CharacterFactionValues> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.id));
			v.push_back(std::to_string(e.faction_id));
			v.push_back(std::to_string(e.current_value));
			v.push_back(std::to_string(e.temp));

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

	static std::vector<CharacterFactionValues> All(Database& db)
	{
		std::vector<CharacterFactionValues> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			CharacterFactionValues e{};

			e.id            = static_cast<int32_t>(atoi(row[0]));
			e.faction_id    = static_cast<int32_t>(atoi(row[1]));
			e.current_value = static_cast<int16_t>(atoi(row[2]));
			e.temp          = static_cast<int8_t>(atoi(row[3]));

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<CharacterFactionValues> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<CharacterFactionValues> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			CharacterFactionValues e{};

			e.id            = static_cast<int32_t>(atoi(row[0]));
			e.faction_id    = static_cast<int32_t>(atoi(row[1]));
			e.current_value = static_cast<int16_t>(atoi(row[2]));
			e.temp          = static_cast<int8_t>(atoi(row[3]));

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

#endif //EQEMU_BASE_CHARACTER_FACTION_VALUES_REPOSITORY_H
