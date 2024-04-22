#include "../../common/database/database_dump_service.h"

void WorldserverCLI::DatabaseDump(int argc, char** argv, argh::parser& cmd, std::string& description)
{
	description = "Dumps server database tables";

	if (cmd[{"-h", "--help"}]) {
		return;
	}

	std::vector<std::string> arguments = {};
	std::vector<std::string> options = {
		"--all",
		"--content-tables",
		//"--login-tables",
		"--player-tables",
		"--state-tables",
		"--system-tables",
		"--query-serv-tables",
		"--table-structure-only",
		"--table-lock",
		"--dump-path=",
		"--dump-output-to-console",
		"--compress"
	};


	if (argc < 3) {
		EQEmuCommand::ValidateCmdInput(arguments, options, cmd, argc, argv);
		return;
	}

	auto database_dump_service = new DatabaseDumpService();
	bool dump_all = cmd[{"-a", "--all"}];

	if (!cmd("--dump-path").str().empty()) {
		database_dump_service->SetDumpPath(cmd("--dump-path").str());
	}

	/**
	 * Set Option
	 */
	database_dump_service->SetDumpContentTables(cmd[{"--content-tables"}] || dump_all);
	//database_dump_service->SetDumpLoginServerTables(cmd[{"--login-tables"}] || dump_all);
	database_dump_service->SetDumpPlayerTables(cmd[{"--player-tables"}] || dump_all);
	database_dump_service->SetDumpStateTables(cmd[{"--state-tables"}] || dump_all);
	database_dump_service->SetDumpSystemTables(cmd[{"--system-tables"}] || dump_all);
	database_dump_service->SetDumpQueryServerTables(cmd[{"--query-serv-tables"}] || dump_all);
	database_dump_service->SetDumpAllTables(dump_all);

	database_dump_service->SetDumpWithNoData(cmd[{"--table-structure-only"}]);
	database_dump_service->SetDumpTableLock(cmd[{"--table-lock"}]);
	database_dump_service->SetDumpWithCompression(cmd[{"--compress"}]);
	database_dump_service->SetDumpOutputToConsole(cmd[{"--dump-output-to-console"}]);

	/**
	 * Dump
	 */
	database_dump_service->DatabaseDump();
}
