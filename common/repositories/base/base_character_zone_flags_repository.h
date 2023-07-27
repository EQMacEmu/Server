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

#ifndef EQEMU_BASE_CHARACTER_ZONE_FLAGS_REPOSITORY_H
#define EQEMU_BASE_CHARACTER_ZONE_FLAGS_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>


class BaseCharacterZoneFlagsRepository {
public:
	struct CharacterZoneFlags {
		int32_t id;
		int32_t zoneID;
		int8_t  key_;
	};

	static std::string PrimaryKey()
	{
		return std::string("id");
	}

	static std::vector<std::string> Columns()
	{
		return {
			"id",
			"zoneID",
			"key_",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"id",
			"zoneID",
			"key_",
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
		return std::string("character_zone_flags");
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

	static CharacterZoneFlags NewEntity()
	{
		CharacterZoneFlags e{};

		e.id     = 0;
		e.zoneID = 0;
		e.key_   = 0;

		return e;
	}

	static CharacterZoneFlags GetCharacterZoneFlags(
		const std::vector<CharacterZoneFlags> &character_zone_flagss,
		int character_zone_flags_id
	)
	{
		for (auto &character_zone_flags : character_zone_flagss) {
			if (character_zone_flags.id == character_zone_flags_id) {
				return character_zone_flags;
			}
		}

		return NewEntity();
	}

	static CharacterZoneFlags FindOne(
		Database& db,
		int character_zone_flags_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				character_zone_flags_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			CharacterZoneFlags e{};

			e.id     = static_cast<int32_t>(atoi(row[0]));
			e.zoneID = static_cast<int32_t>(atoi(row[1]));
			e.key_   = static_cast<int8_t>(atoi(row[2]));

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int character_zone_flags_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				character_zone_flags_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const CharacterZoneFlags &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[0] + " = " + std::to_string(e.id));
		v.push_back(columns[1] + " = " + std::to_string(e.zoneID));
		v.push_back(columns[2] + " = " + std::to_string(e.key_));

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

	static CharacterZoneFlags InsertOne(
		Database& db,
		CharacterZoneFlags e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.id));
		v.push_back(std::to_string(e.zoneID));
		v.push_back(std::to_string(e.key_));

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
		const std::vector<CharacterZoneFlags> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.id));
			v.push_back(std::to_string(e.zoneID));
			v.push_back(std::to_string(e.key_));

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

	static std::vector<CharacterZoneFlags> All(Database& db)
	{
		std::vector<CharacterZoneFlags> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			CharacterZoneFlags e{};

			e.id     = static_cast<int32_t>(atoi(row[0]));
			e.zoneID = static_cast<int32_t>(atoi(row[1]));
			e.key_   = static_cast<int8_t>(atoi(row[2]));

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<CharacterZoneFlags> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<CharacterZoneFlags> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			CharacterZoneFlags e{};

			e.id     = static_cast<int32_t>(atoi(row[0]));
			e.zoneID = static_cast<int32_t>(atoi(row[1]));
			e.key_   = static_cast<int8_t>(atoi(row[2]));

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

#endif //EQEMU_BASE_CHARACTER_ZONE_FLAGS_REPOSITORY_H
