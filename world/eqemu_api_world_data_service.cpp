#include <fmt/format.h>
#include "clientlist.h"
#include "cliententry.h"
#include "eqemu_api_world_data_service.h"
#include "zoneserver.h"
#include "zonelist.h"
#include "../common/database_schema.h"
#include "../common/server_reload_types.h"
#include "../common/zone_store.h"
#include "worlddb.h"
#include "wguild_mgr.h"
#include "world_config.h"
#include "ucs.h"
#include "queryserv.h"

extern ZSList zoneserver_list;
extern ClientList client_list;
extern WorldGuildManager guild_mgr;
extern UCSConnection UCSLink;
extern QueryServConnection QSLink;

void callGetZoneList(Json::Value &response)
{
	for (auto &zone: zoneserver_list.getZoneServerList()) {
		Json::Value row;

		if (!zone) {
			continue;
		}

		if (!zone->IsConnected()) {
			continue;
		}

		row["booting_up"]           = zone->IsBootingUp();
		row["client_address"]       = zone->GetCAddress();
		row["client_local_address"] = zone->GetCLocalAddress();
		row["client_port"]          = zone->GetCPort();
		row["compile_time"]         = zone->GetCompileTime();
		row["id"]                   = zone->GetID();
		row["ip"]                   = zone->GetIP();
		row["is_static_zone"]       = zone->IsStaticZone();
		row["launch_name"]          = zone->GetLaunchName();
		row["launched_name"]        = zone->GetLaunchedName();
		row["number_players"]       = zone->NumPlayers();
		row["port"]                 = zone->GetPort();
		row["previous_zone_id"]     = zone->GetPrevZoneID();
		row["uuid"]                 = zone->GetUUID();
		row["zone_id"]              = zone->GetZoneID();
		row["zone_long_name"]       = zone->GetZoneLongName();
		row["zone_name"]            = zone->GetZoneName();
		row["zone_os_pid"]          = zone->GetZoneOSProcessID();

		response.append(row);
	}
}

void callGetDatabaseSchema(Json::Value &response)
{
	Json::Value              player_tables_json;
	std::vector<std::string> player_tables = DatabaseSchema::GetPlayerTables();
	for (const auto &table : player_tables) {
		player_tables_json.append(table);
	}

	Json::Value              content_tables_json;
	std::vector<std::string> content_tables = DatabaseSchema::GetContentTables();
	for (const auto &table : content_tables) {
		content_tables_json.append(table);
	}

	Json::Value              server_tables_json;
	std::vector<std::string> server_tables = DatabaseSchema::GetServerTables();
	for (const auto &table : server_tables) {
		server_tables_json.append(table);
	}

	Json::Value              login_tables_json;
	std::vector<std::string> login_tables = DatabaseSchema::GetLoginTables();
	for (const auto& table : login_tables) {
		login_tables_json.append(table);
	}

	Json::Value              state_tables_json;
	std::vector<std::string> state_tables = DatabaseSchema::GetStateTables();
	for (const auto &table : state_tables) {
		state_tables_json.append(table);
	}

	Json::Value              version_tables_json;
	std::vector<std::string> version_tables = DatabaseSchema::GetVersionTables();
	for (const auto& table : version_tables) {
		version_tables_json.append(table);
	}

	Json::Value                        character_table_columns_json;
	std::map<std::string, std::string> character_table = DatabaseSchema::GetCharacterTables();
	for (const auto& ctc : character_table) {
		character_table_columns_json[ctc.first] = ctc.second;
	}

	Json::Value schema;

	schema["character_table_columns"] = character_table_columns_json;
	schema["content_tables"] = content_tables_json;
	schema["login_tables"] = login_tables_json;
	schema["player_tables"] = player_tables_json;
	schema["server_tables"] = server_tables_json;
	schema["state_tables"] = state_tables_json;
	schema["version_tables"] = version_tables_json;

	response.append(schema);
}

void callGetClientList(Json::Value &response, const std::vector<std::string> &args)
{
	// if args has "full"
	bool full_list = false;
	if (args.size() > 1) {
		if (args[1] == "full") {
			full_list = true;
		}
	}

	client_list.GetClientList(response, full_list);
}

void getReloadTypes(Json::Value& response)
{
	for (auto &t : ServerReload::GetTypes()) {
		Json::Value v;

		v["command"] = std::to_string(t);
		v["description"] = ServerReload::GetName(t);
		response.append(v);
	}
}

void getServerCounts(Json::Value &response, const std::vector<std::string> &args)
{
	response["zone_count"] = zoneserver_list.GetServerListCount();
	response["client_count"] = client_list.GetClientCount();
}

void EQEmuApiWorldDataService::reload(Json::Value& r, const std::vector<std::string>& args)
{
	std::vector<std::string> commands{};
	commands.reserve(ServerReload::GetTypes().size());
	for (auto &c : ServerReload::GetTypes()) {
		commands.emplace_back(std::to_string(c));
	}

	std::string command = !args[1].empty() ? args[1] : "";
	if (command.empty()) {
		message(r, fmt::format("Need to provide a type ID to reload. Example(s) [{}]", Strings::Implode("|", commands)));
		return;
	}

	ServerPacket* pack = nullptr;

	bool found_command = false;

	for (auto &t : ServerReload::GetTypes()) {
		if (std::to_string(t) == command || Strings::ToLower(ServerReload::GetName(t)) == command) {
			message(r, fmt::format("Reloading [{}] globally", ServerReload::GetName(t)));
			LogInfo("Queueing reload of type [{}] to zones", ServerReload::GetName(t));
			zoneserver_list.QueueServerReload(t);
		}
		found_command = true;
	}

	if (!found_command) {
		message(r, fmt::format("Need to provide a type to reload. Example(s) [{}]", Strings::Implode("|", commands)));
		return;
	}
}

void EQEmuApiWorldDataService::message(Json::Value& r, const std::string& message)
{
	r["message"] = message;
}

void EQEmuApiWorldDataService::get(Json::Value& r, const std::vector<std::string>& args)
{
	const std::string& m = args[0];

	if (m == "get_zone_list") {
		callGetZoneList(r);
	}
	if (m == "get_database_schema") {
		callGetDatabaseSchema(r);
	}
	if (m == "get_client_list") {
		callGetClientList(r, args);
	}
	if (m == "get_reload_types") {
		getReloadTypes(r);
	}
	if (m == "reload") {
		reload(r, args);
	}
	if (m == "get_server_counts") {
		getServerCounts(r, args);
	}
}