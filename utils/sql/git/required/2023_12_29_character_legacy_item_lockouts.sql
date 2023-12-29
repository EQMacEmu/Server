ALTER TABLE `character_legacy_items` 
ADD COLUMN `expire_time` bigint(24) UNSIGNED NOT NULL DEFAULT 0 AFTER `item_id`;