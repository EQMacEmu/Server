#include "../../client.h"

void SetTexture(Client *c, const Seperator *sep)
{
	const auto arguments = sep->argnum;
	if (arguments < 2 || (!sep->IsNumber(2) && Strings::ToUnsignedInt(sep->arg[2]) < 0 && Strings::ToUnsignedInt(sep->arg[2]) > 255)) {
		c->Message(Chat::White, "Usage: #set texture [Texture] [Helmet Texture]");
		return;
	}

	uint16 texture = Strings::ToUnsignedInt(sep->arg[2]);
	uint8 helmet_texture = 0xFF;

	Mob *t = c;
	if (c->GetTarget()) {
		t = c->GetTarget();
	}

	if (IsPlayerRace(t->GetRace())) { // Player Races Wear Armor, so Wearchange is sent instead
		for (
			int texture_slot = EQ::textures::textureBegin;
			texture_slot <= EQ::textures::LastTintableTexture;
			texture_slot++
			) {
			t->WearChange(texture_slot, texture, 0);
		}
	}
	else { // Non-Player Races only need Illusion Packets to be sent for texture
		uint8 helmet_texture = (
			(sep->IsNumber(3) && Strings::ToUnsignedInt(sep->arg[3]) >= 0 && Strings::ToUnsignedInt(sep->arg[3]) <= 255) ?
			Strings::ToUnsignedInt(sep->arg[3]) :
			texture
		);

		if (texture == 255) {
			texture = 0xFFFF;
			helmet_texture = 0xFF;
		}

		t->SendIllusionPacket(
			t->GetRace(), 
			0xFF, 
			texture, 
			helmet_texture
		);
	}

	c->Message(
		Chat::White,
		fmt::format(
			"Texture Changed for {} | Texture: {}{}",
			c->GetTargetDescription(t, TargetDescriptionType::UCSelf),
			texture,
			(
				IsPlayerRace(t->GetRace()) ?
				"" :
				fmt::format(
					" Helmet Texture: {}",
					helmet_texture
				)
				)
		).c_str()
	);
}
