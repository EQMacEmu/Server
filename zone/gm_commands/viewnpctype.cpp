#include "../client.h"

void command_viewnpctype(Client *c, const Seperator *sep){
	if (!sep->IsNumber(1))
		c->Message(Chat::White, "Usage: #viewnpctype [npctype id]");
	else
	{
		uint32 npctypeid = atoi(sep->arg[1]);
		const NPCType* npct = database.LoadNPCTypesData(npctypeid);
		if (npct) {
			c->Message(Chat::White, " NPCType Info, ");
			c->Message(Chat::White, "  NPCTypeID: %u", npct->npc_id);
			c->Message(Chat::White, "  Name: %s", npct->name);
			c->Message(Chat::White, "  Level: %i", npct->level);
			c->Message(Chat::White, "  Race: %i", npct->race);
			c->Message(Chat::White, "  Class: %i", npct->class_);
			c->Message(Chat::White, "  MinDmg: %i", npct->min_dmg);
			c->Message(Chat::White, "  MaxDmg: %i", npct->max_dmg);
			c->Message(Chat::White, "  Special Abilities: %s", npct->special_abilities);
			c->Message(Chat::White, "  Spells: %i", npct->npc_spells_id);
			c->Message(Chat::White, "  Loot Table: %i", npct->loottable_id);
			c->Message(Chat::White, "  NPCFactionID: %i", npct->npc_faction_id);
		}
		else
			c->Message(Chat::White, "NPC #%d not found", npctypeid);
	}
}

