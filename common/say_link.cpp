/*	EQEMu:  Everquest Server Emulator

	Copyright (C) 2001-2016 EQEMu Development Team (http://eqemulator.net)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "say_link.h"
#include "eq_constants.h"

#include "strings.h"
#include "item_instance.h"
#include "item_data.h"
#include "../zone/zonedb.h"


bool EQ::saylink::DegenerateLinkBody(SayLinkBody_Struct& say_link_body_struct, const std::string& say_link_body)
{
	memset(&say_link_body_struct, 0, sizeof(say_link_body_struct));
	if (say_link_body.length() != EQ::constants::SAY_LINK_BODY_SIZE)
		return false;

	say_link_body_struct.action_id = (uint8)strtol(say_link_body.substr(0, 1).c_str(), nullptr, 16);
	say_link_body_struct.item_id = (uint32)strtol(say_link_body.substr(1, 5).c_str(), nullptr, 16);

	return true;
}

bool EQ::saylink::GenerateLinkBody(std::string& say_link_body, const SayLinkBody_Struct& say_link_body_struct)
{

	static char itemid[7];
	sprintf(itemid, "%06d", say_link_body_struct.item_id);

	say_link_body = StringFormat(
		"%1X" "%s",
		(0x0F & say_link_body_struct.action_id),
		itemid
	);

	if (say_link_body.length() != EQ::constants::SAY_LINK_BODY_SIZE)
		return false;

	return true;
}

EQ::SayLinkEngine::SayLinkEngine()
{
	Reset();
}

const std::string& EQ::SayLinkEngine::GenerateLink()
{
	m_Link.clear();
	m_LinkBody.clear();
	m_LinkText.clear();

	generate_body();
	generate_text();

	if ((m_LinkBody.length() == EQ::constants::SAY_LINK_BODY_SIZE) && (m_LinkText.length() > 0)) {
		m_Link.push_back(0x12);
		m_Link.append(m_LinkBody);
		m_Link.append(m_LinkText);
		m_Link.push_back(0x12);
	}

	if ((m_Link.length() == 0) || (m_Link.length() > EQ::constants::SAY_LINK_MAXIMUM_SIZE)) {
		m_Error = true;
		m_Link = "<LINKER ERROR>";
		Log(Logs::General, Logs::Error, "SayLinkEngine::GenerateLink() failed to generate a useable say link");
		Log(Logs::General, Logs::Error, ">> LinkType: %i, Lengths: [link: %u(%u), body: %u(%u), text: %u(%u)]",
			m_LinkType,
			m_Link.length(),
			EQ::constants::SAY_LINK_MAXIMUM_SIZE,
			m_LinkBody.length(),
			EQ::constants::SAY_LINK_BODY_SIZE,
			m_LinkText.length(),
			EQ::constants::SAY_LINK_TEXT_SIZE
		);
		Log(Logs::General, Logs::Error, ">> LinkBody: %s", m_LinkBody.c_str());
		Log(Logs::General, Logs::Error, ">> LinkText: %s", m_LinkText.c_str());
	}

	return m_Link;
}

void EQ::SayLinkEngine::Reset()
{
	m_LinkType = saylink::SayLinkBlank;
	m_ItemData = nullptr;
	m_LootData = nullptr;
	m_ItemInst = nullptr;

	memset(&m_LinkBodyStruct, 0, sizeof(SayLinkBody_Struct));
	memset(&m_LinkProxyStruct, 0, sizeof(SayLinkProxy_Struct));

	m_TaskUse = false;
	m_Link.clear();
	m_LinkBody.clear();
	m_LinkText.clear();
	m_Error = false;
}

void EQ::SayLinkEngine::generate_body()
{
	memset(&m_LinkBodyStruct, 0, sizeof(SayLinkBody_Struct));

	const EQ::ItemData* item_data = nullptr;

	switch (m_LinkType) {
	case saylink::SayLinkBlank:
		break;
	case saylink::SayLinkItemData:
		if (m_ItemData == nullptr) { break; }
		m_LinkBodyStruct.item_id = m_ItemData->ID;
		break;
	case saylink::SayLinkLootItem:
		if (m_LootData == nullptr) { break; }
		item_data = database.GetItem(m_LootData->item_id);
		if (item_data == nullptr) { break; }
		m_LinkBodyStruct.item_id = item_data->ID;
		break;
	case saylink::SayLinkItemInst:
		if (m_ItemInst == nullptr) { break; }
		if (m_ItemInst->GetItem() == nullptr) { break; }
		m_LinkBodyStruct.item_id = m_ItemInst->GetItem()->ID;
		break;
	default:
		break;
	}

	if (m_LinkProxyStruct.action_id)
		m_LinkBodyStruct.action_id = m_LinkProxyStruct.action_id;
	if (m_LinkProxyStruct.item_id)
		m_LinkBodyStruct.item_id = m_LinkProxyStruct.item_id;

	static char itemid[7];
	sprintf(itemid, "%06d", m_LinkBodyStruct.item_id);

	m_LinkBody = StringFormat(
		"%1X" "%s",
		(0x0F & m_LinkBodyStruct.action_id),
		itemid
	);
}

void EQ::SayLinkEngine::generate_text()
{
	if (m_LinkProxyStruct.text != nullptr) {
		m_LinkText = m_LinkProxyStruct.text;
		return;
	}

	const EQ::ItemData* item_data = nullptr;

	switch (m_LinkType) {
	case saylink::SayLinkBlank:
		break;
	case saylink::SayLinkItemData:
		if (m_ItemData == nullptr) { break; }
		m_LinkText = m_ItemData->Name;
		return;
	case saylink::SayLinkLootItem:
		if (m_LootData == nullptr) { break; }
		item_data = database.GetItem(m_LootData->item_id);
		if (item_data == nullptr) { break; }
		m_LinkText = item_data->Name;
		return;
	case saylink::SayLinkItemInst:
		if (m_ItemInst == nullptr) { break; }
		if (m_ItemInst->GetItem() == nullptr) { break; }
		m_LinkText = m_ItemInst->GetItem()->Name;
		return;
	default:
		break;
	}

	m_LinkText = "null";
}

// to do: needs to produce better and reliable result otherwise.  
// If link sayid is over 1000 it'll pop up a item info window instead of the questsay.
std::string EQ::SayLinkEngine::GenerateQuestSaylink(std::string saylink_text, bool silent, std::string link_name)
{
	uint32 saylink_id = 0;

	/**
	 * Query for an existing phrase and id in the saylink table
	 */
	std::string query = StringFormat(
		"SELECT `id` FROM `saylink` WHERE `phrase` = '%s' LIMIT 1",
		Strings::Escape(saylink_text).c_str());

	auto results = database.QueryDatabase(query);

	if (results.Success()) {
		if (results.RowCount() >= 1) {
			for (auto row = results.begin(); row != results.end(); ++row)
				saylink_id = static_cast<uint32>(atoi(row[0]));
		}
		else {
			std::string insert_query = StringFormat(
				"INSERT INTO `saylink` (`phrase`) VALUES ('%s')",
				Strings::Escape(saylink_text).c_str());

			results = database.QueryDatabase(insert_query);
			if (!results.Success()) {
				Log(Logs::General, Logs::Error, "Error in saylink phrase queries %s", results.ErrorMessage().c_str());
			}
			else {
				saylink_id = results.LastInsertedID();
			}
		}
	}

	if (silent)
		saylink_id = saylink_id + 500;

	/**
	 * Generate the actual link
	 */
	EQ::SayLinkEngine linker;
	linker.SetProxyItemID(saylink_id);
	linker.SetProxyText(link_name.c_str());

	return linker.GenerateLink();
}

std::string Saylink::Create(std::string saylink_text, bool silent, std::string link_name)
{
	return EQ::SayLinkEngine::GenerateQuestSaylink(saylink_text, silent, link_name);
}
