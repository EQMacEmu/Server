#include "../common/global_define.h"
#include "../common/eqemu_logsys.h"
#include <iostream>
#include <map>
#include <vector>

#include "database.h"
#include "../common/strings.h"

// this function does not delete the ServerPacket, so it must be handled at call site
void QSDatabase::GeneralQueryReceive(ServerPacket *pack)
{
	/*
	 * These are general queries passed from anywhere in zone instead of packing structures and breaking them down again and again
	 */
	auto queryBuffer = new char[pack->ReadUInt32() + 1];
	pack->ReadString(queryBuffer);

	std::string query(queryBuffer);
	auto results = QueryDatabase(query);
	if (!results.Success())
	{
		Log(Logs::Detail, Logs::QSServer, "Failed Delete Log Record Insert: %s\n%s", results.ErrorMessage().c_str(), query.c_str());
	}
	safe_delete_array(queryBuffer);
}
