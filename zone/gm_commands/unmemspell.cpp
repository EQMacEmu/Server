#include "../client.h"

void command_unmemspell(Client *c, const Seperator *sep){
	uint16 spell_id = 0;
	uint16 mem_slot = -1;
	Client *t = c;

	if (c->GetTarget() && c->GetTarget()->IsClient() && c->GetGM())
		t = c->GetTarget()->CastToClient();

	if (!sep->arg[1][0]) {
		c->Message(CC_Default, "FORMAT: #unmemspell <spellid>");
		return;
	}

	spell_id = atoi(sep->arg[1]);

	if (IsValidSpell(spell_id)) {
		mem_slot = t->FindSpellMemSlotBySpellID(spell_id);

		if (mem_slot >= 0) {
			t->UnmemSpell(mem_slot);

			t->Message(CC_Default, "Unmemming spell: %s (%i) from gembar.", spells[spell_id].name, spell_id);

			if (t != c)
				c->Message(CC_Default, "Unmemming spell: %s (%i) for %s.", spells[spell_id].name, spell_id, t->GetName());

			Log(Logs::Detail, Logs::Normal, "Unmem spell: %s (%i) request for %s from %s.", spells[spell_id].name, spell_id, t->GetName(), c->GetName());
		}
		else {
			t->Message(CC_Red, "Unable to unmemspell spell: %s (%i) from your gembar. This spell is not memmed.", spells[spell_id].name, spell_id);

			if (t != c)
				c->Message(CC_Red, "Unable to unmemspell spell: %s (%i) for %s due to spell not memmed.", spells[spell_id].name, spell_id, t->GetName());
		}
	}
}

