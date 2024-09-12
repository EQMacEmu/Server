#include "../../client.h"

void SetTexture(Client *c, const Seperator *sep){

	uint16 texture;
	if (sep->IsNumber(2) && atoi(sep->arg[2]) >= 0 && atoi(sep->arg[2]) <= 255) {
		texture = atoi(sep->arg[2]);
		uint8 helm = 0xFF;

		// Player Races Wear Armor, so Wearchange is sent instead
		int i;
		if (!c->GetTarget())
			for (i = EQ::textures::textureBegin; i <= EQ::textures::LastTintableTexture; i++)
			{
			c->WearChange(i, texture, 0);
			}
		else if (c->GetTarget()->IsPlayableRace(c->GetTarget()->GetRace())) {
			for (i = EQ::textures::textureBegin; i <= EQ::textures::LastTintableTexture; i++)
			{
				c->GetTarget()->WearChange(i, texture, 0);
			}
		}
		else	// Non-Player Races only need Illusion Packets to be sent for texture
		{
			if (sep->IsNumber(3) && atoi(sep->arg[3]) >= 0 && atoi(sep->arg[3]) <= 255)
				helm = atoi(sep->arg[3]);
			else
				helm = texture;

			if (texture == 255) {
				texture = 0xFFFF;	// Should be pulling these from the database instead
				helm = 0xFF;
			}

			if ((c->GetTarget()) && (c->Admin() >= commandTextureOthers))
				c->GetTarget()->SendIllusionPacket(c->GetTarget()->GetRace(), 0xFF, texture, helm);
			else
				c->SendIllusionPacket(c->GetRace(), 0xFF, texture, helm);
		}
	}
	else
		c->Message(Chat::White, "Usage: #texture [texture] [helmtexture] (0-255, 255 for show equipment)");
}

