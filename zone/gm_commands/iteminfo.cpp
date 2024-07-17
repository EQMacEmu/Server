#include "../client.h"

void command_iteminfo(Client *c, const Seperator *sep)
{
	auto inst = c->GetInv()[EQ::invslot::slotCursor];
	if (!inst) { c->Message(13, "Error: You need an item on your cursor for this command"); }
	auto item = inst->GetItem();
	if (!item)
		c->Message(Chat::Red, "Error: You need an item on your cursor for this command");
	else {

		EQ::SayLinkEngine linker;
		linker.SetLinkType(EQ::saylink::SayLinkItemInst);
		linker.SetItemInst(inst);

		c->Message(0, "*** Item Info for [%s] ***", linker.GenerateLink().c_str());
		c->Message(Chat::White, "ID: %i Name: %s", item->ID, item->Name);
		c->Message(Chat::White, "  Lore: %s  ND: %i  NS: %i  Type: %i", item->Lore, item->NoDrop, item->NoRent, item->ItemClass);
		c->Message(Chat::White, "  IDF: %s  Size: %i  Weight: %i  icon_id: %i  Price: %i", item->IDFile, item->Size, item->Weight, item->Icon, item->Price);
		c->Message(Chat::White, "  QuantityType: %d (1 Normal, 2 Charges, 3 Stacked)", database.ItemQuantityType(inst->GetItem()->ID));
		if (c->Admin() >= 200)
			c->Message(Chat::White, "MinStatus: %i", item->MinStatus);
		if (item->ItemClass == EQ::item::ItemClassBook)
			c->Message(Chat::White, "  This item is a Book: %s", item->Filename);
		else if (item->ItemClass == EQ::item::ItemClassBag)
			c->Message(Chat::White, "  This item is a container with %i slots", item->BagSlots);
		else {
			c->Message(Chat::White, "  equipableSlots: %u equipable Classes: %u", item->Slots, item->Classes);
			c->Message(Chat::White, "  Magic: %i  SpellID: %i  Proc Level: %i DBCharges: %i  CurCharges: %i", item->Magic, item->Click.Effect, item->Click.Level, item->MaxCharges, inst->GetCharges());
			c->Message(Chat::White, "  EffectType: 0x%02x  CastTime: %.2f", (uint8)item->Click.Type, (double)item->CastTime / 1000);
			c->Message(Chat::White, "  Material: 0x%02x  Color: 0x%08x  Skill: %i", item->Material, item->Color, item->ItemType);
			c->Message(Chat::White, " Required level: %i Required skill: %i Recommended level:%i", item->ReqLevel, item->RecSkill, item->RecLevel);
			c->Message(Chat::White, " Skill mod: %i percent: %i", item->SkillModType, item->SkillModValue);
			c->Message(Chat::White, " BaneRace: %i BaneBody: %i BaneDMG: %i", item->BaneDmgRace, item->BaneDmgBody, item->BaneDmgAmt);
		}
	}
}

