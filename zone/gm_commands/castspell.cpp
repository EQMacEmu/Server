#include "../client.h"

void command_castspell(Client *c, const Seperator *sep){
	if (!sep->IsNumber(1))
	{
		c->Message(Chat::White, "Usage: #CastSpell spellid gm_override entityid");
		c->Message(Chat::White, "gm_override 0: Cast Normally. 1: Skip stacking and resist checks. 2: Normal, but don't send buff fade when a spell overwrites.");
	}
	else 
	{
		uint16 spellid = atoi(sep->arg[1]);
		uint8 gm_override = atoi(sep->arg[2]);
		uint16 entityid = atoi(sep->arg[3]);

		if(entityid > 0)
		{ 
			Mob* caster = entity_list.GetMob(entityid);
			if (caster && caster->IsNPC())
			{
				if (c->GetTarget() == 0)
				{
					caster->CastSpell(spellid, c->GetID(), EQ::spells::CastingSlot::Item);
				}
				else
				{
					caster->CastSpell(spellid, c->GetTarget()->GetID(), EQ::spells::CastingSlot::Item);
				}
			}
			else
			{
				c->Message(Chat::White, "Caster specified is not a NPC or is not valid.");
				return;
			}
		}
		/*
		Spell restrictions.
		*/
		else if (((spellid == 2859) || (spellid == 841) || (spellid == 300) || (spellid == 2314) ||
			(spellid == 3716) || (spellid == 911) || (spellid == 3014) || (spellid == 982) ||
			(spellid == 905) || (spellid == 2079) || (spellid == 1218) || (spellid == 819) ||
			((spellid >= 780) && (spellid <= 785)) || ((spellid >= 1200) && (spellid <= 1205)) ||
			((spellid >= 1342) && (spellid <= 1348)) || (spellid == 1923) || (spellid == 1924) ||
			(spellid == 3355)) &&
			c->Admin() < commandCastSpecials)
			c->Message(Chat::Red, "Unable to cast spell.");
		else if (spellid >= SPDAT_RECORDS)
			c->Message(Chat::White, "Error: #CastSpell: Argument out of range");
		else
		{
			if (c->GetTarget() == 0)
			{
				if (gm_override != 0)
				{
					c->SetGMSpellException(gm_override);
				}

				if (c->Admin() >= commandInstacast)
					c->SpellFinished(spellid, 0, EQ::spells::CastingSlot::Item, 0, -1, spells[spellid].ResistDiff);
				else
					c->CastSpell(spellid, 0, EQ::spells::CastingSlot::Item, 0);

				c->SetGMSpellException(0);
			}
			else
			{
				if (gm_override != 0)
				{
					c->GetTarget()->SetGMSpellException(gm_override);
				}

				if (c->Admin() >= commandInstacast)
					c->SpellFinished(spellid, c->GetTarget(), EQ::spells::CastingSlot::Item, 0, -1, spells[spellid].ResistDiff);
				else
					c->CastSpell(spellid, c->GetTarget()->GetID(), EQ::spells::CastingSlot::Item, 0);

				if(c->GetTarget())
					c->GetTarget()->SetGMSpellException(0);
			}
		}
	}
}

