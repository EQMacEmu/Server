#include "../client.h"
#include "../object.h"

void command_peekinv(Client *c, const Seperator *sep)
{
	enum {
		peekNone = 0x00,
		peekWorn = 0x01,
		peekInv = 0x02,
		peekCursor = 0x04,
		peekBank = 0x08,
		peekTrade = 0x10,
		peekWorld = 0x20,
		peekOutOfScope = (peekWorld * 2)
	};

	if (!c->GetTarget() || !c->GetTarget()->IsClient()) {
		c->Message(Chat::White, "You must have a PC target selected for this command");
		return;
	}

	int scopeWhere = peekNone;

	if (strcasecmp(sep->arg[1], "all") == 0) { scopeWhere = (peekOutOfScope); }
	else if (strcasecmp(sep->arg[1], "worn") == 0) { scopeWhere |= peekWorn; }
	else if (strcasecmp(sep->arg[1], "inv") == 0) { scopeWhere |= peekInv; }
	else if (strcasecmp(sep->arg[1], "cursor") == 0) { scopeWhere |= peekCursor; }
	else if (strcasecmp(sep->arg[1], "bank") == 0) { scopeWhere |= peekBank; }
	else if (strcasecmp(sep->arg[1], "trade") == 0) { scopeWhere |= peekTrade; }
	else if (strcasecmp(sep->arg[1], "world") == 0) { scopeWhere |= peekWorld; }

	if (scopeWhere == 0) {
		c->Message(Chat::White, "Usage: #peekinv [worn|inv|cursor|trib|bank|trade|world|all]");
		c->Message(Chat::White, "  Displays a portion of the targeted user's inventory");
		c->Message(Chat::White, "  Caution: 'all' is a lot of information!");
		return;
	}

	Client* targetClient = c->GetTarget()->CastToClient();
	const EQ::ItemInstance* inst_main = nullptr;
	const EQ::ItemInstance* inst_sub = nullptr;
	const EQ::ItemData* item_data = nullptr;
	
	EQ::SayLinkEngine linker;
	linker.SetLinkType(EQ::saylink::SayLinkItemInst);

	c->Message(Chat::White, fmt::format("Displaying inventory for {}...", targetClient->GetName()).c_str());
	
	// worn
	for (int16 indexMain = EQ::invslot::EQUIPMENT_BEGIN; (scopeWhere & peekWorn) && (indexMain <= EQ::invslot::EQUIPMENT_END); ++indexMain) {
		inst_main = targetClient->GetInv().GetItem(indexMain);
		item_data = (inst_main == nullptr) ? nullptr : inst_main->GetItem();
		linker.SetItemInst(inst_main);

				c->Message((item_data == nullptr), "WornSlot: %i, Item: %i (%s), Charges: %i",
			indexMain, ((item_data == nullptr) ? 0 : item_data->ID), linker.GenerateLink().c_str(), ((item_data == nullptr) ? 0 : inst_main->GetCharges()));
	}

	// inv
	for (int16 indexMain = EQ::invslot::GENERAL_BEGIN; (scopeWhere & peekInv) && (indexMain <= EQ::invslot::GENERAL_END); ++indexMain) {
		inst_main = targetClient->GetInv().GetItem(indexMain);
		item_data = (inst_main == nullptr) ? nullptr : inst_main->GetItem();
		linker.SetItemInst(inst_main);

				c->Message((item_data == 0), fmt::format("InvSlot: {}, Item: {} ({}), Charges: {}",
			indexMain, ((item_data == 0) ? 0 : item_data->ID), linker.GenerateLink().c_str(), ((item_data == 0) ? 0 : inst_main->GetCharges())).c_str());

		for (uint8 indexSub = EQ::invbag::SLOT_BEGIN; inst_main && inst_main->IsClassBag() && (indexSub <= EQ::invbag::SLOT_END); ++indexSub) {
			inst_sub = inst_main->GetItem(indexSub);
			item_data = (inst_sub == nullptr) ? nullptr : inst_sub->GetItem();
			linker.SetItemInst(inst_sub);

			c->Message((item_data == nullptr), fmt::format("  InvBagSlot: {} (Slot #{}, Bag #{}), Item: {} ({}), Charges: {}",
				EQ::InventoryProfile::CalcSlotId(indexMain, indexSub), indexMain, indexSub, ((item_data == nullptr) ? 0 : item_data->ID), linker.GenerateLink().c_str(), ((item_data == nullptr) ? 0 : inst_sub->GetCharges())).c_str());

		}
	}
	// money
	if (scopeWhere & peekInv)
		c->Message(Chat::White, fmt::format("Carried Money: {} pp {} gp {} sp {} cp", targetClient->GetPP().platinum, targetClient->GetPP().gold, targetClient->GetPP().silver, targetClient->GetPP().copper).c_str());

	// cursor
	if (scopeWhere & peekCursor) {
		if (targetClient->GetInv().CursorEmpty()) {
			linker.SetItemInst(nullptr);

			c->Message(Chat::White, fmt::format("CursorSlot: {}, Item: {} ({}), Charges: {}",
				EQ::invslot::slotCursor, 0, linker.GenerateLink().c_str(), 0).c_str());
		}
		else {
			int cursorDepth = 0;
			for (auto it = targetClient->GetInv().cursor_cbegin(); (it != targetClient->GetInv().cursor_cend()); ++it, ++cursorDepth) {
				inst_main = *it;
				item_data = (inst_main == nullptr) ? nullptr : inst_main->GetItem();
				linker.SetItemInst(inst_main);

				int16 SlotCursor = cursorDepth;
				if(SlotCursor > 0)
					SlotCursor += EQ::invslot::CURSOR_QUEUE_BEGIN;

				c->Message((item_data == 0), fmt::format("CursorSlot: {}, Depth: {}, Item: {} ({}), Charges: {}",
					SlotCursor, cursorDepth, ((item_data == 0) ? 0 : item_data->ID), linker.GenerateLink().c_str(), ((item_data == 0) ? 0 : inst_main->GetCharges())).c_str());

				for (uint8 indexSub = EQ::invbag::SLOT_BEGIN; (cursorDepth == 0) && inst_main && inst_main->IsClassBag() && (indexSub <= EQ::invbag::SLOT_END); ++indexSub) {
					inst_sub = inst_main->GetItem(indexSub);
					item_data = (inst_sub == nullptr) ? nullptr : inst_sub->GetItem();
					linker.SetItemInst(inst_sub);

					c->Message((item_data == nullptr), fmt::format("  CursorBagSlot: {} (Slot #{}, Bag #{}), Item: {} ({}), Charges: {}",
						EQ::InventoryProfile::CalcSlotId(SlotCursor, indexSub), SlotCursor, indexSub, ((item_data == nullptr) ? 0 : item_data->ID), linker.GenerateLink().c_str(), ((item_data == nullptr) ? 0 : inst_sub->GetCharges())).c_str());

				}
			}
		}
		// money
		c->Message(Chat::White, fmt::format("Cursor Money: {} pp {} gp {} sp {} cp", targetClient->GetPP().platinum_cursor, targetClient->GetPP().gold_cursor, targetClient->GetPP().silver_cursor, targetClient->GetPP().copper_cursor).c_str());
	}

	// bank
	for (int16 indexMain = EQ::invslot::BANK_BEGIN; (scopeWhere & peekBank) && (indexMain <= EQ::invslot::BANK_END); ++indexMain) {
		inst_main = targetClient->GetInv().GetItem(indexMain);
		item_data = (inst_main == nullptr) ? nullptr : inst_main->GetItem();
		linker.SetItemInst(inst_main);

		c->Message((item_data == nullptr), fmt::format("BankSlot: {}, Item: {} ({}), Charges: {}",
			indexMain, ((item_data == nullptr) ? 0 : item_data->ID), linker.GenerateLink().c_str(), ((item_data == nullptr) ? 0 : inst_main->GetCharges())).c_str());

		for (uint8 indexSub = EQ::invbag::SLOT_BEGIN; inst_main && inst_main->IsClassBag() && (indexSub <= EQ::invbag::SLOT_END); ++indexSub) {
			inst_sub = inst_main->GetItem(indexSub);
			item_data = (inst_sub == nullptr) ? nullptr : inst_sub->GetItem();
			linker.SetItemInst(inst_sub);

			c->Message((item_data == nullptr), fmt::format("  BankBagSlot: {} (Slot #{}, Bag #{}), Item: {} ({}), Charges: {}",
				EQ::InventoryProfile::CalcSlotId(indexMain, indexSub), indexMain, indexSub, ((item_data == nullptr) ? 0 : item_data->ID), linker.GenerateLink().c_str(), ((item_data == nullptr) ? 0 : inst_sub->GetCharges())).c_str());
		}
	}
	// money
	if (scopeWhere & peekBank) 
		c->Message(Chat::White, fmt::format("Bank Money: {} pp {} gp {} sp {} cp", targetClient->GetPP().platinum_bank, targetClient->GetPP().gold_bank, targetClient->GetPP().silver_bank, targetClient->GetPP().copper_bank).c_str());


	// trade
	for (int16 indexMain = EQ::invslot::TRADE_BEGIN; (scopeWhere & peekTrade) && (indexMain <= EQ::invslot::TRADE_END); ++indexMain) {
		inst_main = targetClient->GetInv().GetItem(indexMain);
		item_data = (inst_main == nullptr) ? nullptr : inst_main->GetItem();
		linker.SetItemInst(inst_main);

		c->Message((item_data == nullptr), fmt::format("TradeSlot: {}, Item: {} ({}), Charges: {}",
			indexMain, ((item_data == nullptr) ? 0 : item_data->ID), linker.GenerateLink().c_str(), ((item_data == nullptr) ? 0 : inst_main->GetCharges())).c_str());

		for (uint8 indexSub = EQ::invbag::SLOT_BEGIN; inst_main && inst_main->IsClassBag() && (indexSub <= EQ::invbag::SLOT_END); ++indexSub) {
			inst_sub = inst_main->GetItem(indexSub);
			item_data = (inst_sub == nullptr) ? nullptr : inst_sub->GetItem();
			linker.SetItemInst(inst_sub);

			c->Message((item_data == nullptr), fmt::format("  TradeBagSlot: {} (Slot #{}, Bag #{}), Item: {} ({}), Charges: {}",
				EQ::InventoryProfile::CalcSlotId(indexMain, indexSub), indexMain, indexSub, ((item_data == nullptr) ? 0 : item_data->ID), linker.GenerateLink().c_str(), ((item_data == nullptr) ? 0 : inst_sub->GetCharges())).c_str());
		}
	}

	// world
	if (scopeWhere & peekWorld) {
		Object* objectTradeskill = targetClient->GetTradeskillObject();

		if (objectTradeskill == nullptr) {
			c->Message(Chat::DimGray, "No world tradeskill object selected...");
		}
		else {
			c->Message(Chat::White, fmt::format("[WorldObject DBID: {} (entityid: {})]", objectTradeskill->GetDBID(), objectTradeskill->GetID()).c_str());

			for (int16 indexMain = EQ::invslot::SLOT_BEGIN; indexMain < EQ::invtype::WORLD_SIZE; ++indexMain) {
				inst_main = objectTradeskill->GetItem(indexMain);
				item_data = (inst_main == nullptr) ? nullptr : inst_main->GetItem();
				linker.SetItemInst(inst_main);

				c->Message((item_data == nullptr), fmt::format("WorldSlot: {}, Item: {} ({}), Charges: {}",
					(EQ::invslot::WORLD_BEGIN + indexMain), ((item_data == nullptr) ? 0 : item_data->ID), linker.GenerateLink().c_str(), ((item_data == 0) ? 0 : inst_main->GetCharges())).c_str());

				for (uint8 indexSub = EQ::invbag::SLOT_BEGIN; inst_main && inst_main->IsClassBag() && (indexSub <= EQ::invbag::SLOT_END); ++indexSub) {
					inst_sub = inst_main->GetItem(indexSub);
					item_data = (inst_sub == nullptr) ? nullptr : inst_sub->GetItem();
					linker.SetItemInst(inst_sub);

					c->Message((item_data == nullptr), fmt::format("  WorldBagSlot: {} (Slot #{}, Bag #{}), Item: {} ({}), Charges: {}",
						INVALID_INDEX, indexMain, indexSub, ((item_data == 0) ? 0 : item_data->ID), linker.GenerateLink().c_str(), ((item_data == nullptr) ? 0 : inst_sub->GetCharges())).c_str());

				}
			}
		}
	}
}

