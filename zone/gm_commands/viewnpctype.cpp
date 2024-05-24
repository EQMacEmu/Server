#include "../client.h"

void command_viewnpctype(Client *c, const Seperator *sep){
	if (!sep->IsNumber(1))
		c->Message(CC_Default, "Usage: #viewnpctype [npctype id]");
	else
	{
		uint32 npctypeid = atoi(sep->arg[1]);
		const NPCType* npct = database.LoadNPCTypesData(npctypeid);
		if (npct) {
			c->Message(CC_Default, " NPCType Info, ");
			c->Message(CC_Default, "  NPCTypeID: %u", npct->npc_id);
			c->Message(CC_Default, "  Name: %s", npct->name);
			c->Message(CC_Default, "  Level: %i", npct->level);
			c->Message(CC_Default, "  Race: %i", npct->race);
			c->Message(CC_Default, "  Class: %i", npct->class_);
			c->Message(CC_Default, "  MinDmg: %i", npct->min_dmg);
			c->Message(CC_Default, "  MaxDmg: %i", npct->max_dmg);
			c->Message(CC_Default, "  Special Abilities: %s", npct->special_abilities);
			c->Message(CC_Default, "  Spells: %i", npct->npc_spells_id);
			c->Message(CC_Default, "  Loot Table: %i", npct->loottable_id);
			c->Message(CC_Default, "  NPCFactionID: %i", npct->npc_faction_id);
		}
		else
			c->Message(CC_Default, "NPC #%d not found", npctypeid);
	}
}

