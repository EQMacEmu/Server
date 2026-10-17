/**
 * EQEmulator: Everquest Server Emulator
 * Copyright (C) 2001-2019 EQEmulator Development Team (https://github.com/EQEmu/Server)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY except by those people which sell it, which
 * are required to give you total support for your newly bought product;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include "../common/global_define.h"

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "../common/strings.h"
#include "../common/eqemu_logsys.h"
#include "../common/queue.h"
#include "../common/timer.h"
#include "../common/eq_packet.h"
#include "../common/seperator.h"
#include "../common/version.h"
#include "../common/eqtime.h"
#include "../common/event/event_loop.h"
#include "../common/eq_packet_translator.h"
#include "../common/opcodemgr.h"
#include "../common/guilds.h"
#include "../common/patches/patches.h"
#include "../common/rdp/rdp_connection.h"
#include "../common/rdp/rdp_endpoint.h"
#include "../common/rdp/rdp_runtime.h"
#include "../common/rdp/rdp_stream.h"
#include "../common/rulesys.h"
#include "../common/platform.h"
#include "../common/crash.h"
#include "../common/misc.h"
#include "client.h"
#include "worlddb.h"
#include "wguild_mgr.h"

#ifdef _WINDOWS
#include <process.h>
#define snprintf	_snprintf
#define strncasecmp	_strnicmp
#define strcasecmp	_stricmp
#include <conio.h>
#else

#include <sys/sem.h>
#include <thread>

#endif

#include "zoneserver.h"
#include "login_server.h"
#include "login_server_list.h"
#include "world_config.h"
#include "zonelist.h"
#include "clientlist.h"
#include "launcher_list.h"
#include "ucs.h"
#include "queryserv.h"
#include "web_interface.h"
#include "console.h"

#include "world_server_cli.h"
#include "../common/content/world_content_service.h"
#include "../common/zone_store.h"
#include "world_event_scheduler.h"
#include "world_boot.h"
#include "../common/path_manager.h"
#include "../common/events/player_event_logs.h"
#include "../common/skill_caps.h"

#include <memory>
#include <new>
#include <utility>

LauncherList        launcher_list; 
volatile bool       RunLoops = true;
uint32              numclients = 0;
uint32              numzones = 0;
bool                holdzones = false;
const WorldConfig   *Config;
EQEmuLogSys         LogSys;

void CatchSignal(int sig_num);

inline void UpdateWindowTitle(std::string new_title)
{
#ifdef _WINDOWS
	SetConsoleTitle(new_title.c_str());
#endif
}

/**
 * World process entrypoint
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char** argv) {
	RegisterExecutablePlatform(ExePlatformWorld);
	LogSys.LoadLogSettingsDefaults();
	set_exception_handler();

	if (WorldBoot::HandleCommandInput(argc, argv)) {
		return 0;
	}

	PathManager::Instance()->Init();

	if (!WorldBoot::LoadServerConfig()) {
		return 0;
	}

	Config=WorldConfig::get();

	LogInfo("CURRENT_VERSION: [{0}]", CURRENT_VERSION);

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

	WorldBoot::RegisterLoginservers();
	WorldBoot::LoadDatabaseConnections();
	if (!WorldBoot::DatabaseLoadRoutines(argc, argv)) {
		return 1;
	}

	LogSys.SetDatabase(&database)
		->SetLogPath(PathManager::Instance()->GetLogPath())
		->LoadLogDatabaseSettings()
		->StartFileLogs();

	Timer EQTimeTimer(600000);
	EQTimeTimer.Start(600000);
	Timer player_event_log_process(1000);
	player_event_log_process.Start(1000);

	// global loads
	LogInfo("Loading launcher list..");
	launcher_list.LoadList();
	ZSList::Instance()->Init();

	std::unique_ptr<EQ::Net::ConsoleServer> console;
	if (Config->TelnetEnabled) {
		LogInfo("Console (TCP) listener started on [{}:{}]", Config->TelnetIP, Config->TelnetTCPPort);
		console = std::make_unique<EQ::Net::ConsoleServer>(Config->TelnetIP, Config->TelnetTCPPort);
		RegisterConsoleFunctions(console);
	}

	SkillCaps::Instance()->SetContentDatabase(&database)->LoadSkillCaps();

	std::unique_ptr<EQ::Net::ServertalkServer> server_connection;
	server_connection = std::make_unique<EQ::Net::ServertalkServer>();

	EQ::Net::ServertalkServerOptions server_opts;
	server_opts.port        = Config->WorldTCPPort;
	server_opts.ipv6        = false;
	server_opts.credentials = Config->SharedKey;
	int server_listen_result = server_connection->Listen(server_opts);
	if (server_listen_result != 0) {
		LogError(
			"Failed to start Server (TCP) listener on port [{}]: [{}] ({})",
			Config->WorldTCPPort,
			uv_err_name(server_listen_result),
			uv_strerror(server_listen_result)
		);
		return 1;
	}
	LogInfo("Server (TCP) listener started on port [{}]", Config->WorldTCPPort);
		
	server_connection->OnConnectionIdentified(
		"Zone", [&console](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
			numzones++;
			ZSList::Instance()->Add(new ZoneServer(connection, console.get()));

			LogInfo(
				"New Zone Server connection from [{}] at [{}:{}] zone_count [{}]",
				connection->Handle()->RemoteIP(),
				connection->Handle()->RemotePort(),
				connection->GetUUID(),
				numzones
			);
		}
	);

	server_connection->OnConnectionRemoved(
		"Zone", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
			numzones--;
			ZSList::Instance()->Remove(connection->GetUUID());

			LogInfo(
				"Removed Zone Server connection from [{}] total zone_count [{}]",
				connection->GetUUID(),
				numzones
			);
		}
	);

	server_connection->OnConnectionIdentified(
		"Launcher", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
			LogInfo(
				"New Launcher connection from [{}] at [{}:{}]",
				connection->Handle()->RemoteIP(),
				connection->Handle()->RemotePort(),
				connection->GetUUID()
			);

			launcher_list.Add(connection);
		}
	);

	server_connection->OnConnectionRemoved(
		"Launcher", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
			LogInfo(
				"Removed Launcher connection from [{0}]",
				connection->GetUUID()
			);

			launcher_list.Remove(connection);
		}
	);

	server_connection->OnConnectionIdentified(
		"QueryServ", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
			LogInfo(
				"New Query Server connection from [{}] at [{}:{}]",
				connection->Handle()->RemoteIP(),
				connection->Handle()->RemotePort(),
				connection->GetUUID());

			QueryServConnection::Instance()->AddConnection(connection);
		}
	);

	server_connection->OnConnectionRemoved(
		"QueryServ", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
			LogInfo(
				"Removed Query Server connection from [{}]",
				connection->GetUUID()
			);

			QueryServConnection::Instance()->RemoveConnection(connection);
		}
	);

	server_connection->OnConnectionIdentified(
		"UCS", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
			LogInfo(
				"New UCS Server connection from [{}] at [{}:{}]",
				connection->Handle()->RemoteIP(),
				connection->Handle()->RemotePort(),
				connection->GetUUID()
			);

			UCSConnection::Instance()->SetConnection(connection);

			ZSList::Instance()->UpdateUCSServerAvailable();
		}
	);

	server_connection->OnConnectionRemoved(
		"UCS", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
			LogInfo("Connection lost from UCS Server [{}]", connection->GetUUID());

			auto ucs_connection = UCSConnection::Instance()->GetConnection();

			if (ucs_connection->GetUUID() == connection->GetUUID()) {
				LogInfo("Removing currently active UCS connection");
				UCSConnection::Instance()->SetConnection(nullptr);
				ZSList::Instance()->UpdateUCSServerAvailable(false);
			}
		}
	);

	server_connection->OnConnectionIdentified(
		"WebInterface", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
			LogInfo(
				"New WebInterface Server connection from [{}] at [{}:{}]",
				connection->Handle()->RemoteIP(),
				connection->Handle()->RemotePort(),
				connection->GetUUID()
			);

			WebInterfaceList::Instance()->AddConnection(connection);
		}
	);

	server_connection->OnConnectionRemoved(
		"WebInterface", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
			LogInfo(
				"Removed WebInterface Server connection from [{}]",
				connection->GetUUID()
			);

			WebInterfaceList::Instance()->RemoveConnection(connection);
		}
	);

	WorldBoot::CheckForPossibleConfigurationIssues();

	// Load opcodes and configure packet encode/decode.
	if (!Patches::LoadAll()) {
		LogError("Failed to load patch opcode files");
		return 1;
	}

	RDPRuntime rdp_runtime;
	int rdp_result = rdp_runtime.Open();
	if (rdp_result != RDPLIB_OK) {
		LogError("Failed to open the RDP runtime, result [{}]", rdp_result);
		return 1;
	}

	static constexpr uint16 WorldClientPort = 9000;
	static constexpr uint32 MaximumWorldClientAcceptsPerTick = 5;
	RDPEndpoint rdp_endpoint;
	rdp_result = rdp_endpoint.Open(rdp_runtime, WorldClientPort);
	if (rdp_result != RDPLIB_OK) {
		LogError("Failed to start client RDP listener on port [{}], result [{}]", WorldClientPort, rdp_result);
		return 1;
	}
	LogInfo("Client (RDP/UDP) listener started on port [{}]", WorldClientPort);

	ZSList::Instance()->shutdowntimer = new Timer(60000);
	ZSList::Instance()->shutdowntimer->Disable();
	ZSList::Instance()->reminder = new Timer(20000);
	ZSList::Instance()->reminder->Disable();
	Timer InterserverTimer(INTERSERVER_TIMER); // does MySQL pings and auto-reconnect
	InterserverTimer.Trigger();
	uint8 ReconnectCounter = 100;

	if (PlayerEventLogs::Instance()->LoadDatabaseConnection()) {
		PlayerEventLogs::Instance()->Init();
	}

	auto loop_fn = [&](EQ::Timer* t) {
		Timer::SetCurrentTime();

		if (!RunLoops) {
			EQ::EventLoop::Get().Shutdown();
			return;
		}

		int process_result = rdp_endpoint.Process();
		if (process_result < 0) {
			LogError("Unable to process the world RDP endpoint, result [{}]", process_result);
			RunLoops = false;
			return;
		}

		int accept_result = RDPLIB_OK;
		for (uint32 accepted_connections = 0;
			 accepted_connections < MaximumWorldClientAcceptsPerTick;
			 ++accepted_connections) {
			std::unique_ptr<RDPConnection> connection(rdp_endpoint.Accept(&accept_result));
			if (connection == nullptr)
				break;

			uint8 remote_address[4] = {};
			uint16 remote_port = 0;
			int setup_result = connection->GetRemoteAddress(remote_address, remote_port);
			struct in_addr in = {};
			if (setup_result == RDPLIB_OK)
				memcpy(&in.s_addr, remote_address, sizeof(in.s_addr));

			if (setup_result != RDPLIB_OK) {
				LogError("Unable to read a new world RDP client's address, result [{}]", setup_result);
				continue;
			}

			std::string remote_ip = inet_ntoa(in);
			if (RuleB(World, UseBannedIPsTable)) {
				LogInfo("Checking inbound connection [{}] against BannedIPs table", remote_ip);
				if (database.CheckBannedIPs(remote_ip.c_str())) {
					LogInfo("Connection from [{}] FAILED banned IPs check. Closing connection.", remote_ip);
					connection->Close(0);
					continue;
				}
				LogInfo("Connection [{}] PASSED banned IPs check. Processing connection.", remote_ip);
			}

			std::unique_ptr<RDPStream> stream;
			try {
				stream.reset(new RDPStream(Patches::GetTranslator(EQ::versions::ClientVersion::Mac), std::move(connection)));
			}
			catch (const std::bad_alloc &) {
				accept_result = RDPLIB_ERROR_OUT_OF_MEMORY;
				break;
			}

			setup_result = stream->EnableKeepalive();
			if (setup_result == RDPLIB_OK)
				setup_result = stream->SetDataRate();
			if (setup_result == RDPLIB_OK)
				setup_result = stream->SetSendBufferSize();

			if (setup_result != RDPLIB_OK) {
				LogError("Unable to configure a new world RDP client, result [{}]", setup_result);
				stream->Close(0);
				continue;
			}

			LogInfo("New connection from [{}]:[{}], processing connection", remote_ip, remote_port);
			ClientList::Instance()->Add(new Client(std::move(stream)));
		}

		if (accept_result != RDPLIB_OK) {
			LogError("Unable to accept a world RDP client, result [{}]", accept_result);
			RunLoops = false;
			return;
		}

		WorldEventScheduler::Instance()->Process(ZSList::Instance());

		ClientList::Instance()->Process();
		
		if(EQTimeTimer.Check()) {
			TimeOfDay_Struct tod;
			ZSList::Instance()->worldclock.GetCurrentEQTimeOfDay(time(0), &tod);
			if (!database.SaveTime(tod.minute, tod.hour, tod.day, tod.month, tod.year)) {
				LogEqTime("Failed to save eqtime");
			}
			else {
				LogEqTimeDetail("EQTime successfully saved - time is now year [{}] month [{}] day [{}] hour [{}] minute [{}]",
					tod.year,
					tod.month,
					tod.day,
					tod.hour,
					tod.minute
				);
			}
		}

		ZSList::Instance()->Process();
		launcher_list.Process();

		if (!RuleB(Logging, PlayerEventsQSProcess)) {
			if (player_event_log_process.Check()) {
				PlayerEventLogs::Instance()->Process();
			}
		}

		if (InterserverTimer.Check()) {
			InterserverTimer.Start();
			database.ping();

			std::string window_title = fmt::format(
				"World [{}] Clients [{}]",
				Config->LongName,
				ClientList::Instance()->GetClientCount()
			);
			UpdateWindowTitle(window_title);

			ReconnectCounter++;
			if (ReconnectCounter >= 12) { // only create thread to reconnect every 10 minutes. previously we were creating a new thread every 10 seconds
				ReconnectCounter = 0;
			}
		}
	};

	EQ::Timer process_timer(loop_fn);
	process_timer.Start(32, true);

	EQ::EventLoop::Get().Run();

	LogInfo("World main loop completed.");
	LogInfo("Shutting down game clients.");
	ClientList::Instance()->Clear();
	LogInfo("Shutting down zone connections (if any).");
	ZSList::Instance()->KillAll();
	LogInfo("Zone (TCP) listener stopped.");

	rdp_result = rdp_endpoint.Close();
	if (rdp_result != RDPLIB_OK)
		LogError("Failed to close the world RDP endpoint, result [{}]", rdp_result);

	int runtime_close_result = rdp_runtime.Close();
	if (runtime_close_result != RDPLIB_OK)
		LogError("Failed to close the world RDP runtime, result [{}]", runtime_close_result);
	if (rdp_result == RDPLIB_OK)
		rdp_result = runtime_close_result;

	LogInfo("Client (RDP/UDP) listener stopped.");
	LogSys.CloseFileLogs();

	return rdp_result == RDPLIB_OK ? 0 : 1;
}

void CatchSignal(int sig_num) {
	LogInfo("Caught signal [{}]",sig_num);
	RunLoops = false;
}
