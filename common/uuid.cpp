/*
	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2014 EQEMu Development Team (http://eqemulator.net)
	
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

#include "uuid.h"

#ifdef WIN32
#include <rpc.h>
#else
#include <uuid/uuid.h>
#endif

std::string CreateUUID() {
#ifdef WIN32
	UUID uuid;
	/*
	C6031	Return value ignored
	Return value ignored: 'UuidCreate'.	common	uuid.cpp	31

	C6102		Using 'uuid' from failed function call at line '31'.	common	uuid.cpp	33
	'uuid' is not initialized			31
	'uuid' is an Input to 'UuidToStringA'
	(declared at c:\program files (x86)\windows kits\8.1\include\shared\rpcdce.h:2496)			33
	'uuid' is used, but may not have been initialized			33

	C6102		Using 'str' from failed function call at line '33'.	common	uuid.cpp	34
	'str' is not initialized			33
	'str' is an In/Out argument to 'std::basic_string<char,std::char_traits<char>,std::allocator<char> >::{ctor}'
	(declared at c:\program files (x86)\microsoft visual studio 12.0\vc\include\xstring:778)			34
	'str' is used, but may not have been initialized			34
	*/
	UuidCreate(&uuid);
	unsigned char *str = nullptr;
	UuidToStringA(&uuid, &str);
	std::string s((char*)str);
	RpcStringFreeA(&str);
	return s;
#else
	char str[64] = { 0 };
	uuid_t uuid;
	uuid_generate_random(uuid);
	uuid_unparse(uuid, str);
	return str;
#endif
}
