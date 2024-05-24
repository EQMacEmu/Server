#include "../client.h"

void command_beard(Client *c, const Seperator *sep){
	Mob *target = c->GetTarget();
	if (!sep->IsNumber(1))
		c->Message(CC_Default, "Usage: #beard [number of beard style]");
	else if (!target)
		c->Message(CC_Default, "Error: this command requires a target");
	else {
		uint16 Race = target->GetRace();
		uint8 Gender = target->GetGender();
		uint8 Texture = 0xFF;
		uint8 HelmTexture = 0xFF;
		uint8 HairColor = target->GetHairColor();
		uint8 BeardColor = target->GetBeardColor();
		uint8 EyeColor1 = target->GetEyeColor1();
		uint8 EyeColor2 = target->GetEyeColor2();
		uint8 HairStyle = target->GetHairStyle();
		uint8 LuclinFace = target->GetLuclinFace();
		uint8 Beard = atoi(sep->arg[1]);

		target->SendIllusionPacket(Race, Gender, Texture, HelmTexture, HairColor, BeardColor,
			EyeColor1, EyeColor2, HairStyle, LuclinFace, Beard, 0xFF);

		c->Message(CC_Default, "Beard = %i", atoi(sep->arg[1]));
	}
}

