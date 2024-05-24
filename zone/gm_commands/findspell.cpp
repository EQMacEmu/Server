#include "../client.h"

void command_findspell(Client *c, const Seperator *sep){
	if (sep->arg[1][0] == 0)
		c->Message(CC_Default, "Usage: #FindSpell [spellname]");
	else if (SPDAT_RECORDS <= 0)
		c->Message(CC_Default, "Spells not loaded");
	else if (Seperator::IsNumber(sep->argplus[1])) {
		int spellid = atoi(sep->argplus[1]);
		if (spellid <= 0 || spellid >= SPDAT_RECORDS) {
			c->Message(CC_Default, "Error: Number out of range");
		}
		else {
			c->Message(CC_Default, "  %i: %s", spellid, spells[spellid].name);
		}
	}
	else {
		int count = 0;
		//int iSearchLen = strlen(sep->argplus[1])+1;
		std::string sName;
		std::string sCriteria;
		sCriteria = sep->argplus[1];
		for (auto & c : sCriteria) c = toupper(c);
		for (int i = 0; i<SPDAT_RECORDS; i++) {
			if (spells[i].name[0] != 0) {
				sName = spells[i].name;

				for (auto & c : sName) c = toupper(c);
				if (sName.find(sCriteria) != std::string::npos && (count <= 20)) {
					c->Message(CC_Default, "  %i: %s", i, spells[i].name);
					count++;
				}
				else if (count > 20)
					break;
			}
		}
		if (count > 20)
			c->Message(CC_Default, "20 spells found... max reached.");
		else
			c->Message(CC_Default, "%i spells found.", count);
	}
}

