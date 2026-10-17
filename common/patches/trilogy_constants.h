#ifndef TRILOGY_CONSTANTS_H_
#define TRILOGY_CONSTANTS_H_

#include "../types.h"

namespace Trilogy {
	namespace maps {
		typedef enum : int16 {
			MapPossessions = 0,
			MapBank,
			MapSharedBank,
			MapTrade,
			MapWorld,
			MapLimbo,
			MapTribute,
			MapTrophyTribute,
			MapGuildTribute,
			MapMerchant,
			MapDeleted,
			MapCorpse,
			MapBazaar,
			MapInspect,
			MapRealEstate,
			MapViewMODPC,
			MapViewMODBank,
			MapViewMODSharedBank,
			MapViewMODLimbo,
			MapAltStorage,
			MapArchived,
			MapMail,
			MapGuildTrophyTribute,
			MapOther,
			_MapCount
		} InventoryMaps;
	}

	namespace slots {
		typedef enum : int16 {
			MainCursor = 0,
			MainEar1,
			MainHead,
			MainFace,
			MainEar2,
			MainNeck,
			MainShoulders,
			MainArms,
			MainBack,
			MainWrist1,
			MainWrist2,
			MainRange,
			MainHands,
			MainPrimary,
			MainSecondary,
			MainFinger1,
			MainFinger2,
			MainChest,
			MainLegs,
			MainFeet,
			MainWaist,
			MainAmmo,
			MainGeneral1,
			MainGeneral2,
			MainGeneral3,
			MainGeneral4,
			MainGeneral5,
			MainGeneral6,
			MainGeneral7,
			MainGeneral8,
			_MainCount,
			_MainEquipmentBegin = MainEar1,
			_MainEquipmentEnd = MainAmmo,
			_MainEquipmentCount = (_MainEquipmentEnd - _MainEquipmentBegin + 1),
			_MainGeneralBegin = MainGeneral1,
			_MainGeneralEnd = MainGeneral8,
			_MainGeneralCount = (_MainGeneralEnd - _MainGeneralBegin + 1)
		} EquipmentSlots;
	}

	namespace consts {
		static const uint16	MAP_POSSESSIONS_SIZE = slots::_MainCount;
		static const uint16 MAP_BANK_SIZE = 8;
		static const uint16 MAP_SHARED_BANK_SIZE = 2;
		static const uint16 MAP_TRADE_SIZE = 8;
		static const uint16 MAP_WORLD_SIZE = 10;
		static const uint16 MAP_LIMBO_SIZE = 36;
		static const uint16 MAP_TRIBUTE_SIZE = 0;
		static const uint16 MAP_TROPHY_TRIBUTE_SIZE = 0;
		static const uint16 MAP_GUILD_TRIBUTE_SIZE = 0;
		static const uint16 MAP_MERCHANT_SIZE = 0;
		static const uint16 MAP_DELETED_SIZE = 0;
		static const uint16 MAP_CORPSE_SIZE = slots::_MainCount;
		static const uint16 MAP_BAZAAR_SIZE = 80;
		static const uint16 MAP_INSPECT_SIZE = slots::_MainEquipmentCount;
		static const uint16 MAP_REAL_ESTATE_SIZE = 0;
		static const uint16 MAP_VIEW_MOD_PC_SIZE = MAP_POSSESSIONS_SIZE;
		static const uint16 MAP_VIEW_MOD_BANK_SIZE = MAP_BANK_SIZE;
		static const uint16 MAP_VIEW_MOD_SHARED_BANK_SIZE = MAP_SHARED_BANK_SIZE;
		static const uint16 MAP_VIEW_MOD_LIMBO_SIZE = MAP_LIMBO_SIZE;
		static const uint16 MAP_ALT_STORAGE_SIZE = 0;
		static const uint16 MAP_ARCHIVED_SIZE = 0;
		static const uint16 MAP_MAIL_SIZE = 0;
		static const uint16 MAP_GUILD_TROPHY_TRIBUTE_SIZE = 0;
		static const uint16 MAP_KRONO_SIZE = NOT_USED;
		static const uint16 MAP_OTHER_SIZE = 0;

		static const int16 EQUIPMENT_BEGIN = slots::MainEar1;
		static const int16 EQUIPMENT_END = slots::MainAmmo;
		static const uint16 EQUIPMENT_SIZE = slots::_MainEquipmentCount;

		static const int16 GENERAL_BEGIN = slots::MainGeneral1;
		static const int16 GENERAL_END = slots::MainGeneral8;
		static const uint16 GENERAL_SIZE = slots::_MainGeneralCount;
		static const int16 GENERAL_BAGS_BEGIN = 250;
		static const int16 GENERAL_BAGS_END_OFFSET = 79;
		static const int16 GENERAL_BAGS_END = GENERAL_BAGS_BEGIN + GENERAL_BAGS_END_OFFSET;

		static const int16 CURSOR = slots::MainCursor;
		static const int16 CURSOR_BAG_BEGIN = 330;
		static const int16 CURSOR_BAG_END_OFFSET = 9;
		static const int16 CURSOR_BAG_END = CURSOR_BAG_BEGIN + CURSOR_BAG_END_OFFSET;

		static const int16 BANK_BEGIN = 2000;
		static const int16 BANK_END = 2007;
		static const int16 BANK_BAGS_BEGIN = 2031;
		static const int16 BANK_BAGS_END_OFFSET = 159;
		static const int16 BANK_BAGS_END = BANK_BAGS_BEGIN + BANK_BAGS_END_OFFSET;

		static const int16 SHARED_BANK_BEGIN = 2500;
		static const int16 SHARED_BANK_END = 2501;
		static const int16 SHARED_BANK_BAGS_BEGIN = 2531;
		static const int16 SHARED_BANK_BAGS_END_OFFSET = 19;
		static const int16 SHARED_BANK_BAGS_END = SHARED_BANK_BAGS_BEGIN + SHARED_BANK_BAGS_END_OFFSET;

		static const int16 TRADE_BEGIN = 3000;
		static const int16 TRADE_END = 3007;
		static const int16 TRADE_NPC_END = 3003;
		static const int16 TRADE_BAGS_BEGIN = 3030;
		static const int16 TRADE_BAGS_END_OFFSET = 79;
		static const int16 TRADE_BAGS_END = TRADE_BAGS_BEGIN + TRADE_BAGS_END_OFFSET;

		static const int16 WORLD_BEGIN = 4000;
		static const int16 WORLD_END = 4009;

		static const int16 TRIBUTE_BEGIN = 400;
		static const int16 TRIBUTE_END = 404;

		static const int16 CORPSE_BEGIN = slots::MainGeneral1;
		static const int16 CORPSE_END = slots::MainGeneral1 + slots::MainCursor;

		static const uint16 ITEM_COMMON_SIZE = 5;
		static const uint16 ITEM_CONTAINER_SIZE = 10;

		static const uint32 BANDOLIERS_COUNT = 4;
		static const uint32 BANDOLIER_SIZE = 4;
		static const uint32 POTION_BELT_SIZE = 4;
	}

	namespace limits {
		static const bool ALLOWS_EMPTY_BAG_IN_BAG = false;
		static const bool ALLOWS_CLICK_CAST_FROM_BAG = false;
		static const bool COIN_HAS_WEIGHT = true;
	}

} // end namespace Trilogy

#endif /* TRILOGY_CONSTANTS_H_ */
