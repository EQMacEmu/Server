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
#include "../common/global_define.h"
#include "../common/misc_functions.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include "../common/version.h"

#include "../common/servertalk.h"
#include "login_server.h"
#include "login_server_list.h"
#include "../common/eq_packet_structs.h"
#include "../common/packet_dump.h"
#include "../common/strings.h"
#include "zoneserver.h"
#include "worlddb.h"
#include "zonelist.h"
#include "clientlist.h"
#include "world_config.h"

extern ZSList zoneserver_list;
extern ClientList client_list;
extern uint32 numzones;
extern uint32 numplayers;
extern volatile bool	RunLoops;

LoginServer::LoginServer(const char* iAddress, uint16 iPort, const char* Account, const char* Password, uint8 Type)
{
	strn0cpy(LoginServerAddress, iAddress, 256);
	LoginServerPort = iPort;
	strn0cpy(LoginAccount, Account, 16);
	strn0cpy(LoginPassword, Password, 16);
	LoginIsLegacy = Type == 1;
	CanAccountUpdate = false;
	Connect();
}

LoginServer::~LoginServer() {

}

void LoginServer::ProcessUsertoWorldReq(uint16_t opcode, EQ::Net::Packet& p)
{
	const WorldConfig* Config = WorldConfig::get();
	LogNetcode("Received ServerPacket from LS OpCode {:#04x}", opcode);

	UsertoWorldRequest_Struct* utwr = (UsertoWorldRequest_Struct*)p.Data();
	uint32 id = database.GetAccountIDFromLSID(utwr->lsaccountid);
	int16 status = database.CheckStatus(id);
	bool mule = false;
	uint16 expansion = 0;
	database.GetAccountRestriction(id, expansion, mule);

	auto outpack = new ServerPacket;
	outpack->opcode = ServerOP_UsertoWorldResp;
	outpack->size = sizeof(UsertoWorldResponse_Struct);
	outpack->pBuffer = new uchar[outpack->size];
	memset(outpack->pBuffer, 0, outpack->size);
	UsertoWorldResponse_Struct* utwrs = (UsertoWorldResponse_Struct*)outpack->pBuffer;
	utwrs->lsaccountid = utwr->lsaccountid;
	utwrs->ToID = utwr->FromID;

	if (Config->Locked == true)
	{
		if (status < 80 && status > -1)
			utwrs->response = 0;
		if (status >= 80)
			utwrs->response = 1;
	}
	else {
		utwrs->response = 1;
	}

	int32 x = Config->MaxClients;
	if ((int32)numplayers >= x && x != -1 && x != 255 && status < 80)
		utwrs->response = -3;

	if (status == -1)
		utwrs->response = -1;
	if (status == -2)
		utwrs->response = -2;

	if (utwrs->response == 1)
	{
		// active account checks
		if (RuleI(World, AccountSessionLimit) >= 0 && status < (RuleI(World, ExemptAccountLimitStatus)) && (RuleI(World, ExemptAccountLimitStatus) != -1) && client_list.CheckAccountActive(id))
			utwrs->response = -4;
	}
	if (utwrs->response == 1)
	{
		// ip limit checks
		if (!mule && RuleI(World, MaxClientsPerIP) >= 0 && !client_list.CheckIPLimit(id, utwr->ip, status))
			utwrs->response = -5;
	}

	utwrs->worldid = utwr->worldid;
	SendPacket(outpack);
	delete outpack;
}

void LoginServer::ProcessLSClientAuth(uint16_t opcode, EQ::Net::Packet& p) {
	const WorldConfig* Config = WorldConfig::get();
	LogNetcode("Received ServerPacket from LS OpCode {:#04x}", opcode);

	try {
		auto slsca = p.GetSerialize<ClientAuth_Struct>(0);

		client_list.CLEAdd(slsca.loginserver_account_id, slsca.account_name, slsca.key, slsca.is_world_admin, slsca.ip, slsca.is_client_from_local_network, slsca.version);
	}
	catch (std::exception& ex) {
		LogError("Error parsing LSClientAuth packet from world.\n{0}", ex.what());
	}
}

void LoginServer::ProcessLSFatalError(uint16_t opcode, EQ::Net::Packet& p) {
	const WorldConfig* Config = WorldConfig::get();
	LogNetcode("Received ServerPacket from LS OpCode {:#04x}", opcode);

	if (p.Length() > 1) {
		LogInfo("     {}", (const char*)p.Data());
	}
	database.LSDisconnect();
}

void LoginServer::ProcessSystemwideMessage(uint16_t opcode, EQ::Net::Packet& p) {
	const WorldConfig* Config = WorldConfig::get();
	LogNetcode("Received ServerPacket from LS OpCode {:#04x}", opcode);

	ServerSystemwideMessage* swm = (ServerSystemwideMessage*)p.Data();
	zoneserver_list.SendEmoteMessageRaw(0, 0, AccountStatus::Player, swm->type, swm->message);
}

void LoginServer::ProcessLSRemoteAddr(uint16_t opcode, EQ::Net::Packet& p) {
	const WorldConfig* Config = WorldConfig::get();
	LogNetcode("Received ServerPacket from LS OpCode {:#04x}", opcode);

	if (!Config->WorldAddress.length()) {
		WorldConfig::SetWorldAddress((char*)p.Data());
		LogInfo("Loginserver provided {} as world address", (const char*)p.Data());
	}
}

void LoginServer::ProcessLSAccountUpdate(uint16_t opcode, EQ::Net::Packet& p) {
	const WorldConfig* Config = WorldConfig::get();
	LogNetcode("Received ServerPacket from LS OpCode {:#04x}", opcode);

	LogInfo("Received ServerOP_LSAccountUpdate packet from loginserver");
	CanAccountUpdate = true;
}

bool LoginServer::Connect() {

	char errbuf[1024];
	if ((LoginServerIP = ResolveIP(LoginServerAddress, errbuf)) == 0) {
		LogInfo("Unable to resolve [{}] to an IP", LoginServerAddress);
		database.LSDisconnect();
		return false;
	}

	if (LoginServerIP == 0 || LoginServerPort == 0) {
		LogInfo("Connect info incomplete, cannot connect: [{0}:{1}]", LoginServerAddress, LoginServerPort);
		database.LSDisconnect();
		return false;
	}

	if (LoginIsLegacy) {
		legacy_client.reset(new EQ::Net::ServertalkLegacyClient(LoginServerAddress, LoginServerPort, false));

		legacy_client->OnConnect(
			[this](EQ::Net::ServertalkLegacyClient* client) {
			if (client) {
				LogInfo(
					"Connected to Legacy Loginserver: [{0}:{1}]",
					LoginServerAddress,
					LoginServerPort
				);

				SendNewInfo();
				SendStatus();
				zoneserver_list.SendLSZones();
				statusupdate_timer.reset(new EQ::Timer(LoginServer_StatusUpdateInterval, true, [this](EQ::Timer* t) {
					SendStatus();
				}
				));
			}
			else {
				LogInfo(
					"Could not connect to Legacy Loginserver: [{0}:{1}]",
					LoginServerAddress,
					LoginServerPort
				);
			}
		}
		);

		legacy_client->OnMessage(ServerOP_UsertoWorldReq, std::bind(&LoginServer::ProcessUsertoWorldReq, this, std::placeholders::_1, std::placeholders::_2));
		legacy_client->OnMessage(ServerOP_LSClientAuth, std::bind(&LoginServer::ProcessLSClientAuth, this, std::placeholders::_1, std::placeholders::_2));
		legacy_client->OnMessage(ServerOP_LSFatalError, std::bind(&LoginServer::ProcessLSFatalError, this, std::placeholders::_1, std::placeholders::_2));
		legacy_client->OnMessage(ServerOP_SystemwideMessage, std::bind(&LoginServer::ProcessSystemwideMessage, this, std::placeholders::_1, std::placeholders::_2));
		legacy_client->OnMessage(ServerOP_LSRemoteAddr, std::bind(&LoginServer::ProcessLSRemoteAddr, this, std::placeholders::_1, std::placeholders::_2));
		legacy_client->OnMessage(ServerOP_LSAccountUpdate, std::bind(&LoginServer::ProcessLSAccountUpdate, this, std::placeholders::_1, std::placeholders::_2));
	}
	else {
		client.reset(new EQ::Net::ServertalkClient(LoginServerAddress, LoginServerPort, false, "World", ""));
		client->OnConnect([this](EQ::Net::ServertalkClient* client) {
			if (client) {
				LogInfo("Connected to Loginserver: {}:{}", LoginServerAddress, LoginServerPort);
				SendNewInfo();
				SendStatus();
				zoneserver_list.SendLSZones();
				statusupdate_timer.reset(new EQ::Timer(LoginServer_StatusUpdateInterval, true, [this](EQ::Timer* t) {
					SendStatus();
				}
				));
			}
			else {
				LogInfo("Could not connect to Loginserver: {}:{}", LoginServerAddress, LoginServerPort);
			}
		});

		client->OnMessage(ServerOP_UsertoWorldReq, std::bind(&LoginServer::ProcessUsertoWorldReq, this, std::placeholders::_1, std::placeholders::_2));
		client->OnMessage(ServerOP_LSClientAuth, std::bind(&LoginServer::ProcessLSClientAuth, this, std::placeholders::_1, std::placeholders::_2));
		client->OnMessage(ServerOP_LSFatalError, std::bind(&LoginServer::ProcessLSFatalError, this, std::placeholders::_1, std::placeholders::_2));
		client->OnMessage(ServerOP_SystemwideMessage, std::bind(&LoginServer::ProcessSystemwideMessage, this, std::placeholders::_1, std::placeholders::_2));
		client->OnMessage(ServerOP_LSRemoteAddr, std::bind(&LoginServer::ProcessLSRemoteAddr, this, std::placeholders::_1, std::placeholders::_2));
		client->OnMessage(ServerOP_LSAccountUpdate, std::bind(&LoginServer::ProcessLSAccountUpdate, this, std::placeholders::_1, std::placeholders::_2));
	}
	return true;
}

void LoginServer::SendInfo() {
	const WorldConfig* Config = WorldConfig::get();

	auto pack = new ServerPacket;
	pack->opcode = ServerOP_LSInfo;
	pack->size = sizeof(ServerLSInfo_Struct);
	pack->pBuffer = new uchar[pack->size];
	memset(pack->pBuffer, 0, pack->size);
	ServerLSInfo_Struct* lsi = (ServerLSInfo_Struct*)pack->pBuffer;
	strcpy(lsi->protocolversion, EQEMU_PROTOCOL_VERSION);
	strcpy(lsi->serverversion, LOGIN_VERSION);
	strcpy(lsi->name, Config->LongName.c_str());
	strcpy(lsi->account, LoginAccount);
	strcpy(lsi->password, LoginPassword);
	strcpy(lsi->address, Config->WorldAddress.c_str());
	SendPacket(pack);
	delete pack;
}

void LoginServer::SendNewInfo() {
	uint16 port;
	const WorldConfig* Config = WorldConfig::get();

	auto pack = new ServerPacket;
	pack->opcode = ServerOP_NewLSInfo;
	pack->size = sizeof(ServerNewLSInfo_Struct);
	pack->pBuffer = new uchar[pack->size];
	memset(pack->pBuffer, 0, pack->size);
	ServerNewLSInfo_Struct* lsi = (ServerNewLSInfo_Struct*)pack->pBuffer;
	strcpy(lsi->protocolversion, EQEMU_PROTOCOL_VERSION);
	strcpy(lsi->serverversion, LOGIN_VERSION);
	strcpy(lsi->name, Config->LongName.c_str());
	strcpy(lsi->shortname, Config->ShortName.c_str());
	strcpy(lsi->account, LoginAccount);
	strcpy(lsi->password, LoginPassword);
	if (Config->WorldAddress.length())
		strcpy(lsi->remote_address, Config->WorldAddress.c_str());
	if (Config->LocalAddress.length())
		strcpy(lsi->local_address, Config->LocalAddress.c_str());
	else {
		if (legacy_client) {
			WorldConfig::SetLocalAddress(legacy_client->Handle()->LocalIP());
		}
		else if (client) {
			WorldConfig::SetLocalAddress(client->Handle()->LocalIP());
		}
	}
	SendPacket(pack);
	delete pack;
}

void LoginServer::SendStatus() {
	auto pack = new ServerPacket;
	pack->opcode = ServerOP_LSStatus;
	pack->size = sizeof(ServerLSStatus_Struct);
	pack->pBuffer = new uchar[pack->size];
	memset(pack->pBuffer, 0, pack->size);
	ServerLSStatus_Struct* lss = (ServerLSStatus_Struct*)pack->pBuffer;

	if (WorldConfig::get()->Locked)
		lss->status = -2;
	else if (numzones <= 0)
		lss->status = -1;
	else
		lss->status = numplayers > 0 ? numplayers : 0;

	lss->num_zones = numzones;
	lss->num_players = numplayers;
	SendPacket(pack);
	delete pack;
}

void LoginServer::SendPacket(ServerPacket* pack)
{
	if (legacy_client) {
		legacy_client->SendPacket(pack);
	}
	else if (client) {
		client->SendPacket(pack);
	}
}

void LoginServer::SendAccountUpdate(ServerPacket* pack) {
	ServerLSAccountUpdate_Struct* s = (ServerLSAccountUpdate_Struct*)pack->pBuffer;
	if (CanUpdate()) {
		Log(Logs::Detail, Logs::WorldServer, "Sending ServerOP_LSAccountUpdate packet to loginserver: %s:%d", LoginServerAddress, LoginServerPort);
		strn0cpy(s->worldaccount, LoginAccount, 30);
		strn0cpy(s->worldpassword, LoginPassword, 30);
		SendPacket(pack);
	}
}
