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
#include "server_manager.h"
#include "client.h"
#include "login_server.h"
#include "login_types.h"
#include <stdlib.h>

#include "../common/eq_packet.h"
#include "../common/eqemu_logsys.h"
#include "../common/ip_util.h"

extern EQEmuLogSys LogSys;
extern LoginServer server;
extern bool run_server;

ServerManager::ServerManager()
{
	int listen_port = server.config.GetVariableInt("client_configuration", "listen_port", 5998);

	m_server_connection = std::make_unique<EQ::Net::ServertalkServer>();
	EQ::Net::ServertalkServerOptions opts;
	opts.port = listen_port;
	opts.ipv6 = false;
	m_server_connection->Listen(opts);

	LogInfo("Loginserver now listening on port [{0}]", listen_port);

	m_server_connection->OnConnectionIdentified(
		"World", [this](std::shared_ptr<EQ::Net::ServertalkServerConnection> world_connection) {
			LogInfo(
				"New World Server connection from {0}:{1}",
				world_connection->Handle()->RemoteIP(),
				world_connection->Handle()->RemotePort()
			);
				
			auto iter = m_world_servers.begin();
			while (iter != m_world_servers.end()) {
				if ((*iter)->GetConnection()->Handle()->RemoteIP().compare(world_connection->Handle()->RemoteIP()) ==
					0 &&
					(*iter)->GetConnection()->Handle()->RemotePort() == world_connection->Handle()->RemotePort()) {

					LogInfo(
						"World server already existed for {0}:{1}, removing existing connection.",
						world_connection->Handle()->RemoteIP(),
						world_connection->Handle()->RemotePort()
					);

					m_world_servers.erase(iter);
					break;
				}

				++iter;
			}

			m_world_servers.push_back(std::make_unique<WorldServer>(world_connection));
		}
	);

	m_server_connection->OnConnectionRemoved(
		"World", [this](std::shared_ptr<EQ::Net::ServertalkServerConnection> c) {
			auto iter = m_world_servers.begin();
			while (iter != m_world_servers.end()) {
				if ((*iter)->GetConnection()->GetUUID() == c->GetUUID()) {
					LogInfo(
						"World server {0} has been disconnected, removing.",
						(*iter)->GetServerLongName()
					);
					m_world_servers.erase(iter);
					return;
				}

				++iter;
			}
		}
	);
}

ServerManager::~ServerManager() = default;


WorldServer* ServerManager::GetServerByAddress(const std::string& addr, int port)
{
	auto iter = m_world_servers.begin();
	while (iter != m_world_servers.end()) {
		if ((*iter)->GetConnection()->Handle()->RemoteIP() == addr && (*iter)->GetConnection()->Handle()->RemotePort()) {
			return (*iter).get();
		}
		++iter;
	}

	return nullptr;
}

EQApplicationPacket* ServerManager::CreateServerListPacket(Client* c)
{
	if (c->IsTrilogy() || c->GetMacClientVersion() == trilogy) {
		return CreateServerListPacketTrilogy(c);
	}
	return CreateServerListPacketMac(c);
}

EQApplicationPacket* ServerManager::CreateServerListPacketTrilogy(Client* c)
{
	unsigned int header_size = sizeof(ServerListClassic_Struct);
	unsigned int flags_size = sizeof(ServerListServerFlagsClassic_Struct);

	unsigned int packet_size = header_size;
	unsigned int server_count = 0;
	in_addr in;
	in.s_addr = c->GetIP();
	std::string client_ip = inet_ntoa(in);

	auto iter = m_world_servers.begin();
	while (iter != m_world_servers.end()) {
		if ((*iter)->IsAuthorized() == false) {
			++iter;
			continue;
		}

		std::string world_ip = (*iter)->GetConnection()->Handle()->RemoteIP();
		std::string servername = (*iter)->GetServerLongName();

		std::string target_ip;
		if (world_ip.compare(client_ip) == 0 || IpUtil::IsIpInPrivateRfc1918(client_ip)) {
			target_ip = (*iter)->GetLocalIP();
		}
		else {
			target_ip = (*iter)->GetRemoteIP();
		}

		packet_size += servername.size() + 1 + target_ip.size() + 1 + flags_size;
		server_count++;
		++iter;
	}

	packet_size += sizeof(ServerListEndFlags_Struct);
	packet_size += 1; // chatserver null terminator

	auto outapp = new EQApplicationPacket(OP_ServerListRequest, packet_size);
	memset(outapp->pBuffer, 0, packet_size);

	uint8 showcount = 0x0;
	if (server.options.IsShowPlayerCountEnabled()) {
		showcount = 0xFF;
	}

	unsigned char* data_ptr = outapp->pBuffer;
	ServerListClassic_Struct* sl = (ServerListClassic_Struct*)outapp->pBuffer;
	sl->numservers = static_cast<uint8>(server_count);
	sl->unknown1 = 0;
	sl->unknown2 = 0;
	sl->showusercount = showcount;
	data_ptr += sizeof(ServerListClassic_Struct);

	iter = m_world_servers.begin();
	while (iter != m_world_servers.end()) {
		if ((*iter)->IsAuthorized() == false) {
			++iter;
			continue;
		}

		std::string world_ip = (*iter)->GetConnection()->Handle()->RemoteIP();
		std::string servername = (*iter)->GetServerLongName();

		std::string target_ip;
		if (world_ip.compare(client_ip) == 0 || IpUtil::IsIpInPrivateRfc1918(client_ip)) {
			target_ip = (*iter)->GetLocalIP();
		}
		else {
			target_ip = (*iter)->GetRemoteIP();
		}

		strcpy((char*)data_ptr, servername.c_str());
		data_ptr += servername.size() + 1;

		strcpy((char*)data_ptr, target_ip.c_str());
		data_ptr += target_ip.size() + 1;

		ServerListServerFlagsClassic_Struct* slsf = (ServerListServerFlagsClassic_Struct*)data_ptr;
		slsf->greenname = 0;
		slsf->usercount = (*iter)->GetStatus();
		memset(slsf->unknown, 0, sizeof(slsf->unknown));
		data_ptr += sizeof(ServerListServerFlagsClassic_Struct);

		++iter;
	}

	ServerListEndFlags_Struct* slef = (ServerListEndFlags_Struct*)data_ptr;
	memset(slef, 0, sizeof(ServerListEndFlags_Struct));
	slef->admin = 0;
	slef->kunark = 1;
	slef->velious = 1;

	return outapp;
}

EQApplicationPacket* ServerManager::CreateServerListPacketMac(Client* c)
{
	unsigned int packet_size = sizeof(ServerListMac_Struct);
	unsigned int server_count = 0;
	in_addr in;
	in.s_addr = c->GetIP();
	std::string client_ip = inet_ntoa(in);

	auto iter = m_world_servers.begin();
	while (iter != m_world_servers.end()) {
		if ((*iter)->IsAuthorized() == false) {
			++iter;
			continue;
		}

		std::string world_ip = (*iter)->GetConnection()->Handle()->RemoteIP();
		std::string servername = (*iter)->GetServerLongName();
		servername.append(" Server");

		std::string target_ip;
		if (world_ip.compare(client_ip) == 0 || IpUtil::IsIpInPrivateRfc1918(client_ip)) {
			target_ip = (*iter)->GetLocalIP();
		}
		else {
			target_ip = (*iter)->GetRemoteIP();
		}

		packet_size += servername.size() + 1 + target_ip.size() + 1 + sizeof(ServerListServerFlagsMac_Struct);
		server_count++;
		++iter;
	}

	packet_size += sizeof(ServerListEndFlags_Struct);
	packet_size += 1;

	auto outapp = new EQApplicationPacket(OP_ServerListRequest, packet_size);
	memset(outapp->pBuffer, 0, packet_size);

	ServerListMac_Struct* sl = (ServerListMac_Struct*)outapp->pBuffer;
	sl->numservers = static_cast<uint16>(server_count);
	sl->padding[0] = 0;
	sl->padding[1] = 0;
	uint8 showcount = 0x0;
	if (server.options.IsShowPlayerCountEnabled()) {
		showcount = 0xFF;
	}
	sl->showusercount = showcount;

	unsigned char* data_ptr = outapp->pBuffer;
	data_ptr += sizeof(ServerListMac_Struct);

	iter = m_world_servers.begin();
	while (iter != m_world_servers.end()) {
		if ((*iter)->IsAuthorized() == false) {
			++iter;
			continue;
		}

		std::string world_ip = (*iter)->GetConnection()->Handle()->RemoteIP();
		std::string servername = (*iter)->GetServerLongName();
		servername.append(" Server");

		std::string target_ip;
		if (world_ip.compare(client_ip) == 0 || IpUtil::IsIpInPrivateRfc1918(client_ip)) {
			target_ip = (*iter)->GetLocalIP();
		}
		else {
			target_ip = (*iter)->GetRemoteIP();
		}

		strcpy((char*)data_ptr, servername.c_str());
		data_ptr += (servername.size() + 1);

		strcpy((char*)data_ptr, target_ip.c_str());
		data_ptr += (target_ip.size() + 1);

		ServerListServerFlagsMac_Struct* slsf = (ServerListServerFlagsMac_Struct*)data_ptr;
		slsf->greenname = 0;
		slsf->flags = 0x1;
		slsf->worldid = (*iter)->GetServerId();
		slsf->usercount = (*iter)->GetStatus();
		data_ptr += sizeof(ServerListServerFlagsMac_Struct);
		++iter;
	}

	ServerListEndFlags_Struct* slef = (ServerListEndFlags_Struct*)data_ptr;
	memset(slef, 0, sizeof(ServerListEndFlags_Struct));
	slef->admin = 0;

	return outapp;
}

void ServerManager::SendUserToWorldRequest(const char* server_id, unsigned int client_account_id, uint32 ip)
{
	auto iter = m_world_servers.begin();
	bool found = false;
	std::string req_id = server_id ? server_id : "";

	while (iter != m_world_servers.end()) {
		if ((*iter)->GetRemoteIP() == req_id ||
		    (*iter)->GetLocalIP() == req_id ||
		    (*iter)->GetServerLongName() == req_id ||
		    (*iter)->GetServerShortName() == req_id ||
		    ((*iter)->GetServerLongName() + " Server") == req_id ||
		    (m_world_servers.size() == 1)) {
			EQ::Net::DynamicPacket outapp;
			outapp.Resize(sizeof(UsertoWorldRequest));
			UsertoWorldRequest* utwr = (UsertoWorldRequest*)outapp.Data();

			utwr->worldid = (*iter)->GetServerId();
			utwr->lsaccountid = client_account_id;
			utwr->ip = ip;
			(*iter)->GetConnection()->Send(ServerOP_UsertoWorldReq, outapp);
			found = true;

			LogPacketServerClient(
				"[{}]",
				outapp.ToString()
			);
			break;
		}

		++iter;
	}

	if (!found) {
		LogError("Client requested a user to world but supplied an invalid id of {}.", server_id);
	}
}

bool ServerManager::ServerExists(std::string l_name, std::string s_name, WorldServer *ignore)
{
	auto iter = m_world_servers.begin();
	while (iter != m_world_servers.end()) {
		if ((*iter).get() == ignore) {
			++iter;
			continue;
		}

		if ((*iter)->GetServerLongName().compare(l_name) == 0 && (*iter)->GetServerShortName().compare(s_name) == 0) {
			return true;
		}

		++iter;
	}
	return false;
}

void ServerManager::DestroyServerByName(std::string l_name, std::string s_name, WorldServer *ignore)
{
	auto iter = m_world_servers.begin();
	while (iter != m_world_servers.end()) {
		if ((*iter).get() == ignore) {
			++iter;
			continue;
		}

		if ((*iter)->GetServerLongName().compare(l_name) == 0 && (*iter)->GetServerShortName().compare(s_name) == 0) {
			(*iter)->GetConnection()->Handle()->Disconnect();
			iter = m_world_servers.erase(iter);
			continue;
		}

		++iter;
	}
}
