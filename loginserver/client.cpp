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
#include "../common/eq_packet.h"
#include "../common/md5.h"
#include "../common/misc_functions.h"
#include "../common/eqemu_logsys.h"
#include "../common/opcodemgr.h"
#include "../common/packet_dump.h"
#include "../common/rdp/rdp_stream.h"
#include "../common/sha1.h"
#include "eq_crypto.h"

#include <cstring>
#include <utility>

extern EQCrypto eq_crypto;
extern EQEmuLogSys LogSys;
extern LoginServer server;

Client::Client(std::unique_ptr<RDPStream> stream, LSMacClientVersion default_version)
	: m_client_status(cs_not_sent_session_ready),
	  m_client_mac_version(default_version),
	  m_is_trilogy(default_version == trilogy),
	  m_account_id(0),
	  m_sent_session_info(false),
	  m_ip(0),
	  m_port(0),
	  m_stream(std::move(stream))
{
	uint8 remote_address[4] = {};
	if (m_stream == nullptr || m_stream->GetRemoteAddress(remote_address, m_port) != RDPLIB_OK) {
		LogError("Login client was created without a usable RDP stream");
	}
	else {
		std::memcpy(&m_ip, remote_address, sizeof(m_ip));
	}
}

Client::~Client()
{
	CloseStream(0);
}

void Client::SendToStream(EQApplicationPacket **packet, bool reliable)
{
	if (packet == nullptr || *packet == nullptr)
		return;

	EmuOpcode opcode = (*packet)->GetOpcode();
	if (m_stream == nullptr) {
		delete *packet;
		*packet = nullptr;
		return;
	}

	int result = m_stream->Send(packet, reliable);
	if (result != RDPLIB_OK)
		LogNetcode("Unable to send login opcode [{}], RDP result [{}]", OpcodeManager::EmuToName(opcode), result);
}

void Client::QueuePacket(const EQApplicationPacket *packet, bool reliable)
{
	if (packet == nullptr)
		return;

	EQApplicationPacket *copy = packet->Copy();
	SendToStream(&copy, reliable);
}

void Client::CloseStream()
{
	CloseStream(RDPStream::DefaultLingerTimeout);
}

void Client::CloseStream(uint32 linger_timeout_ms)
{
	if (m_stream == nullptr)
		return;

	m_stream->Close(linger_timeout_ms);
	m_stream.reset();
}

bool Client::Process()
{
	bool transport_ended = m_stream == nullptr;
	uint32 disconnect_reason = 0;

	while (m_stream != nullptr) {
		EQApplicationPacket *app = nullptr;
		RDPStream::ReceiveResult receive_result = m_stream->Receive(&app, &disconnect_reason);
		if (receive_result == RDPStream::NoData)
			break;

		if (receive_result != RDPStream::PacketReceived) {
			transport_ended = true;
			if (receive_result == RDPStream::PeerClosed)
				LogNetcode("Login client [{}] closed its RDP connection", GetClientDescription());
			else
				LogNetcode("Login client [{}] lost its RDP connection, reason [{:#010x}]", GetClientDescription(), disconnect_reason);
			break;
		}

		LogPacketClientServer(
			"[{}] [{:#06x}] Size [{}] {}",
			OpcodeManager::EmuToName(app->GetOpcode()),
			app->GetProtocolOpcode(),
			app->Size(),
			(LogSys.IsLogEnabled(Logs::Detail, Logs::PacketClientServer) ? DumpPacketToString(app) : "")
		);

		switch(app->GetOpcode()) {
			case OP_SessionReady: {
				Handle_SessionReady();
				break;
			}
			case OP_LoginOSX: {
				std::string client;
				std::string check = DumpPacketToRawString(app->pBuffer, app->Size());

				if (check.find("eqworld-52.989studios.com") != std::string::npos) {
					LogInfo("Login received from OSX client {}", GetClientDescription());
					client = "OSX";
				}
				else {
					LogInfo("Login received from ticketed PC client {}", GetClientDescription());
					client = "PCT";
				}

				Handle_Login((const char*)app->pBuffer, app->Size(), client);
				break;
			}
			case OP_LoginPC: {
				if(app->Size() < 20) {
					LogError("Login received but it is too small, discarding.");
					break;
				}

				Handle_Login((const char*)app->pBuffer, app->Size(), "PC");
				LogInfo("Login received from PC client. {}", GetClientDescription());
				break;
			}
			case OP_LoginComplete: {
				LogInfo("Login complete received from client.");
				Handle_LoginComplete((const char*)app->pBuffer, app->Size());
				break;
			}
			case OP_LoginUnknown1: { //Seems to be related to world status in older clients; we use our own logic for that though.
				LogInfo("OP_LoginUnknown1 received from client.");
				auto outapp = new EQApplicationPacket(OP_LoginUnknown2, 0);
				QueuePacket(outapp);
				delete(outapp);
				break;
			}
			case OP_LoginDisconnect: {
				LogInfo("Client disconnected from the Server");
				break;
			}
			case OP_ServerListRequest: {
				LogInfo("Server list request received from client {}", GetClientDescription());

				SendServerListPacket();
				break;
			}
			case OP_PlayEverquestRequest: {
				Handle_Play((const char*)app->pBuffer);
				break;
			}
			case OP_LoginBanner: {
				Handle_Banner(app->Size());
				break;
			}
			default: {
				char dump[64];
				app->build_header_dump(dump);
				LogError("Received unhandled application packet from the client: [{}]", dump);
			}
		}
		delete app;
	}

	if (transport_ended || m_stream == nullptr) {
		CloseStream();
		return false;
	}

	return true;
}

void Client::Handle_SessionReady()
{
	if(m_client_status != cs_not_sent_session_ready)	{
		LogError("Session ready received again after already being received.");
		return;
	}

	m_client_status = cs_waiting_for_login;

	char buf[64] = {0};
	std::string ver;
	if (IsTrilogy()) {
		ver = server.options.GetLoginVersion();
		if (ver.empty() && server.db) {
			char db_buf[64] = {0};
			if (server.db->GetVariable("LoginVersion", db_buf, sizeof(db_buf))) {
				ver = db_buf;
			}
		}
		if (ver.empty()) {
			ver = "8-09-2001 14:25";
		}
	}
	else {
		ver = server.options.GetMacLoginVersion();
		if (ver.empty() && server.db) {
			char db_buf[64] = {0};
			if (server.db->GetVariable("MacLoginVersion", db_buf, sizeof(db_buf))) {
				ver = db_buf;
			}
		}
		if (ver.empty()) {
			ver = "12-4-2002 1800";
		}
	}

	strncpy(buf, ver.c_str(), sizeof(buf) - 1);
	auto outapp = new EQApplicationPacket(OP_SessionReady, strlen(buf) + 1);
	strcpy((char*)outapp->pBuffer, buf);
	LogInfo("SessionReady sent version timestamp: [{}] (client_type={})", buf, (IsTrilogy() ? "Trilogy" : "Mac"));
	QueuePacket(outapp);
	delete outapp;
}

void Client::Handle_Login(const char* data, unsigned int size, std::string client) {
	in_addr in{};
	in.s_addr = GetIP();

	if (m_client_status != cs_waiting_for_login) {
		LogError("Login received after already having logged in.");
		return;
	}

	else if (client != "PCT" && size < sizeof(LoginServerInfo_Struct)) {
		LogError("Bad Login Struct size {0}.", size);
		return;
	}

	else if (client == "PCT" && size < sizeof(LoginServerInfo_Struct) - 21) {
		LogError("Bad Login Struct size {0}.", size);
		return;
	}

	string username;
	string password;
	string platform;
	bool allowedClient = true;

	if (client == "OSX" && !server.options.IsIntelClientAllowed()) {
		allowedClient = false;
	}
	else if (client == "PC" && !server.options.IsPcClientAllowed()) {
		allowedClient = false;
	}
	else if (client == "PCT" && !server.options.IsTicketClientAllowed())	{
		allowedClient = false;
	}

	if (!allowedClient)	{
		LogError("Unauthorized client from {} using client < {} > , exiting them.", inet_ntoa(in), client);
		return;
	}

	if (client == "OSX") {
		string ourdata = data;

		if (size < strlen("eqworld-52.989studios.com") + 1)
			return;

		//Get rid of that 989 studios part of the string, plus remove null term zero.
		string userpass = ourdata.substr(0, ourdata.find("eqworld-52.989studios.com") - 1);

		username = userpass.substr(0, userpass.find("/"));
		password = userpass.substr(userpass.find("/") + 1);
		platform = "OSX";
		m_client_mac_version = intel;
	}
	else if (client == "PC") {
		string e_hash;
		char* e_buffer = nullptr;
		string d_pass_hash;
		uchar eqlogin[40];
		eq_crypto.DoEQDecrypt((unsigned char*)data, eqlogin, 40);
		LoginCrypt_struct* lcs = (LoginCrypt_struct*)eqlogin;
		username = lcs->username;
		password = lcs->password;
		platform = "PC";
		if (m_is_trilogy) {
			m_client_mac_version = trilogy;
		}
		else {
			m_client_mac_version = pc;
		}
	}
	else if (client == "PCT") {
		string ourdata = data;
		if (size < strlen("none") + 1)
			return;

		//Get rid of the "none" part of the string, plus remove null term zero.
		string userpass = ourdata.substr(0, ourdata.find("none") - 1);

		username = userpass.substr(0, userpass.find("/"));
		password = userpass.substr(userpass.find("/") + 1);
		platform = "PCT";
		m_client_mac_version = trilogy;
		m_is_trilogy = true;
	}
	std::string userandpass = m_salt.Salt(password);
	m_client_status = cs_logged_in;
	unsigned int d_account_id = 0;
	string d_pass_hash;
	bool result = false;
	uchar sha1pass[40];
	char sha1hash[41];

	if (!server.db->GetLoginDataFromAccountName(username, d_pass_hash, d_account_id)) {
		LogError("Error logging in, user {0} does not exist in the database.", username.c_str());
		LogError("platform : {} , username : {} does not exist", platform, username);
		if (server.options.CanAutoCreateAccounts())	{
			LogInfo("platform : {} , username : {} is created", platform, username);
			server.db->CreateLoginData(username.c_str(), userandpass, d_account_id);
			
		}
		else {
			FatalError("Account does not exist and auto creation is not enabled.");
			return;
		}
		result = false;
	}
	else {
		sha1::calc(userandpass.c_str(), (int)userandpass.length(), sha1pass);
		sha1::toHexString(sha1pass, sha1hash);
		if (d_pass_hash.compare((char*)sha1hash) == 0) {
			result = true;
		}
		else {
			LogInfo("badpassword");
			LogError("[{0}]", sha1hash);
			result = false;
		}
	}
	if (result)	{
		if (!m_sent_session_info) {
			LogInfo("username : {} logging on platform : {} is a success", username, platform);
			server.db->UpdateLSAccountData(d_account_id, string(inet_ntoa(in)));
			GenerateKey();
			m_account_id = d_account_id;
			m_account_name = username.c_str();

			if (client == "OSX") {
				auto outapp = new EQApplicationPacket(OP_LoginAccepted, sizeof(SessionIdEQMacPPC_Struct));
				SessionIdEQMacPPC_Struct* s_id = (SessionIdEQMacPPC_Struct*)outapp->pBuffer;
				// this is submitted to world server as "username"
				sprintf(s_id->session_id, "LS#%i", m_account_id);
				strcpy(s_id->unused, "unused");
				s_id->unknown = 4;
				QueuePacket(outapp);
				delete outapp;

				string buf = server.options.GetNetworkIP();
				auto outapp2 = new EQApplicationPacket(OP_ServerName, (uint32)buf.length() + 1);
				strncpy((char*)outapp2->pBuffer, buf.c_str(), buf.length() + 1);
				QueuePacket(outapp2);
				delete outapp2;
				m_sent_session_info = true;
			}
			else {
				auto outapp = new EQApplicationPacket(OP_LoginAccepted, sizeof(SessionId_Struct));
				SessionId_Struct* s_id = (SessionId_Struct*)outapp->pBuffer;
				// this is submitted to world server as "username"
				sprintf(s_id->session_id, "LS#%i", m_account_id);
				strcpy(s_id->unused, "unused");
				s_id->unknown = 4;
				QueuePacket(outapp);
				delete outapp;
			}
		}
	}
	else {
		FatalError("Invalid username or password.");
	}
	return;
}

void Client::FatalError(const char* message) {
	auto outapp = new EQApplicationPacket(OP_ClientError, strlen(message) + 1);
	if (strlen(message) > 1) {
		strcpy((char*)outapp->pBuffer, message);
	}
	QueuePacket(outapp);
	delete outapp;
	return;
}

void Client::Handle_LoginComplete(const char* data, unsigned int size) {
	auto outapp = new EQApplicationPacket(OP_LoginComplete, 20);
	outapp->pBuffer[0] = 1;
	QueuePacket(outapp);
	delete outapp;
	return;
}


void Client::Handle_Play(const char* data)
{
	if(m_client_status != cs_logged_in) {
		LogError("Client sent a play request when they either were not logged in, discarding.");
		return;
	}

	if (data) {
		server.server_manager->SendUserToWorldRequest(data, m_account_id, GetIP());
	}
}

void Client::SendServerListPacket()
{
	auto *outapp = server.server_manager->CreateServerListPacket(this);

	QueuePacket(outapp);
	delete outapp;
}

void Client::Handle_Banner(unsigned int size)
{
	std::string ticker = server.options.GetBannerTicker();
	if (server.options.GetBannerTicker().empty()) {
		ticker = "Welcome to EQMacEmu";
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
	QueuePacket(outapp);
	delete outapp;
}

void Client::SendPlayResponse(EQApplicationPacket *outapp)
{
	LogInfo("Sending play response for {}", GetClientDescription());

	QueuePacket(outapp);
	m_client_status = cs_logged_in;
}

void Client::GenerateKey()
{
	m_key.clear();
	int count = 0;
	while (count < 10) {
		static const char key_selection[] =	{
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', '0', '1', '2', '3', '4', '5',
			'6', '7', '8', '9'
		};

		m_key.append((const char*)&key_selection[m_random.Int(0, 35)], 1);
		count++;
	}
}

std::string Client::GetClientDescription()
{
	in_addr in{};
	in.s_addr = GetIP();
	std::string client_ip = inet_ntoa(in);

	return fmt::format(
		"account_name [{}] account_id ({}) ip_address [{}]",
		GetAccountName(),
		GetAccountID(),
		client_ip
	);
}
