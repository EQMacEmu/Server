#include "../client.h"

void command_summonitem(Client *c, const Seperator *sep)
{
	uint32 itemid = 0;
	std::string cmd_msg = sep->msg;
	size_t link_open = cmd_msg.find('\x12');
	size_t link_close = cmd_msg.find_last_of('\x12');
	if (link_open != link_close && (cmd_msg.length() - link_open) > EQ::constants::SAY_LINK_BODY_SIZE) {
		EQ::SayLinkBody_Struct link_body;
		EQ::saylink::DegenerateLinkBody(link_body, cmd_msg.substr(link_open + 1, EQ::constants::SAY_LINK_BODY_SIZE));
		itemid = link_body.item_id;
	}
	else if (!sep->IsNumber(1)) {
		c->Message(CC_Default, "Usage: #summonitem [item id] [charges], charges are optional");
		return;
	}
	else {
		uint32 itemid = atoi(sep->arg[1]);
		int16 item_status = 0;
		const EQ::ItemData* item = database.GetItem(itemid);
		if (item) {
			item_status = static_cast<int16>(item->MinStatus);
		}

		int16 charges = 0;
		if (sep->argnum<2 || !sep->IsNumber(2))
		{
			if(item && database.ItemQuantityType(itemid) == EQ::item::Quantity_Charges)
			{
				charges = item->MaxCharges;
			}
			else
			{
				charges = 0;
			}
		}
		else
			charges = atoi(sep->arg[2]);

		//No rent GM Uber Sword
		if (item_status > c->Admin() || (c->Admin() < 80 && itemid == 2661))
			c->Message(CC_Red, "Error: Insufficient status to summon this item.");
		else
			c->SummonItem(itemid, charges, 0, true);
	}
}

