#include "../client.h"

void command_memspell(Client *c, const Seperator *sep){
	uint32 slot;
	uint16 spell_id;

	if (!(sep->IsNumber(1) && sep->IsNumber(2)))
	{
		c->Message(CC_Default, "Usage: #MemSpell slotid spellid");
	}
	else
	{
		slot = atoi(sep->arg[1]) - 1;
		spell_id = atoi(sep->arg[2]);
		if (slot > MAX_PP_MEMSPELL || spell_id >= SPDAT_RECORDS)
		{
			c->Message(CC_Default, "Error: #MemSpell: Arguement out of range");
		}
		else
		{
			c->MemSpell(spell_id, slot);
			c->Message(CC_Default, "Spell slot changed, have fun!");
		}
	}
}

