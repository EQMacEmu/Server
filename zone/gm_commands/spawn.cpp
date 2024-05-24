#include "../client.h"

void command_spawn(Client *c, const Seperator *sep)
{
	const auto arguments = sep->argnum;
	if (!arguments) {
		c->Message(CC_Default, "Usage: #spawn [Name]");
		c->Message(
			CC_Default,
			"Optional Usage: #spawn [Name] [Race] [Level] [Texture] [Health] [Gender] [Class] [Primary Model] [Secondary Model] [Merchant ID] [Body Type]"
		);
		c->Message(
			CC_Default,
			"Name Format: NPCFirstname_NPCLastname - All numbers in a name are stripped and \"_\" characters become a space."
		);
		c->Message(
			CC_Default,
			"Note: Using \"-\" for gender will autoselect the gender for the race. Using \"-\" for HP will use the calculated maximum HP."
		);
		return;
	}

	const auto *m = entity_list.GetClientByName(sep->arg[1]);
	if (m) {
		c->Message(CC_Default, "You cannot spawn a mob with the same name as a character!");
		return;
	}

	const auto* n = NPC::SpawnNPC(sep->argplus[1], c->GetPosition(), c);
	if (!n) {
		c->Message(CC_Default, "Usage: #spawn [Name]");
		c->Message(
			CC_Default,
			"Optional Usage: #spawn [Name] [Race] [Level] [Texture] [Health] [Gender] [Class] [Primary Model] [Secondary Model] [Merchant ID] [Body Type]"
		);
		c->Message(
			CC_Default,
			"Name Format: NPCFirstname_NPCLastname - All numbers in a name are stripped and \"_\" characters become a space."
		);
		c->Message(
			CC_Default,
			"Note: Using \"-\" for gender will autoselect the gender for the race. Using \"-\" for HP will use the calculated maximum HP."
		);
	}
}

