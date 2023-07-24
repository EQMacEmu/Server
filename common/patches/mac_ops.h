D(OP_SendLoginInfo)
E(OP_PlayerProfile)
E(OP_NewZone)
E(OP_NewSpawn)
E(OP_ZoneSpawns)
E(OP_SpecialMesg)
E(OP_CancelTrade)
E(OP_ItemPacket)
E(OP_TradeItemPacket)
E(OP_ItemLinkResponse)
E(OP_CharInventory)
D(OP_MoveItem)
E(OP_MoveItem)
E(OP_DeleteCharge)
D(OP_DeleteCharge)
E(OP_HPUpdate)
E(OP_MobHealth)
E(OP_ShopRequest)
D(OP_ShopRequest)
E(OP_ShopInventoryPacket)
D(OP_ShopPlayerBuy)
E(OP_ShopPlayerBuy)
E(OP_ShopDelItem)
D(OP_ShopPlayerSell)
E(OP_ShopPlayerSell)
E(OP_AAAction)
E(OP_PickPocket);

//Below are invalid opcodes ONLY
E(OP_Unknown);
E(OP_RaidJoin);
#undef E
#undef D
