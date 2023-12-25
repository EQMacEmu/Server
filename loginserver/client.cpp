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
#include "client.h"
#include "login_server.h"
#include "login_structures.h"
#include "../common/md5.h"
#include "../common/misc_functions.h"
#include "../common/eqemu_logsys.h"
#include "../common/sha1.h"
#include "eq_crypto.h"

extern EQCrypto eq_crypto;
extern EQEmuLogSys LogSys;
extern LoginServer server;

Client::Client(std::shared_ptr<EQStreamInterface> c, ClientVersion v)
{
	connection = c;
	version = v;
	status = cs_not_sent_session_ready;
	account_id = 0;
	sentsessioninfo = false;
	play_server_id = 0;
	play_sequence_id = 0;
}

bool Client::Process()
{
	EQApplicationPacket *app = connection->PopPacket();
	while(app)
	{
		LogDebug("Application packet received from client.");
		if(server.options.IsDumpInPacketsOn())
		{
			LogDebug("[Size: {0}] [{1}]", app->size, DumpPacketToString(app).c_str());
		}

		switch(app->GetOpcode())
		{
			case OP_SessionReady:
			{
				LogInfo("Session ready received from client.");
				Handle_SessionReady((const char*)app->pBuffer, app->Size());
				break;
			}
			case OP_LoginOSX:
			{
				std::string client;
				std::string check = DumpPacketToRawString(app->pBuffer, app->Size());

				if (check.find("eqworld-52.989studios.com") != std::string::npos)
				{
					LogInfo("Login received from OSX client.");
					client = "OSX";
				}
				else
				{
					LogInfo("Login received from ticketed PC client.");
					client = "PCT";
				}
				Handle_Login((const char*)app->pBuffer, app->Size(), client);
				break;
			}
			case OP_LoginPC:
			{
				if(app->Size() < 20)
				{
					LogError("Login received but it is too small, discarding.");
					break;
				}

				LogInfo("Login received from PC client.");
				Handle_Login((const char*)app->pBuffer, app->Size(), "PC");
				break;
			}
			case OP_LoginComplete:
			{
				LogInfo("Login complete received from client.");
				Handle_LoginComplete((const char*)app->pBuffer, app->Size());
				break;
			}
			case OP_LoginUnknown1: //Seems to be related to world status in older clients; we use our own logic for that though.
			{
				LogInfo("OP_LoginUnknown1 received from client.");
				auto outapp = new EQApplicationPacket(OP_LoginUnknown2, 0);
				connection->QueuePacket(outapp);
				delete(outapp);
				break;
			}
			case OP_ServerListRequest:
			{
				LogInfo("Server list request received from client.");
				SendServerListPacket();
				break;
			}
			case OP_PlayEverquestRequest:
			{
				if(app->Size() < sizeof(PlayEverquestRequest_Struct) && version != cv_old)
				{
					LogError("Play received but it is too small, discarding.");
					break;
				}
				Handle_Play((const char*)app->pBuffer);
				break;
			}
			case OP_LoginBanner:
			{
				Handle_Banner(app->Size());
				break;
			}
			default:
			{
				if (LogSys.log_settings[Logs::PacketClientServerUnhandled].is_category_enabled == 1) {
					char dump[64];
					app->build_header_dump(dump);
					LogError("Recieved unhandled application packet from the client: [{0}]", dump);
				}
			}
		}
		delete app;
		app = connection->PopPacket();
	}
	return true;
}

void Client::Handle_SessionReady(const char* data, unsigned int size)
{
	if(status != cs_not_sent_session_ready)
	{
		LogError("Session ready received again after already being received.");
		return;
	}

	if (version != cv_old)
	{
		if (size < sizeof(unsigned int))
		{
			LogError("Session ready was too small.");
			return;
		}

		unsigned int mode = *((unsigned int*)data);
		if (mode == (unsigned int)lm_from_world)
		{
			LogError("Session ready indicated logged in from world(unsupported feature), disconnecting.");
			connection->Close();
			return;
		}
	}

	status = cs_waiting_for_login;

	if(version == cv_old)
	{
		//Special logic for old streams.
		char buf[20];
		strcpy(buf, "12-4-2002 1800");
		auto outapp = new EQApplicationPacket(OP_SessionReady, strlen(buf) + 1);
		strcpy((char*)outapp->pBuffer, buf);
		connection->QueuePacket(outapp);
		delete outapp;
		LogInfo("EQMac Stream selected.");
	}
}

void Client::Handle_Login(const char* data, unsigned int size, std::string client)
{
	in_addr in{};
	in.s_addr = connection->GetRemoteIP();

	if (version != cv_old)
	{
		//Not old client, gtfo haxxor!
		string error = "Unauthorized client from " + string(inet_ntoa(in)) + " , exiting them.";
		LogError(error.c_str());
		return;
	}
	else if (status != cs_waiting_for_login)
	{
		LogError("Login received after already having logged in.");
		return;
	}

	else if (client != "PCT" && size < sizeof(LoginServerInfo_Struct))
	{
		LogError("Bad Login Struct size {0}.", size);
		return;
	}

	else if (client == "PCT" && size < sizeof(LoginServerInfo_Struct) - 21)
	{
		LogError("Bad Login Struct size {0}.", size);
		return;
	}

	string username;
	string password;
	string platform;
	bool allowedClient = 1;

	std::string allowOSX = server.db->LoginSettings("allow_OSX");
	std::string allowPC = server.db->LoginSettings("allow_PC");
	std::string allowPCT = server.db->LoginSettings("allow_PCT");

	std::transform(allowOSX.begin(), allowOSX.end(), allowOSX.begin(), ::toupper);
	std::transform(allowPC.begin(), allowPC.end(), allowPC.begin(), ::toupper);
	std::transform(allowPCT.begin(), allowPCT.end(), allowPCT.begin(), ::toupper);

	if (client == "OSX" && allowOSX != "TRUE")
	{
		allowedClient = 0;
	}
	else if (client == "PC" && allowPC != "TRUE")
	{
		allowedClient = 0;
	}
	else if (client == "PCT" && allowPCT != "TRUE")
	{
		allowedClient = 0;
	}

	if (!allowedClient)
	{
		string error = "Unauthorized client from " + string(inet_ntoa(in)) + " using client < " + client + " > , exiting them.";
		LogError(error.c_str());
		return;
	}

	if (client == "OSX")
	{
		string ourdata = data;

		if (size < strlen("eqworld-52.989studios.com") + 1)
			return;

		//Get rid of that 989 studios part of the string, plus remove null term zero.
		string userpass = ourdata.substr(0, ourdata.find("eqworld-52.989studios.com") - 1);

		username = userpass.substr(0, userpass.find("/"));
		password = userpass.substr(userpass.find("/") + 1);
		platform = "OSX";
		macversion = intel;
	}
	else if (client == "PC")
	{
		string e_hash;
		char* e_buffer = nullptr;
		string d_pass_hash;
		uchar eqlogin[40];
		eq_crypto.DoEQDecrypt((unsigned char*)data, eqlogin, 40);
		LoginCrypt_struct* lcs = (LoginCrypt_struct*)eqlogin;
		username = lcs->username;
		password = lcs->password;
		platform = "PC";
		macversion = pc;
	}
	else if (client == "PCT")
	{
		string ourdata = data;
		if (size < strlen("none") + 1)
			return;

		//Get rid of the "none" part of the string, plus remove null term zero.
		string userpass = ourdata.substr(0, ourdata.find("none") - 1);

		username = userpass.substr(0, userpass.find("/"));
		password = userpass.substr(userpass.find("/") + 1);
		platform = "PCT";
		macversion = pc;
	}
	string userandpass = password;
	status = cs_logged_in;
	unsigned int d_account_id = 0;
	string d_pass_hash;
	bool result = false;
	uchar sha1pass[40];
	char sha1hash[41];

	if (!server.db->GetLoginDataFromAccountName(username, d_pass_hash, d_account_id))
	{
		LogError("Error logging in, user {0} does not exist in the database.", username.c_str());
		LogError("platform : {} , username : {} does not exist", platform, username);
		if (server.options.CanAutoCreateAccounts())
		{
			LogInfo("platform : {} , username : {} is created", platform, username);
			server.db->CreateLoginData(username.c_str(), userandpass, d_account_id);
			
		}
		else
		{
			FatalError("Account does not exist and auto creation is not enabled.");
			return;
		}
		result = false;
	}
	else
	{
		sha1::calc(userandpass.c_str(), (int)userandpass.length(), sha1pass);
		sha1::toHexString(sha1pass, sha1hash);
		if (d_pass_hash.compare((char*)sha1hash) == 0)
		{
			result = true;
		}
		else
		{
			LogInfo("badpassword");
			LogError("[{0}]", sha1hash);
			result = false;
		}
	}
	if (result)
	{
		if (!sentsessioninfo)
		{
			LogInfo("username : {} logging on platform : {} is a success", username, platform);
			server.db->UpdateLSAccountData(d_account_id, string(inet_ntoa(in)));
			GenerateKey();
			account_id = d_account_id;
			account_name = username.c_str();

			if (client == "OSX")
			{
				auto outapp = new EQApplicationPacket(OP_LoginAccepted, sizeof(SessionIdEQMacPPC_Struct));
				SessionIdEQMacPPC_Struct* s_id = (SessionIdEQMacPPC_Struct*)outapp->pBuffer;
				// this is submitted to world server as "username"
				sprintf(s_id->session_id, "LS#%i", account_id);
				strcpy(s_id->unused, "unused");
				s_id->unknown = 4;
				connection->QueuePacket(outapp);
				delete outapp;

				string buf = server.options.GetNetworkIP();
				auto outapp2 = new EQApplicationPacket(OP_ServerName, (uint32)buf.length() + 1);
				strncpy((char*)outapp2->pBuffer, buf.c_str(), buf.length() + 1);
				connection->QueuePacket(outapp2);
				delete outapp2;
				sentsessioninfo = true;
			}
			else
			{
				auto outapp = new EQApplicationPacket(OP_LoginAccepted, sizeof(SessionId_Struct));
				SessionId_Struct* s_id = (SessionId_Struct*)outapp->pBuffer;
				// this is submitted to world server as "username"
				sprintf(s_id->session_id, "LS#%i", account_id);
				strcpy(s_id->unused, "unused");
				s_id->unknown = 4;
				connection->QueuePacket(outapp);
				delete outapp;
			}
		}
	}
	else
	{
		FatalError("Invalid username or password.");
	}
	return;
}

void Client::FatalError(const char* message) {
	auto outapp = new EQApplicationPacket(OP_ClientError, strlen(message) + 1);
	if (strlen(message) > 1) {
		strcpy((char*)outapp->pBuffer, message);
	}
	connection->QueuePacket(outapp);
	delete outapp;
	return;
}

void Client::Handle_LoginComplete(const char* data, unsigned int size) {
	auto outapp = new EQApplicationPacket(OP_LoginComplete, 20);
	outapp->pBuffer[0] = 1;
	connection->QueuePacket(outapp);
	delete outapp;
	return;
}


void Client::Handle_Play(const char* data)
{
	if(status != cs_logged_in)
	{
		LogError("Client sent a play request when they either were not logged in, discarding.");
		return;
	}

	if (data)
	{
		server.server_manager->SendOldUserToWorldRequest(data, account_id, connection->GetRemoteIP());
	}
}

void Client::SendServerListPacket()
{
	EQApplicationPacket* outapp = server.server_manager->CreateOldServerListPacket(this);

	if (server.options.IsDumpOutPacketsOn())
	{
		LogInfo("[Size: {0}] [{1}]", outapp->size, DumpPacketToString(outapp).c_str());
	}

	connection->QueuePacket(outapp);
	delete outapp;
}

void Client::Handle_Banner(unsigned int size)
{
	std::string ticker = "Welcome to EQMacEmu";
	if (server.db->CheckExtraSettings("ticker"))
	{
		ticker = server.db->LoginSettings("ticker");
	}

	auto outapp = new EQApplicationPacket(OP_LoginBanner);
	uint32 bufsize = 5 + strlen(ticker.c_str());
	outapp->size = bufsize;
	outapp->pBuffer = new uchar[bufsize];
	outapp->pBuffer[0] = 1;
	outapp->pBuffer[1] = 0;
	outapp->pBuffer[2] = 0;
	outapp->pBuffer[3] = 0;
	strcpy((char*)&outapp->pBuffer[4], ticker.c_str());
	connection->QueuePacket(outapp);
	delete outapp;
}

void Client::SendPlayResponse(EQApplicationPacket *outapp)
{
	LogDebug("Sending play response to client.");
	LogDebug("[Size: {0}] [{1}]", outapp->size, DumpPacketToString(outapp).c_str());

	connection->QueuePacket(outapp);
	status = cs_logged_in;
}

void Client::GenerateKey()
{
	key.clear();
	int count = 0;
	while (count < 10)
	{
		static const char key_selection[] =
		{
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', '0', '1', '2', '3', '4', '5',
			'6', '7', '8', '9'
		};

		key.append((const char*)&key_selection[random.Int(0, 35)], 1);
		count++;
	}
}
