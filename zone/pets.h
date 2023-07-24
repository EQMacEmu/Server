/*
 * vim: set noexpandtab tabstop=4 shiftwidth=4 syntax=cpp:
*/
#ifndef PETS_H
#define PETS_H

#define PET_BACKOFF			1
#define PET_GETLOST			2
#define PET_HEALTHREPORT	4
#define PET_GUARDHERE		5
#define PET_GUARDME			6
#define PET_ATTACK			7
#define PET_FOLLOWME		8
#define PET_SITDOWN			9
#define PET_STANDUP			10
#define PET_TAUNT			11
#define PET_HOLD			12
#define PET_NOTAUNT			14
#define PET_LEADER			16
#define	PET_SLUMBER			17

typedef enum {
	PET_FOCUS_TIME = 20508,				// Symbol of Ancient Summoning
	PET_FOCUS_VT = 28144,				// Gloves of Dark Summoning
	PET_FOCUS_HATE_NECRO = 11571,		// Encyclopedia Necrotheurgia
	PET_FOCUS_STAFF_WATER = 11569,		// Staff of Elemental Mastery: Water
	PET_FOCUS_STAFF_EARTH = 11567,		// Staff of Elemental Mastery: Earth
	PET_FOCUS_STAFF_FIRE = 11566,		// Staff of Elemental Mastery: Fire
	PET_FOCUS_STAFF_AIR = 11568,		// Staff of Elemental Mastery: Air
	PET_FOCUS_SOLTEMPLE_AIR = 6360,		// Broom of Trilon
	PET_FOCUS_SOLTEMPLE_EARTH = 6361,	// Shovel of Ponz
	PET_FOCUS_SOLTEMPLE_FIRE = 6362,	// Torch of Alna
	PET_FOCUS_SOLTEMPLE_WATER = 6363,	// Stein of Ulissa
} FocusItemIds;

typedef enum {
	ALL,
	FIRE,
	WATER,
	AIR,
	EARTH,
	NECRO,
	BEAST,
	NONE
} FocusPetType;

struct FocusPetItem {
	int item_id;
	int power;
	int max_level;
	int min_level;
	int pet_type;
};

#define FocusPetItemSize 11

class Mob;
struct NPCType;

class Pet : public NPC {
public:
	Pet(NPCType *type_data, Mob *owner, PetType type, uint16 spell_id, int16 power, int16 focusItemId);
	static const FocusPetItem focusItems[11];
	static FocusPetType GetPetItemPetTypeFromSpellId(uint16 spell_id);
};

#endif
