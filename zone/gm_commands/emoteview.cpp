#include "../client.h"

void command_emoteview(Client *c, const Seperator *sep){
	if (!c->GetTarget() || !c->GetTarget()->IsNPC())
	{
		c->Message(CC_Default, "You must target a NPC to view their emotes.");
		return;
	}

	if (c->GetTarget() && c->GetTarget()->IsNPC())
	{
		int count = 0;
		int emoteid = c->GetTarget()->CastToNPC()->GetEmoteID();

		for (auto &i : zone->NPCEmoteList)
		{
			NPC_Emote_Struct* nes = i;
			if (emoteid == nes->emoteid)
			{
				c->Message(CC_Default, "EmoteID: %i Event: %i Type: %i Text: %s", nes->emoteid, nes->event_, nes->type, nes->text);
				count++;
			}
		}
		if (count == 0)
			c->Message(CC_Default, "No emotes found.");
		else
			c->Message(CC_Default, "%i emote(s) found", count);
	}
}

