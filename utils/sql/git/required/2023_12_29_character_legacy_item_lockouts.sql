ALTER TABLE `character_legacy_items` 
ADD COLUMN `expire_time` bigint(24) UNSIGNED NOT NULL DEFAULT 0 AFTER `item_id`;

ALTER TABLE `lootdrop_entries` 
ADD COLUMN `item_loot_lockout_timer` int(11) UNSIGNED NOT NULL DEFAULT 0 AFTER `min_looter_level`;