#ifndef CODE_LOOT_H
#define CODE_LOOT_H

#include <list>
#include <string>
#include "types.h"

struct LootItem {
	uint32  item_id;
	int16   equip_slot;
	int8    charges;
	uint16  lootslot;
	uint8   min_level;
	uint8   max_level;
	uint8   quest;
	uint8   pet;
	bool    forced;
};

typedef std::list<LootItem*> LootItems;

#endif
