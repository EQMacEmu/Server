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

#define PLATFORM_WORLD 1

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
#include "../common/eq_stream_factory.h"
#include "../common/net/eqstream.h"
#include "../common/opcodemgr.h"
#include "../common/guilds.h"
#include "../common/eq_stream_ident.h"
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

#include "../common/patches/patches.h"
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
#include "world_boot.h"

#include "world_server_cli.h"
#include "../common/content/world_content_service.h"
#include "../common/zone_store.h"
#include "world_event_scheduler.h"
#include "../common/path_manager.h"
#include "../common/events/player_event_logs.h"
#include "../common/skill_caps.h"
#include "../common/ip_util.h"

SkillCaps           skill_caps;
ZoneStore           zone_store;
TimeoutManager      timeout_manager;
EQStreamFactory     eqsf(WorldStream,9000);
ClientList          client_list;
ZSList              zoneserver_list;
LoginServerList     loginserverlist;
UCSConnection       UCSLink;
QueryServConnection QSLink;
LauncherList        launcher_list; 
WorldEventScheduler event_scheduler;
EQ::Random          emu_random;
volatile bool       RunLoops = true;
uint32              numclients = 0;
uint32              numzones = 0;
bool                holdzones = false;
const WorldConfig   *Config;
EQEmuLogSys         LogSys;
WebInterfaceList    web_interface;
WorldContentService content_service;
PathManager         path;
PlayerEventLogs     player_event_logs;

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

	path.LoadPaths();

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

	Timer EQTimeTimer(600000);
	EQTimeTimer.Start(600000);

	// global loads
	LogInfo("Loading launcher list..");
	launcher_list.LoadList();
	zoneserver_list.Init();

	std::unique_ptr<EQ::Net::ConsoleServer> console;
	if (Config->TelnetEnabled) {
		LogInfo("Console (TCP) listener started on [{}:{}]", Config->TelnetIP, Config->TelnetTCPPort);
		console = std::make_unique<EQ::Net::ConsoleServer>(Config->TelnetIP, Config->TelnetTCPPort);
		RegisterConsoleFunctions(console);
	}

	skill_caps.SetContentDatabase(&database)->LoadSkillCaps();

	std::unique_ptr<EQ::Net::ServertalkServer> server_connection;
	server_connection = std::make_unique<EQ::Net::ServertalkServer>();

	EQ::Net::ServertalkServerOptions server_opts;
	server_opts.port        = Config->WorldTCPPort;
	server_opts.ipv6        = false;
	server_opts.credentials = Config->SharedKey;
	server_connection->Listen(server_opts);
	LogInfo("Server (TCP) listener started on port [{}]", Config->WorldTCPPort);
		
	server_connection->OnConnectionIdentified(
		"Zone", [&console](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
			numzones++;
			zoneserver_list.Add(new ZoneServer(connection, console.get()));

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
			zoneserver_list.Remove(connection->GetUUID());

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

			QSLink.AddConnection(connection);
		}
	);

	server_connection->OnConnectionRemoved(
		"QueryServ", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
			LogInfo(
				"Removed Query Server connection from [{}]",
				connection->GetUUID()
			);

			QSLink.RemoveConnection(connection);
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

			UCSLink.SetConnection(connection);

			zoneserver_list.UpdateUCSServerAvailable();

			
		}
	);

	server_connection->OnConnectionRemoved(
		"UCS", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
			LogInfo("Connection lost from UCS Server [{}]", connection->GetUUID());

			auto ucs_connection = UCSLink.GetConnection();

			if (ucs_connection->GetUUID() == connection->GetUUID()) {
				LogInfo("Removing currently active UCS connection");
				UCSLink.SetConnection(nullptr);
				zoneserver_list.UpdateUCSServerAvailable(false);
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

			web_interface.AddConnection(connection);
		}
	);

	server_connection->OnConnectionRemoved(
		"WebInterface", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
			LogInfo(
				"Removed WebInterface Server connection from [{}]",
				connection->GetUUID()
			);

			web_interface.RemoveConnection(connection);
		}
	);

	WorldBoot::CheckForPossibleConfigurationIssues();

	if (eqsf.Open()) {
		LogInfo("Client (UDP) listener started.");
	} else {
		LogInfo("Failed to start client (UDP) listener (port 9000)");
		return 1;
	}

	//register all the patches we have avaliable with the stream identifier.
	EQStreamIdentifier stream_identifier;
	RegisterAllPatches(stream_identifier);
	zoneserver_list.shutdowntimer = new Timer(60000);
	zoneserver_list.shutdowntimer->Disable();
	zoneserver_list.reminder = new Timer(20000);
	zoneserver_list.reminder->Disable();
	Timer InterserverTimer(INTERSERVER_TIMER); // does MySQL pings and auto-reconnect
	InterserverTimer.Trigger();
	uint8 ReconnectCounter = 100;
	std::shared_ptr<EQStream> eqs;
	std::shared_ptr<EQOldStream> eqos;
	EQStreamInterface *eqsi;

	Timer player_event_process_timer(1000);
	player_event_logs.SetDatabase(&database)->Init();

	auto loop_fn = [&](EQ::Timer* t) {
		Timer::SetCurrentTime();

		if (!RunLoops) {
			EQ::EventLoop::Get().Shutdown();
			return;
		}

		//give the stream identifier a chance to do its work....
		stream_identifier.Process();

		int i = 5;
		//check the factory for any new incoming streams.
		while ((eqs = eqsf.Pop())) {
			//pull the stream out of the factory and give it to the stream identifier
			//which will figure out what patch they are running, and set up the dynamic
			//structures and opcodes for that patch.
			struct in_addr	in{};
			in.s_addr = eqs->GetRemoteIP();
			LogInfo("New connection from {0}:{1}", inet_ntoa(in),ntohs(eqs->GetRemotePort()));
			stream_identifier.AddStream(eqs);	//takes the stream
			i++;
			if (i == 5)
				break;
		}

		i = 0;
		//check the factory for any new incoming streams.
		while ((eqos = eqsf.PopOld())) {
			//pull the stream out of the factory and give it to the stream identifier
			//which will figure out what patch they are running, and set up the dynamic
			//structures and opcodes for that patch.
			struct in_addr	in{};
			in.s_addr = eqos->GetRemoteIP();
			LogInfo("New connection from {0}:{1}", inet_ntoa(in), ntohs(eqos->GetRemotePort()));
			stream_identifier.AddOldStream(eqos);	//takes the stream
			i++;
			if (i == 5)
				break;
		}

		//check the stream identifier for any now-identified streams
		while((eqsi = stream_identifier.PopIdentified())) {
			//now that we know what patch they are running, start up their client object
			struct in_addr	in{};
			in.s_addr = eqsi->GetRemoteIP();
			if (RuleB(World, UseBannedIPsTable)){ //Lieka: Check to see if we have the responsibility for blocking IPs.
				LogInfo("Checking inbound connection [{0}] against BannedIPs table", inet_ntoa(in));
				if (!database.CheckBannedIPs(inet_ntoa(in))){ //Lieka: Check inbound IP against banned IP table.
					LogInfo("Connection [{0}] PASSED banned IPs check. Processing connection.", inet_ntoa(in));
					auto client = new Client(eqsi);
					// @merth: client->zoneattempt=0;
					client_list.Add(client);
				} else {
					LogInfo("Connection from [{0}] FAILED banned IPs check. Closing connection.", inet_ntoa(in));
					eqsi->Close(); //Lieka: If the inbound IP is on the banned table, close the EQStream.
				}
			}
			if (!RuleB(World, UseBannedIPsTable)){
				LogInfo(
					"New connection from [{}]:[{}], processing connection",
					inet_ntoa(in),
					ntohs(eqsi->GetRemotePort())
				);
				auto client = new Client(eqsi);
				// @merth: client->zoneattempt=0;
				client_list.Add(client);
			}
		}

		event_scheduler.Process(&zoneserver_list);

		client_list.Process();
		
		if (player_event_process_timer.Check()) {
			player_event_logs.Process();
		}

		if(EQTimeTimer.Check()) {
			TimeOfDay_Struct tod;
			zoneserver_list.worldclock.GetCurrentEQTimeOfDay(time(0), &tod);
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

		//check for timeouts in other threads
		timeout_manager.CheckTimeouts();
		zoneserver_list.Process();
		launcher_list.Process();

		if (InterserverTimer.Check()) {
			InterserverTimer.Start();
			database.ping();

			std::string window_title = fmt::format(
				"World [{}] Clients [{}]",
				Config->LongName,
				client_list.GetClientCount()
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
	LogInfo("Shutting down zone connections (if any).");
	zoneserver_list.KillAll();
	LogInfo("Zone (TCP) listener stopped.");
	LogInfo("Client (UDP) listener stopped.");
	eqsf.Close();
	LogSys.CloseFileLogs();

	return 0;
}

void CatchSignal(int sig_num) {
	LogInfo("Caught signal [{}]",sig_num);
	RunLoops = false;
}
