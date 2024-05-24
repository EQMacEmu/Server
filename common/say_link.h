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

#ifndef COMMON_SAY_LINK_H
#define COMMON_SAY_LINK_H


#include "types.h"
#include <string>
#include "loot.h"


struct LootItem;

namespace EQ
{
	class ItemInstance;
	struct SayLinkBody_Struct;
	struct ItemData;

	namespace saylink {
		enum SayLinkType {
			SayLinkBlank = 0,
			SayLinkItemData,
			SayLinkLootItem,
			SayLinkItemInst
		};

		extern bool DegenerateLinkBody(SayLinkBody_Struct& say_Link_body_struct, const std::string& say_link_body);
		extern bool GenerateLinkBody(std::string& say_link_body, const SayLinkBody_Struct& say_link_body_struct);

	}/*saylink*/

	struct SayLinkBody_Struct {
		uint8 action_id;		/* %1X */
		uint32 item_id;			/* %05X */
	};

	struct SayLinkProxy_Struct : SayLinkBody_Struct {
		const char* text;
	};

	class SayLinkEngine {
	public:
		SayLinkEngine();

		void SetLinkType(saylink::SayLinkType  link_type) { m_LinkType = link_type; }
		void SetItemData(const EQ::ItemData* item_data) { m_ItemData = item_data; }
		void SetLootData(const LootItem* loot_data) { m_LootData = loot_data; }
		void SetItemInst(const EQ::ItemInstance* item_inst) { m_ItemInst = item_inst; }

		// mainly for saylinks..but, not limited to
		void SetProxyActionID(uint8 proxy_action_id) { m_LinkProxyStruct.action_id = proxy_action_id; }
		void SetProxyItemID(uint32 proxy_item_id) { m_LinkProxyStruct.item_id = proxy_item_id; }
		
		void SetProxyText(const char* proxy_text) { m_LinkProxyStruct.text = proxy_text; } // overrides standard text use
		void SetTaskUse() { m_TaskUse = true; }

		const std::string& GenerateLink();
		bool LinkError() { return m_Error; }

		const std::string& Link() { return m_Link; }			// contains full string format: '/12x' '<LinkBody>' '<LinkText>' '/12x'
		const std::string& LinkBody() { return m_LinkBody; }	// contains string format: '<LinkBody>'
		const std::string& LinkText() { return m_LinkText; }	// contains string format: '<LinkText>'

		static std::string GenerateQuestSaylink(std::string saylink_text, bool silent, std::string link_name);

		void Reset();

	private:
		void generate_body();
		void generate_text();

		int m_LinkType;
		const ItemData     * m_ItemData;
		const LootItem     * m_LootData;
		const ItemInstance * m_ItemInst;

		SayLinkBody_Struct m_LinkBodyStruct;
		SayLinkProxy_Struct m_LinkProxyStruct;
		bool m_TaskUse;
		std::string m_Link;
		std::string m_LinkBody;
		std::string m_LinkText;
		bool m_Error;
	};

}

class Saylink {
public:
	static std::string Create(const std::string &saylink_text, bool silent,  const std::string &link_name = "");
	static std::string Silent(const std::string &saylink_text,  const std::string &link_name = "");
};

#endif /* COMMON_SAY_LINK_H */