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

#ifndef EQEMU_BASE_ACCOUNT_REPOSITORY_H
#define EQEMU_BASE_ACCOUNT_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>

class BaseAccountRepository {
public:
	struct Account {
		int32_t     id;
		std::string name;
		std::string charname;
		uint32_t    sharedplat;
		std::string password;
		int32_t     status;
		uint32_t    lsaccount_id;
		uint8_t     gmspeed;
		uint8_t     revoked;
		uint32_t    karma;
		std::string minilogin_ip;
		int8_t      hideme;
		uint8_t     rulesflag;
		time_t      suspendeduntil;
		uint32_t    time_creation;
		int8_t      expansion;
		std::string ban_reason;
		std::string suspend_reason;
		int8_t      active;
		int32_t     ip_exemption_multiplier;
		int8_t      gminvul;
		int8_t      flymode;
		int8_t      ignore_tells;
		int8_t      mule;
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
			"charname",
			"sharedplat",
			"password",
			"status",
			"lsaccount_id",
			"gmspeed",
			"revoked",
			"karma",
			"minilogin_ip",
			"hideme",
			"rulesflag",
			"suspendeduntil",
			"time_creation",
			"expansion",
			"ban_reason",
			"suspend_reason",
			"active",
			"ip_exemption_multiplier",
			"gminvul",
			"flymode",
			"ignore_tells",
			"mule",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"id",
			"name",
			"charname",
			"sharedplat",
			"password",
			"status",
			"lsaccount_id",
			"gmspeed",
			"revoked",
			"karma",
			"minilogin_ip",
			"hideme",
			"rulesflag",
			"UNIX_TIMESTAMP(suspendeduntil)",
			"time_creation",
			"expansion",
			"ban_reason",
			"suspend_reason",
			"active",
			"ip_exemption_multiplier",
			"gminvul",
			"flymode",
			"ignore_tells",
			"mule",
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
		return std::string("account");
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

	static Account NewEntity()
	{
		Account e{};

		e.id                      = 0;
		e.name                    = "";
		e.charname                = "";
		e.sharedplat              = 0;
		e.password                = "";
		e.status                  = 0;
		e.lsaccount_id            = 0;
		e.gmspeed                 = 0;
		e.revoked                 = 0;
		e.karma                   = 0;
		e.minilogin_ip            = "";
		e.hideme                  = 0;
		e.rulesflag               = 0;
		e.suspendeduntil          = 0;
		e.time_creation           = 0;
		e.expansion               = 12;
		e.ban_reason              = "";
		e.suspend_reason          = "";
		e.active                  = 0;
		e.ip_exemption_multiplier = 1;
		e.gminvul                 = 0;
		e.flymode                 = 0;
		e.ignore_tells            = 0;
		e.mule                    = 0;

		return e;
	}

	static Account GetAccount(
		const std::vector<Account> &accounts,
		int account_id
	)
	{
		for (auto &account : accounts) {
			if (account.id == account_id) {
				return account;
			}
		}

		return NewEntity();
	}

	static Account FindOne(
		Database& db,
		int account_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				account_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			Account e{};

			e.id                      = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.name                    = row[1] ? row[1] : "";
			e.charname                = row[2] ? row[2] : "";
			e.sharedplat              = row[3] ? static_cast<uint32_t>(strtoul(row[3], nullptr, 10)) : 0;
			e.password                = row[4] ? row[4] : "";
			e.status                  = row[5] ? static_cast<int32_t>(atoi(row[5])) : 0;
			e.lsaccount_id            = row[6] ? static_cast<uint32_t>(strtoul(row[6], nullptr, 10)) : 0;
			e.gmspeed                 = row[7] ? static_cast<uint8_t>(strtoul(row[7], nullptr, 10)) : 0;
			e.revoked                 = row[8] ? static_cast<uint8_t>(strtoul(row[8], nullptr, 10)) : 0;
			e.karma                   = row[9] ? static_cast<uint32_t>(strtoul(row[9], nullptr, 10)) : 0;
			e.minilogin_ip            = row[10] ? row[10] : "";
			e.hideme                  = row[11] ? static_cast<int8_t>(atoi(row[11])) : 0;
			e.rulesflag               = row[12] ? static_cast<uint8_t>(strtoul(row[12], nullptr, 10)) : 0;
			e.suspendeduntil          = strtoll(row[13] ? row[13] : "-1", nullptr, 10);
			e.time_creation           = row[14] ? static_cast<uint32_t>(strtoul(row[14], nullptr, 10)) : 0;
			e.expansion               = row[15] ? static_cast<int8_t>(atoi(row[15])) : 12;
			e.ban_reason              = row[16] ? row[16] : "";
			e.suspend_reason          = row[17] ? row[17] : "";
			e.active                  = row[18] ? static_cast<int8_t>(atoi(row[18])) : 0;
			e.ip_exemption_multiplier = row[19] ? static_cast<int32_t>(atoi(row[19])) : 1;
			e.gminvul                 = row[20] ? static_cast<int8_t>(atoi(row[20])) : 0;
			e.flymode                 = row[21] ? static_cast<int8_t>(atoi(row[21])) : 0;
			e.ignore_tells            = row[22] ? static_cast<int8_t>(atoi(row[22])) : 0;
			e.mule                    = row[23] ? static_cast<int8_t>(atoi(row[23])) : 0;

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int account_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				account_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const Account &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[1] + " = '" + Strings::Escape(e.name) + "'");
		v.push_back(columns[2] + " = '" + Strings::Escape(e.charname) + "'");
		v.push_back(columns[3] + " = " + std::to_string(e.sharedplat));
		v.push_back(columns[4] + " = '" + Strings::Escape(e.password) + "'");
		v.push_back(columns[5] + " = " + std::to_string(e.status));
		v.push_back(columns[6] + " = " + std::to_string(e.lsaccount_id));
		v.push_back(columns[7] + " = " + std::to_string(e.gmspeed));
		v.push_back(columns[8] + " = " + std::to_string(e.revoked));
		v.push_back(columns[9] + " = " + std::to_string(e.karma));
		v.push_back(columns[10] + " = '" + Strings::Escape(e.minilogin_ip) + "'");
		v.push_back(columns[11] + " = " + std::to_string(e.hideme));
		v.push_back(columns[12] + " = " + std::to_string(e.rulesflag));
		v.push_back(columns[13] + " = FROM_UNIXTIME(" + (e.suspendeduntil > 0 ? std::to_string(e.suspendeduntil) : "null") + ")");
		v.push_back(columns[14] + " = " + std::to_string(e.time_creation));
		v.push_back(columns[15] + " = " + std::to_string(e.expansion));
		v.push_back(columns[16] + " = '" + Strings::Escape(e.ban_reason) + "'");
		v.push_back(columns[17] + " = '" + Strings::Escape(e.suspend_reason) + "'");
		v.push_back(columns[18] + " = " + std::to_string(e.active));
		v.push_back(columns[19] + " = " + std::to_string(e.ip_exemption_multiplier));
		v.push_back(columns[20] + " = " + std::to_string(e.gminvul));
		v.push_back(columns[21] + " = " + std::to_string(e.flymode));
		v.push_back(columns[22] + " = " + std::to_string(e.ignore_tells));
		v.push_back(columns[23] + " = " + std::to_string(e.mule));

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

	static Account InsertOne(
		Database& db,
		Account e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.id));
		v.push_back("'" + Strings::Escape(e.name) + "'");
		v.push_back("'" + Strings::Escape(e.charname) + "'");
		v.push_back(std::to_string(e.sharedplat));
		v.push_back("'" + Strings::Escape(e.password) + "'");
		v.push_back(std::to_string(e.status));
		v.push_back(std::to_string(e.lsaccount_id));
		v.push_back(std::to_string(e.gmspeed));
		v.push_back(std::to_string(e.revoked));
		v.push_back(std::to_string(e.karma));
		v.push_back("'" + Strings::Escape(e.minilogin_ip) + "'");
		v.push_back(std::to_string(e.hideme));
		v.push_back(std::to_string(e.rulesflag));
		v.push_back("FROM_UNIXTIME(" + (e.suspendeduntil > 0 ? std::to_string(e.suspendeduntil) : "null") + ")");
		v.push_back(std::to_string(e.time_creation));
		v.push_back(std::to_string(e.expansion));
		v.push_back("'" + Strings::Escape(e.ban_reason) + "'");
		v.push_back("'" + Strings::Escape(e.suspend_reason) + "'");
		v.push_back(std::to_string(e.active));
		v.push_back(std::to_string(e.ip_exemption_multiplier));
		v.push_back(std::to_string(e.gminvul));
		v.push_back(std::to_string(e.flymode));
		v.push_back(std::to_string(e.ignore_tells));
		v.push_back(std::to_string(e.mule));

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
		const std::vector<Account> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.id));
			v.push_back("'" + Strings::Escape(e.name) + "'");
			v.push_back("'" + Strings::Escape(e.charname) + "'");
			v.push_back(std::to_string(e.sharedplat));
			v.push_back("'" + Strings::Escape(e.password) + "'");
			v.push_back(std::to_string(e.status));
			v.push_back(std::to_string(e.lsaccount_id));
			v.push_back(std::to_string(e.gmspeed));
			v.push_back(std::to_string(e.revoked));
			v.push_back(std::to_string(e.karma));
			v.push_back("'" + Strings::Escape(e.minilogin_ip) + "'");
			v.push_back(std::to_string(e.hideme));
			v.push_back(std::to_string(e.rulesflag));
			v.push_back("FROM_UNIXTIME(" + (e.suspendeduntil > 0 ? std::to_string(e.suspendeduntil) : "null") + ")");
			v.push_back(std::to_string(e.time_creation));
			v.push_back(std::to_string(e.expansion));
			v.push_back("'" + Strings::Escape(e.ban_reason) + "'");
			v.push_back("'" + Strings::Escape(e.suspend_reason) + "'");
			v.push_back(std::to_string(e.active));
			v.push_back(std::to_string(e.ip_exemption_multiplier));
			v.push_back(std::to_string(e.gminvul));
			v.push_back(std::to_string(e.flymode));
			v.push_back(std::to_string(e.ignore_tells));
			v.push_back(std::to_string(e.mule));

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

	static std::vector<Account> All(Database& db)
	{
		std::vector<Account> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			Account e{};

			e.id                      = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.name                    = row[1] ? row[1] : "";
			e.charname                = row[2] ? row[2] : "";
			e.sharedplat              = row[3] ? static_cast<uint32_t>(strtoul(row[3], nullptr, 10)) : 0;
			e.password                = row[4] ? row[4] : "";
			e.status                  = row[5] ? static_cast<int32_t>(atoi(row[5])) : 0;
			e.lsaccount_id            = row[6] ? static_cast<uint32_t>(strtoul(row[6], nullptr, 10)) : 0;
			e.gmspeed                 = row[7] ? static_cast<uint8_t>(strtoul(row[7], nullptr, 10)) : 0;
			e.revoked                 = row[8] ? static_cast<uint8_t>(strtoul(row[8], nullptr, 10)) : 0;
			e.karma                   = row[9] ? static_cast<uint32_t>(strtoul(row[9], nullptr, 10)) : 0;
			e.minilogin_ip            = row[10] ? row[10] : "";
			e.hideme                  = row[11] ? static_cast<int8_t>(atoi(row[11])) : 0;
			e.rulesflag               = row[12] ? static_cast<uint8_t>(strtoul(row[12], nullptr, 10)) : 0;
			e.suspendeduntil          = strtoll(row[13] ? row[13] : "-1", nullptr, 10);
			e.time_creation           = row[14] ? static_cast<uint32_t>(strtoul(row[14], nullptr, 10)) : 0;
			e.expansion               = row[15] ? static_cast<int8_t>(atoi(row[15])) : 12;
			e.ban_reason              = row[16] ? row[16] : "";
			e.suspend_reason          = row[17] ? row[17] : "";
			e.active                  = row[18] ? static_cast<int8_t>(atoi(row[18])) : 0;
			e.ip_exemption_multiplier = row[19] ? static_cast<int32_t>(atoi(row[19])) : 1;
			e.gminvul                 = row[20] ? static_cast<int8_t>(atoi(row[20])) : 0;
			e.flymode                 = row[21] ? static_cast<int8_t>(atoi(row[21])) : 0;
			e.ignore_tells            = row[22] ? static_cast<int8_t>(atoi(row[22])) : 0;
			e.mule                    = row[23] ? static_cast<int8_t>(atoi(row[23])) : 0;

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<Account> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<Account> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			Account e{};

			e.id                      = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.name                    = row[1] ? row[1] : "";
			e.charname                = row[2] ? row[2] : "";
			e.sharedplat              = row[3] ? static_cast<uint32_t>(strtoul(row[3], nullptr, 10)) : 0;
			e.password                = row[4] ? row[4] : "";
			e.status                  = row[5] ? static_cast<int32_t>(atoi(row[5])) : 0;
			e.lsaccount_id            = row[6] ? static_cast<uint32_t>(strtoul(row[6], nullptr, 10)) : 0;
			e.gmspeed                 = row[7] ? static_cast<uint8_t>(strtoul(row[7], nullptr, 10)) : 0;
			e.revoked                 = row[8] ? static_cast<uint8_t>(strtoul(row[8], nullptr, 10)) : 0;
			e.karma                   = row[9] ? static_cast<uint32_t>(strtoul(row[9], nullptr, 10)) : 0;
			e.minilogin_ip            = row[10] ? row[10] : "";
			e.hideme                  = row[11] ? static_cast<int8_t>(atoi(row[11])) : 0;
			e.rulesflag               = row[12] ? static_cast<uint8_t>(strtoul(row[12], nullptr, 10)) : 0;
			e.suspendeduntil          = strtoll(row[13] ? row[13] : "-1", nullptr, 10);
			e.time_creation           = row[14] ? static_cast<uint32_t>(strtoul(row[14], nullptr, 10)) : 0;
			e.expansion               = row[15] ? static_cast<int8_t>(atoi(row[15])) : 12;
			e.ban_reason              = row[16] ? row[16] : "";
			e.suspend_reason          = row[17] ? row[17] : "";
			e.active                  = row[18] ? static_cast<int8_t>(atoi(row[18])) : 0;
			e.ip_exemption_multiplier = row[19] ? static_cast<int32_t>(atoi(row[19])) : 1;
			e.gminvul                 = row[20] ? static_cast<int8_t>(atoi(row[20])) : 0;
			e.flymode                 = row[21] ? static_cast<int8_t>(atoi(row[21])) : 0;
			e.ignore_tells            = row[22] ? static_cast<int8_t>(atoi(row[22])) : 0;
			e.mule                    = row[23] ? static_cast<int8_t>(atoi(row[23])) : 0;

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
		const Account &e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.id));
		v.push_back("'" + Strings::Escape(e.name) + "'");
		v.push_back("'" + Strings::Escape(e.charname) + "'");
		v.push_back(std::to_string(e.sharedplat));
		v.push_back("'" + Strings::Escape(e.password) + "'");
		v.push_back(std::to_string(e.status));
		v.push_back(std::to_string(e.lsaccount_id));
		v.push_back(std::to_string(e.gmspeed));
		v.push_back(std::to_string(e.revoked));
		v.push_back(std::to_string(e.karma));
		v.push_back("'" + Strings::Escape(e.minilogin_ip) + "'");
		v.push_back(std::to_string(e.hideme));
		v.push_back(std::to_string(e.rulesflag));
		v.push_back("FROM_UNIXTIME(" + (e.suspendeduntil > 0 ? std::to_string(e.suspendeduntil) : "null") + ")");
		v.push_back(std::to_string(e.time_creation));
		v.push_back(std::to_string(e.expansion));
		v.push_back("'" + Strings::Escape(e.ban_reason) + "'");
		v.push_back("'" + Strings::Escape(e.suspend_reason) + "'");
		v.push_back(std::to_string(e.active));
		v.push_back(std::to_string(e.ip_exemption_multiplier));
		v.push_back(std::to_string(e.gminvul));
		v.push_back(std::to_string(e.flymode));
		v.push_back(std::to_string(e.ignore_tells));
		v.push_back(std::to_string(e.mule));

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
		const std::vector<Account> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.id));
			v.push_back("'" + Strings::Escape(e.name) + "'");
			v.push_back("'" + Strings::Escape(e.charname) + "'");
			v.push_back(std::to_string(e.sharedplat));
			v.push_back("'" + Strings::Escape(e.password) + "'");
			v.push_back(std::to_string(e.status));
			v.push_back(std::to_string(e.lsaccount_id));
			v.push_back(std::to_string(e.gmspeed));
			v.push_back(std::to_string(e.revoked));
			v.push_back(std::to_string(e.karma));
			v.push_back("'" + Strings::Escape(e.minilogin_ip) + "'");
			v.push_back(std::to_string(e.hideme));
			v.push_back(std::to_string(e.rulesflag));
			v.push_back("FROM_UNIXTIME(" + (e.suspendeduntil > 0 ? std::to_string(e.suspendeduntil) : "null") + ")");
			v.push_back(std::to_string(e.time_creation));
			v.push_back(std::to_string(e.expansion));
			v.push_back("'" + Strings::Escape(e.ban_reason) + "'");
			v.push_back("'" + Strings::Escape(e.suspend_reason) + "'");
			v.push_back(std::to_string(e.active));
			v.push_back(std::to_string(e.ip_exemption_multiplier));
			v.push_back(std::to_string(e.gminvul));
			v.push_back(std::to_string(e.flymode));
			v.push_back(std::to_string(e.ignore_tells));
			v.push_back(std::to_string(e.mule));

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

#endif //EQEMU_BASE_ACCOUNT_REPOSITORY_H
