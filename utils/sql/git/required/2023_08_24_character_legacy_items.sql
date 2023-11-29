CREATE TABLE `character_legacy_items`  (
  `character_id` int(11) NOT NULL,
  `item_id` int(11) NOT NULL,
  PRIMARY KEY (`character_id`, `item_id`) USING BTREE,
  INDEX `Index_CharacterID`(`character_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

ALTER TABLE `lootdrop_entries` 
ADD COLUMN `min_looter_level` int(3) UNSIGNED NOT NULL DEFAULT 0 AFTER `max_expansion`;