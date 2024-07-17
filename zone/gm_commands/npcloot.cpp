#include "../client.h"
#include "../corpse.h"

void command_npcloot(Client *c, const Seperator *sep){
	if (c->GetTarget() == 0)
		c->Message(Chat::White, "Error: No target");
	// #npcloot show
	else if (strcasecmp(sep->arg[1], "show") == 0)
	{
		if (c->GetTarget()->IsNPC())
			c->GetTarget()->CastToNPC()->QueryLoot(c);
		else if (c->GetTarget()->IsCorpse())
			c->GetTarget()->CastToCorpse()->QueryLoot(c);
	}
	// These 2 types are *BAD* for the next few commands
	else if (c->GetTarget()->IsClient() || c->GetTarget()->IsCorpse())
		c->Message(Chat::White, "Error: Invalid target type, try a NPC =).");
	// #npcloot add
	else if (strcasecmp(sep->arg[1], "add") == 0)
	{
		// #npcloot add item
		if (c->GetTarget()->IsNPC() && sep->IsNumber(2))
		{
			uint32 item = atoi(sep->arg[2]);
			const EQ::ItemData* dbitem = database.GetItem(item);
			if (dbitem)
			{
				bool quest = false;
				if (sep->arg[4][0] != 0 && sep->IsNumber(4))
					quest = atoi(sep->arg[4]);

				if (sep->arg[3][0] != 0 && sep->IsNumber(3))
					c->GetTarget()->CastToNPC()->AddItem(item, atoi(sep->arg[3]), !quest, quest);
				else
				{
					if(database.ItemQuantityType(item) == EQ::item::Quantity_Charges)
					{
						int8 charges = dbitem->MaxCharges;
						c->GetTarget()->CastToNPC()->AddItem(item, charges, !quest, quest);
					}
					else
						c->GetTarget()->CastToNPC()->AddItem(item, 1, !quest, quest);
				}
				c->Message(Chat::White, "Added item(%i) to %s's loot.", item, c->GetTarget()->GetCleanName());
			}
			else
				c->Message(Chat::White, "Error: #npcloot add: Item(%i) does not exist!", item);
		}
		else if (!sep->IsNumber(2))
			c->Message(Chat::White, "Error: #npcloot add: Itemid must be a number.");
		else
			c->Message(Chat::White, "Error: #npcloot add: This is not a valid target.");
	}
	// #npcloot remove
	else if (strcasecmp(sep->arg[1], "remove") == 0)
	{
		//#npcloot remove all
		if (strcasecmp(sep->arg[2], "all") == 0)
		{
			c->GetTarget()->CastToNPC()->ClearLootItems();
			c->GetTarget()->CastToNPC()->RemoveLootCash();
			c->Message(Chat::White, "Removed all loot from %s.", c->GetTarget()->GetCleanName());
		}
		//#npcloot remove itemid
		else
		{
			if (c->GetTarget()->IsNPC() && sep->IsNumber(2))
			{
				NPC* npc = c->GetTarget()->CastToNPC();
				uint32 item = atoi(sep->arg[2]);

				if (npc)
				{
					LootItem* sitem = npc->GetItemByID(item);
					npc->RemoveItem(sitem);
					c->Message(Chat::White, "Removed item(%i) from %s's loot.", item, npc->GetCleanName());
				}
			}
			else if (!sep->IsNumber(2))
				c->Message(Chat::White, "Error: #npcloot remove: Item must be a number.");
			else
				c->Message(Chat::White, "Error: #npcloot remove: This is not a valid target.");
		}
	}
	// #npcloot money
	else if (strcasecmp(sep->arg[1], "money") == 0)
	{
		if (c->GetTarget()->IsNPC() && sep->IsNumber(2) && sep->IsNumber(3) && sep->IsNumber(4) && sep->IsNumber(5))
		{
			if ((atoi(sep->arg[2]) < 34465 && atoi(sep->arg[2]) >= 0) && (atoi(sep->arg[3]) < 34465 && atoi(sep->arg[3]) >= 0) && (atoi(sep->arg[4]) < 34465 && atoi(sep->arg[4]) >= 0) && (atoi(sep->arg[5]) < 34465 && atoi(sep->arg[5]) >= 0))
			{
				c->GetTarget()->CastToNPC()->AddLootCash(atoi(sep->arg[5]), atoi(sep->arg[4]), atoi(sep->arg[3]), atoi(sep->arg[2]));
				c->Message(Chat::White, "Set %i Platinum, %i Gold, %i Silver, and %i Copper as %s's money.", atoi(sep->arg[2]), atoi(sep->arg[3]), atoi(sep->arg[4]), atoi(sep->arg[5]), c->GetTarget()->GetName());
			}
			else
				c->Message(Chat::White, "Error: #npcloot money: Values must be between 0-34465.");
		}
		else
			c->Message(Chat::White, "Usage: #npcloot money platinum gold silver copper");
	}
	else
		c->Message(Chat::White, "Usage: #npcloot [show/money/add/remove] [itemid/all/money: pp gp sp cp] [quantity] [quest]");
}

