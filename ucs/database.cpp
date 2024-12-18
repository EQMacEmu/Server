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


#include "../common/global_define.h"
#include "../common/eqemu_logsys.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysqld_error.h>
#include <limits.h>
#include <ctype.h>
#include <assert.h>
#include <map>

// Disgrace: for windows compile
#ifdef _WINDOWS
#include <windows.h>
#define snprintf	_snprintf
#define strncasecmp	_strnicmp
#define strcasecmp	_stricmp
#else
#include "../common/unix.h"
#include <netinet/in.h>
#endif

#include "database.h"
#include "../common/eq_packet_structs.h"
#include "../common/misc_functions.h"
#include "../common/strings.h"
#include "chatchannel.h"


extern Clientlist *g_Clientlist;
extern ChatChannelList *ChannelList;


void UCSDatabase::GetAccountStatus(Client *client) {

	std::string query = StringFormat(
		"SELECT `status`, `hideme`, `karma`, `revoked` FROM `account` WHERE `id` = %i LIMIT 1",
		client->GetAccountID()
	);

    auto results = QueryDatabase(query);
    if (!results.Success()) {
		LogMySQLError(
			"Unable to get account status for character [{0}], error [{1}]", 
			client->GetName().c_str(), 
			results.ErrorMessage().c_str()
		);

		return;
	}

	LogInfo("GetAccountStatus Query: [{0}]", query.c_str());

	if(results.RowCount() != 1)
	{
		LogMySQLError("Error in GetAccountStatus");
		return;
	}

	auto row = results.begin();

	client->SetAccountStatus(atoi(row[0]));
	client->SetHideMe(atoi(row[1]) != 0);
	client->SetKarma(atoi(row[2]));
	client->SetRevoked((atoi(row[3])==1?true:false));

	LogDebug("Set account status to [{0}], hideme to [{1}] and karma to [{2}] for [{3}]", client->GetAccountStatus(), client->GetHideMe(), client->GetKarma(), client->GetName().c_str());

}

int UCSDatabase::FindAccount(const char *characterName, Client *client) {

	LogInfo("FindAccount for character [{0}]", characterName);


	client->ClearCharacters();
    std::string query = StringFormat("SELECT `id`, `account_id`, `level`, `race`, `class` "
                                    "FROM `character_data` WHERE `name` = '%s' AND `is_deleted` = 0 LIMIT 1",
                                    characterName);
    auto results = QueryDatabase(query);
	if (!results.Success()) {
		LogMySQLError("FindAccount query failed: [{0}]", query.c_str());
		return -1;
	}

	if (results.RowCount() != 1) {
		LogMySQLError("Bad result from query");
		return -1;
	}

	auto row = results.begin();
	client->AddCharacter(atoi(row[0]), characterName, atoi(row[2]), atoi(row[3]), atoi(row[4]));

	int accountID = atoi(row[1]);

	LogMySQLQuery("Account ID for [{0}] is [{1}]", characterName, accountID);

    query = StringFormat("SELECT `id`, `name`, `level`, `race`, `class` FROM `character_data` "
                        "WHERE `account_id` = %i AND `name` != '%s' AND `is_deleted` = 0",
						accountID, characterName);
    results = QueryDatabase(query);
	if (!results.Success())
		return accountID;

	for (auto row = results.begin(); row != results.end(); ++row)
		client->AddCharacter(atoi(row[0]), row[1], atoi(row[2]), atoi(row[3]), atoi(row[4]));

	return accountID;
}

bool UCSDatabase::VerifyMailKey(const std::string& characterName, int IPAddress, const std::string& MailKey)
{

	std::string query = StringFormat(
		"SELECT `mailkey` FROM `character_data` WHERE `name`='%s' LIMIT 1",
		characterName.c_str()
	);

	auto results = QueryDatabase(query);
	if (!results.Success()) {
		LogInfo("Error retrieving mailkey from database: [{}]", results.ErrorMessage().c_str());
		return false;
	}

	if (results.RowCount() == 0) {
		LogInfo("No mailkeys found for [{}].", characterName.c_str());
		return false;
	}

	auto row = results.begin();

	// The key is the client's IP address (expressed as 8 hex digits) and an 8 hex digit random string generated
	// by world.
	//
	char combinedKey[17];

	if (RuleB(Chat, EnableMailKeyIPVerification) == true) {
		sprintf(combinedKey, "%08X%s", IPAddress, MailKey.c_str());
	}
	else {
		sprintf(combinedKey, "%s", MailKey.c_str());
	}

	LogInfo("DB key is [[{}]], Client key is [[{}]]", (row[0] ? row[0] : ""), combinedKey);

	return !strcmp(row[0], combinedKey);
}

int UCSDatabase::FindCharacter(const char *characterName)
{
	char *safeCharName = RemoveApostrophes(characterName);
	std::string query = StringFormat("SELECT `id` FROM `character_data` WHERE `name`='%s' LIMIT 1", safeCharName);
	auto results = QueryDatabase(query);
	if (!results.Success()) {
		safe_delete_array(safeCharName);
		return -1;
	}

	safe_delete_array(safeCharName);

	if (results.RowCount() != 1) {
		LogMySQLError("Bad result from FindCharacter query for character [{0}]",
			characterName);
		return -1;
	}

	auto row = results.begin();

	int characterID = atoi(row[0]);

	return characterID;
}

bool UCSDatabase::GetVariable(const char* varname, char* varvalue, uint16 varvalue_len) {

	std::string query = StringFormat("SELECT `value` FROM `variables` WHERE `varname` = '%s'", varname);
    auto results = QueryDatabase(query);
	if (!results.Success()) {
		return false;
	}

	if (results.RowCount() != 1)
		return false;

	auto row = results.begin();

	snprintf(varvalue, varvalue_len, "%s", row[0]);

	return true;
}

bool UCSDatabase::LoadChatChannels() 
{
	LogInfo("Loading chat channels from the database.");

	const std::string query = "SELECT `name`, `owner`, `password`, `minstatus` FROM `chatchannels`";
    auto results = QueryDatabase(query);
	if (!results.Success()) {
		return false;
	}

	for (auto row = results.begin();row != results.end(); ++row) {
		std::string channel_name = row[0];
		std::string channel_owner = row[1];
		std::string channel_password = row[2];
		auto channel_min_status = row[3];

		if (!ChannelList->FindChannel(channel_name)) {
			ChannelList->CreateChannel(channel_name, channel_owner, channel_password, true, atoi(channel_min_status));
		}
	}

	return true;
}

void UCSDatabase::SetChannelPassword(const std::string& channel_name, const std::string& password)
{
	LogInfo("UCSDatabase::SetChannelPassword([{}], [{}])", channel_name.c_str(), password.c_str());

	std::string query = fmt::format(
		"UPDATE `chatchannels` SET `password` = '{}' WHERE `name` = '{}'",
		Strings::Escape(password), Strings::Escape(channel_name));
	QueryDatabase(query);
}

void UCSDatabase::SetChannelOwner(const std::string& channel_name, const std::string& owner)
{
	LogInfo("Setting channel [{}] owner to [{}]", channel_name, owner);

	std::string query = fmt::format(
		"UPDATE `chatchannels` SET `owner` = '{}' WHERE `name` = '{}'",
		Strings::Escape(owner),
		Strings::Escape(channel_name)
	);

    QueryDatabase(query);
}

void UCSDatabase::SetMessageStatus(const int& messageNumber, const int& status) {

	LogInfo("SetMessageStatus [{0}] [{1}]", messageNumber, status);

	if(status == 0) {
        std::string query = StringFormat("DELETE FROM `mail` WHERE `msgid` = %i", messageNumber);
		auto results = QueryDatabase(query);
        return;
    }

    std::string query = StringFormat("UPDATE `mail` SET `status` = %i WHERE `msgid`=%i", status, messageNumber);
    QueryDatabase(query);
}

void UCSDatabase::AddFriendOrIgnore(const int& charID, const int& type, const std::string& name) {

    std::string query = StringFormat("INSERT INTO `friends` (`charid`, `type`, `name`) "
                                    "VALUES('%i', %i, '%s')",
                                    charID, type, CapitaliseName(name).c_str());
    auto results = QueryDatabase(query);
	if(results.Success())
		LogMySQLQuery("Wrote Friend/Ignore entry for charid [{0}], type [{1}], name [{2}] to database.", charID, type, name.c_str());

}

void UCSDatabase::RemoveFriendOrIgnore(const int& charID, const int& type, const std::string& name) {

	std::string query = StringFormat("DELETE FROM `friends` WHERE `charid` = %i "
                                    "AND `type` = %i AND `name` = '%s'",
                                    charID, type, CapitaliseName(name).c_str());
    auto results = QueryDatabase(query);
	if (!results.Success()) {
		LogMySQLError("Error removing friend/ignore, query was [{0}]", query.c_str());
	}
	else {
		LogMySQLQuery("Removed Friend/Ignore entry for charid [{0}], type [{1}], name [{2}] from database.", charID, type, name.c_str());
	}
}

void UCSDatabase::GetFriendsAndIgnore(const int& charID, std::vector<std::string>& friends, std::vector<std::string>& ignorees) {

	std::string query = StringFormat("select `type`, `name` FROM `friends` WHERE `charid`=%i", charID);
    auto results = QueryDatabase(query);
	if (!results.Success()) {
		return;
	}


	for (auto row = results.begin(); row != results.end(); ++row) {
		std::string name = row[1];

		if(atoi(row[0]) == 0)
		{
			ignorees.push_back(name);
			LogMySQLQuery("Added Ignoree from DB [{0}]", name.c_str());
			continue;
		}

        friends.push_back(name);
        LogMySQLQuery("Added Friend from DB [{0}]", name.c_str());
	}

}
