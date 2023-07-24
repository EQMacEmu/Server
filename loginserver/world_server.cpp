/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2010 EQEMu Development Team (http://eqemulator.net)

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
#include "world_server.h"
#include "login_server.h"
#include "login_structures.h"
#include "config.h"

#include "../common/eqemu_logsys.h"

extern EQEmuLogSys LogSys;
extern LoginServer server;

WorldServer::WorldServer(EmuTCPConnection *c)
{
	connection = c;
	zones_booted = 0;
	players_online = 0;
	server_status = 0;
	runtime_id = 0;
	server_list_id = 0;
	server_type = 0;
	is_server_authorized = false;
	is_server_trusted = false;
	is_server_logged_in = false;
}

WorldServer::~WorldServer()
{
	if(connection) {
		connection->Free();
	}
}

void WorldServer::Reset()
{
	zones_booted = 0;
	players_online = 0;
	server_status = 0;
	runtime_id;
	server_list_id = 0;
	server_type = 0;
	is_server_authorized = false;
	is_server_logged_in = false;
}

bool WorldServer::Process()
{
	ServerPacket *app = nullptr;
	while(app = connection->PopPacket())
	{
		LogDebug("Application packet received from server: [{0}], (size {1})", app->opcode, app->size);

		if(server.options.IsDumpInPacketsOn())
		{
			LogInfo("[Size: {0}] [{1}]", app->size, DumpServerPacketToString(app).c_str());
		}

		switch(app->opcode)
		{
		case ServerOP_NewLSInfo:
			{
				if(app->size < sizeof(ServerNewLSInfo_Struct))
				{
					LogError("Received application packet from server that had opcode ServerOP_NewLSInfo, "
						"but was too small. Discarded to avoid buffer overrun.");
					break;
				}

				LogInfo("New Login Info Recieved.");
				ServerNewLSInfo_Struct *info = (ServerNewLSInfo_Struct*)app->pBuffer;
				Handle_NewLSInfo(info);
				break;
			}
		case ServerOP_LSStatus:
			{
				if(app->size < sizeof(ServerLSStatus_Struct))
				{
					LogError("Recieved application packet from server that had opcode ServerOP_LSStatus, "
						"but was too small. Discarded to avoid buffer overrun.");
					break;
				}

				LogDebug("World Server Status Recieved.");

				auto *ls_status = (ServerLSStatus_Struct*)app->pBuffer;

				LogDebug("World Server Status Update | Server [{0}] Status [{1}] Players [{2}] Zones [{3}]",
					GetLongName(), ls_status->status, ls_status->num_players, ls_status->num_zones);

				Handle_LSStatus(ls_status);
				break;
			}
		case ServerOP_LSZoneInfo:
		case ServerOP_LSZoneShutdown:
		case ServerOP_LSZoneStart:
		case ServerOP_LSZoneBoot:
		case ServerOP_LSZoneSleep:
		case ServerOP_LSPlayerLeftWorld:
		case ServerOP_LSPlayerJoinWorld:
		case ServerOP_LSPlayerZoneChange:
			{
				//Not logging these to cut down on spam until we implement them
				break;
			}

		case ServerOP_UsertoWorldResp:
			{
				if(app->size < sizeof(UsertoWorldResponse_Struct))
				{
					LogError("Recieved application packet from server that had opcode ServerOP_UsertoWorldResp, "
						"but was too small. Discarded to avoid buffer overrun.");
					break;
				}

				LogInfo("User-To-World Response received.");

				UsertoWorldResponse_Struct *utwr = (UsertoWorldResponse_Struct*)app->pBuffer;
				LogInfo("Trying to find client with user id of [{0}].", utwr->lsaccountid);
				Client *c = server.client_manager->GetClient(utwr->lsaccountid);
				if (c && c->GetClientVersion() == cv_old)
				{
					if (utwr->response > 0)
					{
						SendClientAuth(c->GetConnection()->GetRemoteIP(), c->GetAccountName(), c->GetKey(), c->GetAccountID(), c->GetMacClientVersion());
					}

					switch (utwr->response)
					{
					case 1:
						break;
					case 0:
						c->FatalError("\nError 1020: Your chosen World Server is DOWN.\n\nPlease select another.");
						break;
					case -1:
						c->FatalError("You have been suspended from the worldserver.");
						break;
					case -2:
						c->FatalError("You have been banned from the worldserver.");
						break;
					case -3:
						c->FatalError("That server is full.");
						break;
					case -4:
						c->FatalError("Error 1018: You currently have an active character on that EverQuest Server, please allow a minute for synchronization and try again.");
						break;
					case -5:
						c->FatalError("Error IP Limit Exceeded: \n\nYou have exceeded the maximum number of allowed IP addresses for this account.");
						break;
					}
					LogInfo("Found client with user id of {0} and account name of {1}.", utwr->lsaccountid, c->GetAccountName().c_str());
					EQApplicationPacket* outapp = new EQApplicationPacket(OP_PlayEverquestRequest, 17);
					strncpy((char*)&outapp->pBuffer[1], c->GetKey().c_str(), c->GetKey().size());

					c->SendPlayResponse(outapp);
					delete outapp;
				}
				else if (c)
				{
					LogInfo("Found client with user id of [{0}] and account name of [{1}].", utwr->lsaccountid, c->GetAccountName().c_str());
					auto outapp = new EQApplicationPacket(OP_PlayEverquestResponse, sizeof(PlayEverquestResponse_Struct));
					PlayEverquestResponse_Struct *per = (PlayEverquestResponse_Struct*)outapp->pBuffer;
					per->Sequence = c->GetPlaySequence();
					per->ServerNumber = c->GetPlayServerID();
					LogInfo("Found sequence and play of [{0}] [{1}]", c->GetPlaySequence(), c->GetPlayServerID());
					LogDebug("[Size: {0}] [{1}]", outapp->size, DumpPacketToString(outapp).c_str());

					if(utwr->response > 0)
					{
						per->Allowed = 1;
						SendClientAuth(c->GetConnection()->GetRemoteIP(), c->GetAccountName(), c->GetKey(), c->GetAccountID());
					}

					switch(utwr->response)
					{
					case 1:
						per->Message = 101;
						break;
					case 0:
						per->Message = 326;
						break;
					case -1:
						per->Message = 337;
						break;
					case -2:
						per->Message = 338;
						break;
					case -3:
						per->Message = 303;
						break;
					case -4:
						per->Message = 111;
						break;
					case -5:
						per->Message = 198;
						break;
					}

					LogInfo("Sending play response with following data, allowed {} , sequence {} , server number {} , message {} ",
							per->Allowed, per->Sequence, per->ServerNumber, per->Message);

					if(server.options.IsDumpOutPacketsOn())
					{
						LogDebug("[Size: {0}] [{1}]", outapp->size, DumpPacketToString(outapp).c_str());
					}

					c->SendPlayResponse(outapp);
					delete outapp;
				}
				else
				{
					LogError("Received User-To-World Response for [{0}] but could not find the client referenced!.", utwr->lsaccountid);
				}
				break;
			}
		case ServerOP_LSAccountUpdate:
			{
				if(app->size < sizeof(ServerLSAccountUpdate_Struct))
				{
					LogError("Recieved application packet from server that had opcode ServerLSAccountUpdate_Struct, "
						"but was too small. Discarded to avoid buffer overrun.");
					break;
				}
			
				LogInfo("ServerOP_LSAccountUpdate packet received from: {0}", short_name.c_str());
				ServerLSAccountUpdate_Struct *lsau = (ServerLSAccountUpdate_Struct*)app->pBuffer;
				if(is_server_trusted)
				{
					LogInfo("ServerOP_LSAccountUpdate update processed for: {0}", lsau->useraccount);
					string name;
					string password;
					string email;
					name.assign(lsau->useraccount);
					password.assign(lsau->userpassword);
					email.assign(lsau->useremail);
					server.db->UpdateLSAccountInfo(lsau->useraccountid, name, password, email);
				}
				break;
			}
		default:
			{
				LogError("Recieved application packet from server that had an unknown operation code 0x%.4X.", app->opcode);
			}
		}

		delete app;
		app = nullptr;
	}
	return true;
}

void WorldServer::Handle_NewLSInfo(ServerNewLSInfo_Struct* i)
{
	if(is_server_logged_in)
	{
		LogError("WorldServer::Handle_NewLSInfo called but the login server was already marked as logged in, aborting.");
		return;
	}

	if(strlen(i->account) <= 30)
	{
		account_name = i->account;
	}
	else
	{
		LogError("Handle_NewLSInfo error, account name was too long.");
		return;
	}

	if(strlen(i->password) <= 30)
	{
		account_password = i->password;
	}
	else
	{
		LogError("Handle_NewLSInfo error, account password was too long.");
		return;
	}

	if(strlen(i->name) <= 200)
	{
		long_name = i->name;
	}
	else
	{
		LogError("Handle_NewLSInfo error, long name was too long.");
		return;
	}

	if(strlen(i->shortname) <= 50)
	{
		short_name = i->shortname;
	}
	else
	{
		LogError("Handle_NewLSInfo error, short name was too long.");
		return;
	}

	if(strlen(i->local_address) <= 125)
	{
		if(strlen(i->local_address) == 0)
		{
			LogError("Handle_NewLSInfo error, local address was null, defaulting to localhost");
			local_ip = "127.0.0.1";
		}
		else
		{
			local_ip = i->local_address;
		}
	}
	else
	{
		LogError("Handle_NewLSInfo error, local address was too long.");
		return;
	}

	if(strlen(i->remote_address) <= 125)
	{
		if(strlen(i->remote_address) == 0)
		{
			in_addr in;
			in.s_addr = GetConnection()->GetrIP();
			remote_ip = inet_ntoa(in);
			LogError("Handle_NewLSInfo error, remote address was null, defaulting to stream address %s.", remote_ip.c_str());
		}
		else
		{
			remote_ip = i->remote_address;
		}
	}
	else
	{
		in_addr in;
		in.s_addr = GetConnection()->GetrIP();
		remote_ip = inet_ntoa(in);
		LogError("Handle_NewLSInfo error, remote address was too long, defaulting to stream address %s.", remote_ip.c_str());
	}

	if(strlen(i->serverversion) <= 64)
	{
		version = i->serverversion;
	}
	else
	{
		LogError("Handle_NewLSInfo error, server version was too long.");
		return;
	}

	if(strlen(i->protocolversion) <= 25)
	{
		protocol = i->protocolversion;
	}
	else
	{
		LogError("Handle_NewLSInfo error, protocol version was too long.");
		return;
	}

	server_type = i->servertype;
	is_server_logged_in = true;

	if(server.options.IsRejectingDuplicateServers())
	{
		if(server.server_manager->ServerExists(long_name, short_name, this))
		{
			LogError("World tried to login but there already exists a server that has that name.");
			return;
		}
	}
	else
	{
		if(server.server_manager->ServerExists(long_name, short_name, this))
		{
			LogError("World tried to login but there already exists a server that has that name.");
			server.server_manager->DestroyServerByName(long_name, short_name, this);
		}
	}

	if(!server.options.IsUnregisteredAllowed())
	{
		if(account_name.size() > 0 && account_password.size() > 0)
		{
			unsigned int s_id = 0;
			unsigned int s_list_type = 0;
			unsigned int s_trusted = 0;
			string s_desc;
			string s_list_desc;
			string s_acct_name;
			string s_acct_pass;
			if(server.db->GetWorldRegistration(long_name, short_name, s_id, s_desc, s_list_type, s_trusted, s_list_desc, s_acct_name, s_acct_pass))
			{
				if(s_acct_name.size() == 0 || s_acct_pass.size() == 0)
				{
					LogInfo("Server [{0}]([{1}]) successfully logged into account that had no user/password requirement.",
						long_name.c_str(), short_name.c_str());
					is_server_authorized = true;
					SetRuntimeID(s_id);
					server_list_id = s_list_type;
					desc = s_desc;
				}
				else if(s_acct_name.compare(account_name) == 0 && s_acct_pass.compare(account_password) == 0)
				{
					LogInfo("Server [{0}]({1}) successfully logged in.",
						long_name.c_str(), short_name.c_str());
					is_server_authorized = true;
					SetRuntimeID(s_id);
					server_list_id = s_list_type;
					desc = s_desc;
					if(s_trusted) {
						LogInfo("ServerOP_LSAccountUpdate sent to world");
						is_server_trusted = true;
						auto outapp = new ServerPacket(ServerOP_LSAccountUpdate, 0);
						connection->SendPacket(outapp);
						safe_delete(outapp);
					}
				}
				else {
					LogInfo("Server [{0}]([{1}]) attempted to log in but account and password did not match the entry in the database, and only"
						" registered servers are allowed.", long_name.c_str(), short_name.c_str());
					return;
				}
			}
			else {
				LogInfo("Server [{0}]([{1}]) attempted to log in but database couldn't find an entry and only registered servers are allowed.",
					long_name.c_str(), short_name.c_str());
				return;
			}
		}
		else {
			LogInfo("Server [{0}]([{1}]) did not attempt to log in but only registered servers are allowed.",
				long_name.c_str(), short_name.c_str());
			return;
		}
	}
	else {
		unsigned int server_id = 0;
		unsigned int server_list_type = 0;
		unsigned int is_server_trusted = 0;
		string server_description;
		string server_list_description;
		string server_account_name;
		string server_account_password;


		if(server.db->GetWorldRegistration(
			long_name, 
			short_name, 
			server_id, 
			server_description, 
			server_list_type, 
			is_server_trusted, 
			server_list_description, 
			server_account_name, 
			server_account_password)) 
		{
			
			if(account_name.size() > 0 && account_password.size() > 0) {
				if(server_account_name.compare(account_name) == 0 && server_account_password.compare(account_password) == 0) {
					LogInfo("Server [{0}]([{1}]) successfully logged in.", long_name.c_str(), short_name.c_str());
					is_server_authorized = true;
					SetRuntimeID(server_id);
					server_list_id = server_list_type;
					desc = server_description;

					if(is_server_trusted) {
						LogInfo("ServerOP_LSAccountUpdate sent to world");
						is_server_trusted = true;
						auto outapp = new ServerPacket(ServerOP_LSAccountUpdate, 0);
						connection->SendPacket(outapp);
						safe_delete(outapp);
					}
				}
				else {
					// this is the first of two cases where we should deny access even if unregistered is allowed
					LogInfo("Server [{0}]([{1}]) attempted to log in but account and password did not match the entry in the database.",
						long_name.c_str(), short_name.c_str());
				}
			}
			else {
				if(server_account_name.size() > 0 || server_account_password.size() > 0) {
					// this is the second of two cases where we should deny access even if unregistered is allowed
					LogInfo("Server  [{0}]([{1}]) did not attempt to log in but this server requires a password.",
						long_name.c_str(), short_name.c_str());
				}
				else {
					LogInfo("Server  [{0}]([{1}]) did not attempt to log in but unregistered servers are allowed.",
						long_name.c_str(), short_name.c_str());
					is_server_authorized = true;
					SetRuntimeID(server_id);
					server_list_id = 0;
				}
			}
		}
		else
		{
			LogInfo("Server [{0}]([{1}]) attempted to log in but database couldn't find an entry but unregistered servers are allowed.",
				long_name.c_str(), short_name.c_str());
			if(server.db->CreateWorldRegistration(long_name, short_name, server_id)) {
				is_server_authorized = true;
				SetRuntimeID(server_id);
				server_list_id = 0;
			}
		}
	}

	in_addr in;
	in.s_addr = connection->GetrIP();
	server.db->UpdateWorldRegistration(GetRuntimeID(), long_name, string(inet_ntoa(in)));

	if (is_server_authorized)
	{
		server.client_manager->UpdateServerList();
	}
}

void WorldServer::Handle_LSStatus(ServerLSStatus_Struct *s)
{
	players_online = s->num_players;
	zones_booted = s->num_zones;
	server_status = s->status;
}

void WorldServer::SendClientAuth(unsigned int ip, string account, string key, unsigned int account_id, uint8 version)
{
	auto outapp = new ServerPacket(ServerOP_LSClientAuth, sizeof(ServerLSClientAuth));
	ServerLSClientAuth* client_auth = (ServerLSClientAuth*)outapp->pBuffer;

	client_auth->lsaccount_id = account_id;
	strncpy(client_auth->name, account.c_str(), account.size() > 30 ? 30 : account.size());
	strncpy(client_auth->key, key.c_str(), 10);
	client_auth->lsadmin = 0;
	client_auth->worldadmin = 0;
	client_auth->ip = ip;
	client_auth->version = version;

	in_addr in{};
	in.s_addr = ip; connection->GetrIP();
	string client_address(inet_ntoa(in));
	in.s_addr = connection->GetrIP();
	string world_address(inet_ntoa(in));

	if (client_address.compare(world_address) == 0) {
		client_auth->local = 1;
	}
	else if (client_address.find(server.options.GetLocalNetwork()) != string::npos) {
		client_auth->local = 1;
	}
	else {
		client_auth->local = 0;
	}

	connection->SendPacket(outapp);

	if (server.options.IsDumpInPacketsOn())
	{
		Log(Logs::General, Logs::LoginServer, "[Size: %u] %s", outapp->size, DumpServerPacketToString(outapp).c_str());
	}
	safe_delete(outapp);
}

