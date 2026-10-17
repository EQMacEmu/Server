/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2002 EQEMu Development Team (http://eqemu.org)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#define PLATFORM_ZONE 1

#include "../common/global_define.h"
#include "../common/features.h"
#include "../common/queue.h"
#include "../common/eq_packet_translator.h"
#include "../common/eq_packet_structs.h"
#include "../common/mutex.h"
#include "../common/version.h"
#include "../common/guilds.h"
#include "../common/patches/patches.h"
#include "../common/rdp/rdp_connection.h"
#include "../common/rdp/rdp_endpoint.h"
#include "../common/rdp/rdp_runtime.h"
#include "../common/rdp/rdp_stream.h"
#include "../common/rulesys.h"
#include "../common/profanity_manager.h"
#include "../common/misc_functions.h"
#include "../common/strings.h"
#include "../common/platform.h"
#include "../common/crash.h"
#include "../common/ipc_mutex.h"
#include "../common/memory_mapped_file.h"
#include "../common/eqemu_exception.h"
#include "../common/spdat.h"
#include "../common/eqemu_logsys.h"
#include "../common/timer.h"
#include "../common/zone_store.h"
#include "../common/content/world_content_service.h"
#include "../common/repositories/content_flags_repository.h"
#include "../common/skill_caps.h"

#include "api_service.h"
#include "zonedb.h"
#include "zone_config.h"
#include "masterentity.h"
#include "worldserver.h"
#include "zone.h"
#include "queryserv.h"
#include "command.h"
#include "titles.h"
#include "guild_mgr.h"
#include "quest_parser_collection.h"
#include "lua_parser.h"
#include "questmgr.h"

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <ctime>
#include <chrono>
#include <memory>
#include <new>
#include <utility>

#ifdef _CRTDBG_MAP_ALLOC
	#undef new
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#ifdef _WINDOWS
#include <conio.h>
#include <process.h>
#else
#include <pthread.h>
#include "../common/unix.h"
#endif

volatile bool RunLoops = true;
extern volatile bool is_zone_loaded;

#include "../common/file.h"
#include "../common/path_manager.h"
#include "../common/events/player_event_logs.h"
#include "zone_cli.h"

EntityList  entity_list;
WorldServer worldserver;
uint32      numclients = 0;
char        errorname[32];
extern Zone *zone;

TitleManager          title_manager;
QueryServ             *QServ = 0;
QuestParserCollection *parse = 0;
EQEmuLogSys           LogSys;

const SPDat_Spell_Struct* spells;
int32 SPDAT_RECORDS = -1;
const ZoneConfig *Config;
double frame_time = 0.0;

void Shutdown();
void UpdateWindowTitle(char* iNewTitle);
void CatchSignal(int sig_num);

extern void MapOpcodes();

int main(int argc, char** argv) {
	RegisterExecutablePlatform(ExePlatformZone); 
	LogSys.LoadLogSettingsDefaults();
	
	set_exception_handler();

	// silence logging if we ran a command
	if (ZoneCLI::RanConsoleCommand(argc, argv)) {
		LogSys.SilenceConsoleLogging();
	}

	PathManager::Instance()->Init();

	QServ = new QueryServ;

	LogInfo("Loading server configuration..");
	if(!ZoneConfig::LoadConfig()) {
		LogError("Loading server configuration failed.");
		return 1;
	}
	Config = ZoneConfig::get();

	// static zone booting
	const char *zone_name;
	std::string z_name;
	if (!ZoneCLI::RanSidecarCommand(argc, argv)) {
		if (argc == 4) {
			worldserver.SetLauncherName(argv[2]);
			auto zone_port = Strings::Split(argv[1], ':');

			if (!zone_port.empty()) {
				z_name = zone_port[0];
			}

			if (zone_port.size() > 1) {
				std::string p_name = zone_port[1];
				Config->SetZonePort(atoi(p_name.c_str()));
			}

			worldserver.SetLaunchedName(z_name.c_str());
			if (strncmp(z_name.c_str(), "dynamic_", 8) == 0) {
				zone_name = ".";
			}
			else {
				zone_name = z_name.c_str();
			}
		}
		else if (argc == 3) {
			worldserver.SetLauncherName(argv[2]);
			auto zone_port = Strings::Split(argv[1], ':');

			if (!zone_port.empty()) {
				z_name = zone_port[0];
			}

			if (zone_port.size() > 1) {
				std::string p_name = zone_port[1];
				Config->SetZonePort(atoi(p_name.c_str()));
			}

			worldserver.SetLaunchedName(z_name.c_str());
			if (strncmp(z_name.c_str(), "dynamic_", 8) == 0) {
				zone_name = ".";
			}
			else {
				zone_name = z_name.c_str();
			}
		}
		else if (argc == 2) {
			worldserver.SetLauncherName("NONE");
			auto zone_port = Strings::Split(argv[1], ':');

			if(!zone_port.empty()) {
				z_name = zone_port[0];
			}

			if(zone_port.size() > 1) {
				std::string p_name = zone_port[1];
				Config->SetZonePort(atoi(p_name.c_str()));
			}

			worldserver.SetLaunchedName(z_name.c_str());
			if(strncmp(z_name.c_str(), "dynamic_", 8) == 0) {
				zone_name = ".";
			}
			else {
				zone_name = z_name.c_str();
			}
		} 
		else {
			zone_name = ".";
			worldserver.SetLaunchedName(".");
			worldserver.SetLauncherName("NONE");
		}
	}

	LogInfo("Connecting to MySQL...");
	if (!database.Connect(
		Config->DatabaseHost.c_str(),
		Config->DatabaseUsername.c_str(),
		Config->DatabasePassword.c_str(),
		Config->DatabaseDB.c_str(),
		Config->DatabasePort)) {
		LogError("Cannot continue without a database connection.");
		return 1;
	}

	//rules:
	{
		std::string tmp;
		if (database.GetVariable("RuleSet", tmp)) {
			LogInfo("Loading rule set '{}'", tmp.c_str());
			if (!RuleManager::Instance()->LoadRules(&database, tmp.c_str())) {
				LogError("Failed to load ruleset '{}', falling back to defaults.", tmp.c_str());
			}
		}
		else {
			if (!RuleManager::Instance()->LoadRules(&database, "default")) {
				LogInfo("No rule set configured, using default rules");
			}
			else {
				LogInfo("Loaded default rule set 'Default'");
			}
		}
	}

	// command handler
	if (ZoneCLI::RanConsoleCommand(argc, argv) && !ZoneCLI::RanSidecarCommand(argc, argv)) {
		LogSys.EnableConsoleLogging();
		ZoneCLI::CommandHandler(argc, argv);
	}

	LogSys.SetDatabase(&database)
		->SetLogPath(PathManager::Instance()->GetLogPath())
		->LoadLogDatabaseSettings()
		->SetGMSayHandler(&Zone::GMSayHookCallBackProcess)
		->StartFileLogs();

	PlayerEventLogs::Instance()->SetDatabase(&database)->Init();

	SkillCaps::Instance()->SetContentDatabase(&database)->LoadSkillCaps();

	/* Guilds */
	guild_mgr.SetDatabase(&database);

#ifdef _EQDEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	LogInfo("CURRENT_VERSION: {} ", CURRENT_VERSION);

	/*
	* Setup nice signal handlers
	*/
	if (signal(SIGINT, CatchSignal) == SIG_ERR)	{
		LogError("Could not set signal handler");
		return 1;
	}
	if (signal(SIGTERM, CatchSignal) == SIG_ERR)	{
		LogError("Could not set signal handler");
		return 1;
	}
	#ifndef WIN32
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)	{
		LogError("Could not set signal handler");
		return 1;
	}
	#endif

	MapOpcodes();

	database.LoadVariables();

	std::string hotfix_name;
	if(database.GetVariable("hotfix_name", hotfix_name)) {
		if(!hotfix_name.empty()) {
			LogInfo("Current hotfix in use: [{}]", hotfix_name.c_str());
		}
	}

	ZoneStore::Instance()->LoadZones(database);

	if (ZoneStore::Instance()->GetZones().empty()) {
		LogError("Failed to load zones data, check your schema for possible errors");
		return 1;
	}

	// load these here for now
	database.SetSharedItemsCount(database.GetItemsCount());
	database.SetSharedSpellsCount(database.GetSpellsCount());

	if(!database.LoadItems(hotfix_name)) {
		LogError("Loading items FAILED!");
		LogError("Failed. But ignoring error and going on...");
	}

	if(!database.LoadSpells(hotfix_name, &SPDAT_RECORDS, &spells)) {
		LogError("Loading spells FAILED!");
		return 1;
	}

	guild_mgr.LoadGuilds();
	database.LoadFactionData();
	title_manager.LoadTitles();
	database.LoadAlternateAdvancementActions();
	
	if (!EQ::ProfanityManager::LoadProfanityList(&database)) {
		LogInfo("Loading profanity list FAILED!");
	}

	int retval=command_init();
	if (retval < 0) {
		LogError("Command loading FAILED");
	}
	else {
		LogInfo("Loaded [{}] commands loaded", Strings::Commify(std::to_string(retval)));
	}

	WorldContentService::Instance()->SetDatabase(&database)
		->SetExpansionContext()
		->ReloadContentFlags();

	ZoneEventScheduler::Instance()->SetDatabase(&database)->LoadScheduledEvents();

	parse = new QuestParserCollection();
#ifdef LUA_EQEMU
	auto lua_parser = new LuaParser();
	parse->RegisterQuestInterface(lua_parser, "lua");
#endif

	//now we have our parser, load the quests
	LogInfo("Loading quests");
	parse->ReloadQuests();

	QServ->CheckForConnectState();

	worldserver.Connect();
	worldserver.SetScheduler(ZoneEventScheduler::Instance());

	// sidecar command handler
	if (ZoneCLI::RanConsoleCommand(argc, argv) && ZoneCLI::RanSidecarCommand(argc, argv)) {
		ZoneCLI::CommandHandler(argc, argv);
	}

	Timer InterserverTimer(INTERSERVER_TIMER); // does MySQL pings and auto-reconnect
	Timer RemoteCallProcessTimer(5000);
#ifdef EQPROFILE
#ifdef PROFILE_DUMP_TIME
	Timer profile_dump_timer(PROFILE_DUMP_TIME*1000);
	profile_dump_timer.Start();
#endif
#endif
	if (!strlen(zone_name) || !strcmp(zone_name,".")) {
		LogInfo("Entering sleep mode");
	}
	else if (!Zone::Bootup(ZoneID(zone_name), true)) {
		LogError("Zone Bootup failed :: Zone::Bootup");
		zone = nullptr;
	}

	// load opcodes and configure packet encode/decode
	if (!Patches::LoadAll())
	{
		LogError("Failed to load patch opcode files");
		return 1;
	}

	// RDP init
	RDPRuntime rdp_runtime;
	int rdp_result = rdp_runtime.Open();
	if (rdp_result != RDPLIB_OK)
	{
		LogError("Failed to open the RDP runtime, result [{}]", rdp_result);
		return 1;
	}
	RDPEndpoint rdp_endpoint;

#ifdef __linux__
	LogDebug("Main thread running with thread id [{}]", pthread_self());
#elif defined(__FreeBSD__)
	LogDebug("Main thread running with thread id [{}]", pthread_getthreadid_np());
#endif

	bool worldwasconnected = worldserver.Connected();
	bool websocker_server_opened = false;

	Timer quest_timers(100);
	UpdateWindowTitle(nullptr);
	std::chrono::time_point<std::chrono::steady_clock> frame_prev = std::chrono::steady_clock::now();
	std::unique_ptr<EQ::Net::WebsocketServer>          ws_server;

	auto loop_fn = [&](EQ::Timer* t) {
		{	
			if (!RunLoops)
			{
				EQ::EventLoop::Get().Shutdown();
				// Client teardown still consults zone state and releases streams before endpoint shutdown.
				entity_list.Clear();
				Zone::Shutdown(true);
				return;
			}

			//profiler block to omit the sleep from times
			//Advance the timer to our current point in time
			Timer::SetCurrentTime();

			/**
			* Calculate frame time
			*/
			std::chrono::time_point<std::chrono::steady_clock> frame_now = std::chrono::steady_clock::now();
			frame_time = std::chrono::duration<double>(frame_now - frame_prev).count();
			frame_prev = frame_now;

			/**
			* Websocket server
			*/
			if (!websocker_server_opened && Config->ZonePort != 0) {
				LogInfo("Websocket Server listener started on address [{}] port [{}]", Config->TelnetIP.c_str(), Config->ZonePort);
				ws_server = std::make_unique<EQ::Net::WebsocketServer>(Config->TelnetIP, Config->ZonePort);
				RegisterApiService(ws_server);
				websocker_server_opened = true;
			}

			// start listening if network port is configured
			if (!rdp_endpoint.IsOpen() && Config->ZonePort != 0) {
				LogInfo("Starting EQ Network server on port {} ", Config->ZonePort);
				int open_result = rdp_endpoint.Open(rdp_runtime, Config->ZonePort);
				if (open_result != RDPLIB_OK) {
					LogError("Failed to open port {}, RDP result [{}]", Config->ZonePort, open_result);
					ZoneConfig::SetZonePort(0);
					worldwasconnected = false;
				}
			}

			if (rdp_endpoint.IsOpen())
			{
				// consume incoming network traffic
				int process_result = rdp_endpoint.Process();
				if (process_result < 0)
				{
					LogError("Unable to process the RDP endpoint, result [{}]", process_result);
					RunLoops = false;
				}
				else
				{
					int accept_result = RDPLIB_OK;
					for (;;)
					{
						std::unique_ptr<RDPConnection> connection(rdp_endpoint.Accept(&accept_result));
						if (connection == nullptr)
							break;

						std::unique_ptr<RDPStream> stream;
						try
						{
							stream.reset(new RDPStream(Patches::GetTranslator(EQ::versions::ClientVersion::Mac), std::move(connection)));
						}
						catch (const std::bad_alloc &)
						{
							accept_result = RDPLIB_ERROR_OUT_OF_MEMORY;
							break;
						}

						uint8 remote_address[4] = {};
						uint16 remote_port = 0;
						int setup_result = stream->GetRemoteAddress(remote_address, remote_port);
						if (setup_result == RDPLIB_OK)
						{
							uint32 remote_ip = *(uint32*)remote_address;
							uint32 version_bit = zone ? zone->GetAuthVersionByIP(remote_ip) : 0;
							if (version_bit == EQ::versions::ClientVersionBit::bit_Trilogy) {
								stream->SetTranslator(Patches::GetTranslator(EQ::versions::ClientVersion::Trilogy));
							}
						}
						// solar: the AK server used the default 10 second keepalive frequency.  the client also uses a 10 second keepalive.
						// The 500ms configuration here is only to keep the network status meter in the client at 0.0%.
						// This is a deviation from AKurate; the meter did not stay at 0.0% on AK.  Verified in AK pcaps.
						// The client meter adds a gap every 500ms if no reliable packet was received, but it is normal to go up
						// to 6 seconds between reliable packets in an otherwise quiet zone.
						if (setup_result == RDPLIB_OK)
							setup_result = stream->EnableKeepalive(500);
						if (setup_result == RDPLIB_OK)
							setup_result = stream->SetDataRate();
						if (setup_result == RDPLIB_OK)
							setup_result = stream->SetSendBufferSize();

						if (setup_result != RDPLIB_OK)
						{
							LogError("Unable to configure a new RDP client, result [{}]", setup_result);
							stream->Close(0);
							continue;
						}

						LogInfo(
							"New client from [{}.{}.{}.{}]:[{}]",
							static_cast<uint32>(remote_address[0]),
							static_cast<uint32>(remote_address[1]),
							static_cast<uint32>(remote_address[2]),
							static_cast<uint32>(remote_address[3]),
							remote_port
						);
						auto client = new Client(std::move(stream));
						entity_list.AddClient(client);
					}

					if (accept_result != RDPLIB_OK)
					{
						LogError("Unable to accept an RDP client, result [{}]", accept_result);
						RunLoops = false;
					}
				}
			}

			if (worldserver.Connected()) {
				worldwasconnected = true;
			}
			else {
				if (worldwasconnected && is_zone_loaded) {
					entity_list.ChannelMessageFromWorld(0, 0, ChatChannel_Broadcast, 0, 0, 100, "WARNING: World server connection lost");
					worldwasconnected = false;
				}
			}

			if (is_zone_loaded)
			{
				entity_list.GroupProcess();
				entity_list.DoorProcess();
				entity_list.ObjectProcess();
				entity_list.CorpseProcess();
				entity_list.CorpseDepopProcess();
				entity_list.TrapProcess();
				entity_list.RaidProcess();

				entity_list.Process();
				entity_list.MobProcess();
				entity_list.BeaconProcess();
				entity_list.EncounterProcess();
				ZoneEventScheduler::Instance()->Process(zone, WorldContentService::Instance());

				if (zone) {
					// this was put in to appease concerns about the RNG being affected by the time of day or day of week the server was started on, resulting in bad loot
					// The idea is that as the zone processing runs, it takes variable amounts of time based on external factors like player behavior and causes this discard
					// code to execute less or more often based on those factors.
					// By discarding some numbers from the RNG sequence, we hope to add some amount of unpredictability and offset the 'bad loot seed' from startup.
					if (Timer::GetCurrentTime() % 3 == 0) {
						zone->random.Discard(Timer::GetCurrentTime() % 5 + 1); // arbitrary value but discarding more causes more slowdowns as it 'refills'
					}
					if (!zone->Process()) {
						Zone::Shutdown();
					}
				}

				if (quest_timers.Check()) {
					quest_manager.Process();
				}

				entity_list.FlushPositionUpdates();

			}

			QServ->CheckForConnectState();

			if (InterserverTimer.Check()) {
				InterserverTimer.Start();
				database.ping();
				// AsyncLoadVariables(dbasync, &database);
				entity_list.UpdateWho();

			}

#ifdef EQPROFILE
#ifdef PROFILE_DUMP_TIME
			if (profile_dump_timer.Check()) {
				DumpZoneProfile();
			}
#endif
#endif
		}	//end extra profiler block 
	};

	EQ::Timer process_timer(loop_fn);
	process_timer.Start(32, true);

	EQ::EventLoop::Get().Run();

	entity_list.Clear();
	entity_list.RemoveAllEncounters(); // gotta do it manually or rewrite lots of shit :P

	parse->ClearInterfaces();

#ifdef LUA_EQEMU
	safe_delete(lua_parser);
#endif

	safe_delete(Config);

	if (zone != 0) {
		Zone::Shutdown(true);
	}
	rdp_result = rdp_endpoint.Close();
	if (rdp_result != RDPLIB_OK)
		LogError("Failed to close the RDP endpoint, result [{}]", rdp_result);

	int runtime_close_result = rdp_runtime.Close();
	if (runtime_close_result != RDPLIB_OK)
		LogError("Failed to close the RDP runtime, result [{}]", runtime_close_result);
	if (rdp_result == RDPLIB_OK)
		rdp_result = runtime_close_result;

	command_deinit();
	safe_delete(parse);
	LogInfo("Proper zone shutdown complete.");
	LogSys.CloseFileLogs();

	safe_delete(QServ);

	return rdp_result == RDPLIB_OK ? 0 : 1;
}

void Shutdown()
{
	LogInfo("Shutting down...");
	RunLoops = false;
}

void CatchSignal(int sig_num) {
#ifdef _WINDOWS
	LogInfo("Recieved signal: {} ", sig_num);
#endif
	Shutdown();
}


/* Update Window Title with relevant information */
void UpdateWindowTitle(char* iNewTitle) {
#ifdef _WINDOWS
	char tmp[500];
	if (iNewTitle) {
		snprintf(tmp, sizeof(tmp), "%i: %s", ZoneConfig::get()->ZonePort, iNewTitle);
	}
	else {
		if (zone) {
			#if defined(GOTFRAGS) || defined(_EQDEBUG)
				snprintf(tmp, sizeof(tmp), "%i: %s, %i clients, %i", ZoneConfig::get()->ZonePort, zone->GetShortName(), numclients, getpid());
			#else
			snprintf(tmp, sizeof(tmp), "%s :: clients: %i :: port: %i", zone->GetShortName(), numclients, ZoneConfig::get()->ZonePort);
			#endif
		}
		else {
			#if defined(GOTFRAGS) || defined(_EQDEBUG)
				snprintf(tmp, sizeof(tmp), "%i: sleeping, %i", ZoneConfig::get()->ZonePort, getpid());
			#else
				snprintf(tmp, sizeof(tmp), "%i: sleeping", ZoneConfig::get()->ZonePort);
			#endif
		}
	}
	SetConsoleTitle(tmp);
#endif
}
