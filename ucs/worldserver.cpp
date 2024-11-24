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
#include "../common/eqemu_logsys.h"
#include "../common/servertalk.h"
#include "../common/misc_functions.h"
#include "../common/packet_functions.h"
#include "../common/md5.h"
#include "worldserver.h"
#include "clientlist.h"
#include "ucsconfig.h"
#include "database.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <iomanip>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>

extern WorldServer worldserver;
extern Clientlist *g_Clientlist;
extern const ucsconfig *Config;
extern Database database;

WorldServer::WorldServer()
{
	m_connection = std::make_unique<EQ::Net::ServertalkClient>(Config->WorldIP, Config->WorldTCPPort, false, "UCS", Config->SharedKey);
	m_connection->OnMessage(std::bind(&WorldServer::ProcessMessage, this, std::placeholders::_1, std::placeholders::_2));
}

WorldServer::~WorldServer()
{
}

void WorldServer::ProcessMessage(uint16 opcode, EQ::Net::Packet& p) {
	ServerPacket tpack(opcode, p);
	ServerPacket* pack = &tpack;

	LogNetcode("Received Opcode: {:#04x}", opcode);

	switch(opcode) {
		case 0: {
			break;
		}
		case ServerOP_KeepAlive: {
			break;
		}
		case ServerOP_UCSMessage: {
			char *Buffer = (char *)pack->pBuffer;

			auto From = new char[strlen(Buffer) + 1];

			VARSTRUCT_DECODE_STRING(From, Buffer);

			std::string Message = Buffer;

			LogInfo("Player: [{0}], Sent Message: [{1}]", From, Message.c_str());

			Client *c = g_Clientlist->FindCharacter(From);

			safe_delete_array(From);

			if (Message.length() < 2) {
				break;
			}

			if(!c) {
				LogInfo("Client not found");
				break;
			}

			if(Message[0] == ';') {
					c->SendChannelMessageByNumber(Message.substr(1, std::string::npos));
			}
			else if(Message[0] == '[') {
				g_Clientlist->ProcessOPChatCommand(c, Message.substr(1, std::string::npos));
			}

			break;
		}
	}
}