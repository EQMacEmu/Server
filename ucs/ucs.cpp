/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2008 EQEMu Development Team (http://eqemulator.net)

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

#include "../common/eqemu_logsys.h"
#include "../common/global_define.h"
#include "clientlist.h"
#include "../common/opcodemgr.h"
#include "../common/rulesys.h"
#include "../common/servertalk.h"
#include "../common/platform.h"
#include "../common/crash.h"
#include "../common/strings.h"
#include "../common/event/event_loop.h"
#include "database.h"
#include "ucsconfig.h"
#include "chatchannel.h"
#include "worldserver.h"
#include <list>
#include <signal.h>
#include <csignal>
#include <thread>

#include "../common/net/tcp_server.h"
#include "../common/net/servertalk_client_connection.h"
#include "../common/path_manager.h"
#include "../common/zone_store.h"
#include "../common/content/world_content_service.h"
#include "../common/discord/discord_manager.h"
#include "../common/events/player_event_logs.h"

ChatChannelList *ChannelList;
Clientlist *g_Clientlist;
EQEmuLogSys LogSys;
UCSDatabase database;
WorldServerList *worldserverlist = nullptr;

const ucsconfig *Config;

uint32 ChatMessagesSent = 0;

void crash_func() {
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	int* p = 0;
	*p = 0;
}

void Shutdown() {
	LogInfo("Shutting down...");
	ChannelList->RemoveAllChannels();
	g_Clientlist->CloseAllConnections();
	LogSys.CloseFileLogs();
}

int caught_loop = 0;
void CatchSignal(int sig_num) {
	LogInfo("Caught signal [{}]", sig_num);

	EQ::EventLoop::Get().Shutdown();

	caught_loop++;
	// when signal handler is incapable of exiting properly
	if (caught_loop > 1) {
		LogInfo("In a signal handler loop and process is incapable of exiting properly, forcefully cleaning up");
		ChannelList->RemoveAllChannels();
		g_Clientlist->CloseAllConnections();
		LogSys.CloseFileLogs();
		std::exit(0);
	}
}

void PlayerEventQueueListener() {
	while (caught_loop == 0) {
		DiscordManager::Instance()->ProcessMessageQueue();
		Sleep(100);
	}
}

int main() {
	RegisterExecutablePlatform(ExePlatformUCS);
	LogSys.LoadLogSettingsDefaults();
	set_exception_handler();

	PathManager::Instance()->Init();

	// Check every minute for unused channels we can delete
	//
	Timer ChannelListProcessTimer(60000);
	Timer ClientConnectionPruneTimer(60000);

	Timer keepalive(INTERSERVER_TIMER); // does auto-reconnect

	LogInfo("Starting EQEmu Universal Chat Server.");

	if (!ucsconfig::LoadConfig()) { 
		LogInfo("Loading server configuration failed."); 
		return 1;
	}

	Config = ucsconfig::get(); 

	LogInfo("Connecting to MySQL");

	if (!database.Connect(
		Config->DatabaseHost.c_str(),
		Config->DatabaseUsername.c_str(),
		Config->DatabasePassword.c_str(),
		Config->DatabaseDB.c_str(),
		Config->DatabasePort)) {
		LogInfo("Cannot continue without a database connection.");
		return 1;
	}

	LogSys.SetDatabase(&database)
		->SetLogPath(PathManager::Instance()->GetLogPath())
		->LoadLogDatabaseSettings()
		->StartFileLogs();

	PlayerEventLogs::Instance()->SetDatabase(&database)->Init();

	char tmp[64];

	if (database.GetVariable("RuleSet", tmp, sizeof(tmp) - 1)) {
		LogInfo("Loading rule set '[{0}]'", tmp);
		if(!RuleManager::Instance()->LoadRules(&database, tmp)) {
			LogInfo("Failed to load ruleset '[{0}]', falling back to defaults.", tmp);
		}
	} else {
		if(!RuleManager::Instance()->LoadRules(&database, "default")) {
			LogInfo("No rule set configured, using default rules");
		} else {
			LogInfo("Loaded default rule set 'default'", tmp);
		}
	}

	g_Clientlist = new Clientlist(Config->ChatPort);

	ChannelList = new ChatChannelList();

	database.LoadChatChannels();

	std::thread(PlayerEventQueueListener).detach();

	worldserverlist = new WorldServerList();

	// uncomment to simulate timed crash for catching SIGSEV
	//	std::thread crash_test(crash_func);
	//	crash_test.detach();

	auto loop_fn = [&](EQ::Timer* t) {
		if (keepalive.Check()) {
			keepalive.Start();
			database.ping();
		}

		Timer::SetCurrentTime();

		worldserverlist->Process();
		g_Clientlist->Process();

		if (ChannelListProcessTimer.Check()) {
			ChannelList->Process();
		}

		if (ClientConnectionPruneTimer.Check()) {
			g_Clientlist->CheckForStaleConnectionsAll();
		}
	};

	EQ::Timer process_timer(loop_fn);
	process_timer.Start(32, true);

	EQ::EventLoop::Get().Run();

	Shutdown();
}

void UpdateWindowTitle(char* iNewTitle)
{
#ifdef _WINDOWS
	std::string title;
	if (iNewTitle)
	{
		title = StringFormat("UCS: %s", iNewTitle);
	}
	else
	{
		title = "UCS";
	}
	SetConsoleTitle(title.c_str());
#endif
}
