#include "../client.h"

void command_giveitem(Client *c, const Seperator *sep){
	if (!sep->IsNumber(1)) {
		c->Message(Chat::Red, "Usage: #summonitem [item id] [charges], charges are optional");
	}
	else if (c->GetTarget() == nullptr) {
		c->Message(Chat::Red, "You must target a client to give the item to.");
	}
	else if (!c->GetTarget()->IsClient()) {
		c->Message(Chat::Red, "You can only give items to players with this command.");
	}
	else {
		Client *t = c->GetTarget()->CastToClient();
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
			c->Message(Chat::Red, "Error: Insufficient status to summon this item.");
		else
			t->SummonItem(itemid, charges, 0, true);
	}
}

