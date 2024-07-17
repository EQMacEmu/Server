#include "../client.h"

void command_showpetspell(Client *c, const Seperator *sep){
	if (sep->arg[1][0] == 0)
		c->Message(Chat::White, "Usage: #ShowPetSpells [spellid | searchstring]");
	else if (SPDAT_RECORDS <= 0)
		c->Message(Chat::White, "Spells not loaded");
	else if (Seperator::IsNumber(sep->argplus[1]))
	{
		int spellid = atoi(sep->argplus[1]);
		if (spellid <= 0 || spellid >= SPDAT_RECORDS)
			c->Message(Chat::White, "Error: Number out of range");
		else
			c->Message(Chat::White, "	%i: %s, %s", spellid, spells[spellid].teleport_zone, spells[spellid].name);
	}
	else
	{
		int count = 0;
		std::string sName;
		std::string sCriteria;
		sCriteria = sep->argplus[1];
		for (auto & c : sCriteria) c = toupper(c);
		for (int i = 0; i < SPDAT_RECORDS; i++)
		{
			if (spells[i].name[0] != 0 && (spells[i].effectid[0] == SE_SummonPet || spells[i].effectid[0] == SE_NecPet))
			{
				sName = spells[i].teleport_zone;
				for (auto & c : sName) c = toupper(c);
				if (sName.find(sCriteria) != std::string::npos && (count <= 20))
				{
					c->Message(Chat::White, "	%i: %s, %s", i, spells[i].teleport_zone, spells[i].name);
					count++;
				}
				else if (count > 20)
					break;
			}
		}
		if (count > 20)
			c->Message(Chat::White, "20 spells found... max reached.");
		else
			c->Message(Chat::White, "%i spells found.", count);
	}
}