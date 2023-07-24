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
#ifndef EQEMU_LOGINSTRUCTURES_H
#define EQEMU_LOGINSTRUCTURES_H

#pragma pack(1)

struct ServerList_Struct {
	uint16	numservers;
	uint8	padding[2];
	uint8	showusercount; // 0xFF = show numbers, 0x0 = show "UP"
	uchar	data[0];
};

struct ServerList_Trilogy_Struct {
	uint8	numservers;
	uint8	padding[2];
	uint8	showusercount; // 0xFF = show numbers, 0x0 = show "UP"
	uchar	data[0];
};

struct ServerListServerFlags_Struct {
	uint8 greenname;
	int32 flags; // if 0x8 then server is hidden on list
	int32 worldid;
	uint32 usercount;
};

struct ServerListServerFlags_Trilogy_Struct {
	uint8 greenname;
	uint32 usercount;
};

struct ServerListEndFlags_Struct {
	uint32 admin;
	uint8 zeroes_a[8];
	uint8 kunark;
	uint8 velious;
	uint8 zeroes_b[11];
};

struct SessionId_Struct {
	char	session_id[10];
	char	unused[7];
	uint32	unknown; // legends? dunno, seen a 4 here, so gonna use that for now
};

struct SessionIdEQMacPPC_Struct {
	char	session_id[10];
	char	unused[7];
	uint32	unknown; // legends? dunno, seen a 4 here, so gonna use that for now
	char	padding[7];
};

struct LoginServerInfo_Struct {
	uint8	crypt[40];
	uint8	unknown[0];	// in here is the server name you just logged out of, variable length
};

struct Update_Struct {
	uint32 flag;
	char description[0];
};

struct LoginCrypt_struct {
	char	username[20];
	char	password[20];
};

struct PlayEverquestRequest_Struct
{
	uint16 Sequence;
	uint32 Unknown1;
	uint32 Unknown2;
	uint32 ServerNumber;
};

struct PlayEverquestResponse_Struct {
	uint8 Sequence;
	uint8 Unknown1[9];
	uint8 Allowed;
	uint16 Message;
	uint8 Unknown2[3];
	uint32 ServerNumber;
};

static const unsigned char FailedLoginResponseData[] = {
	0xf6, 0x85, 0x9c, 0x23, 0x57, 0x7e, 0x3e, 0x55, 0xb3, 0x4c, 0xf8, 0xc8, 0xcb, 0x77, 0xd5, 0x16,
	0x09, 0x7a, 0x63, 0xdc, 0x57, 0x7e, 0x3e, 0x55, 0xb3, 0x4c, 0xf8, 0xc8, 0xcb, 0x77, 0xd5, 0x16,
	0x09, 0x7a, 0x63, 0xdc, 0x57, 0x7e, 0x3e, 0x55, 0xb3 };

#pragma pack()

#endif