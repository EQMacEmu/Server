ALTER TABLE `items` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `soulbound`;
ALTER TABLE `merchantlist` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `quantity`;
ALTER TABLE `spawn2` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `force_z`;
ALTER TABLE `spawnentry` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `maxtime`;
ALTER TABLE `object` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `incline`;
ALTER TABLE `doors` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `can_open`;
ALTER TABLE `lootdrop_entries` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `disabled_chance`;
ALTER TABLE `ground_spawns` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `respawn_timer`;
REPLACE INTO command_settings`(`command`, `access`) VALUES ('setnpcexpansion', 100);

ALTER TABLE `items` CHANGE COLUMN `expansion` `min_expansion` float NOT NULL DEFAULT 0
ALTER TABLE `merchantlist` CHANGE COLUMN `expansion` `min_expansion` float NOT NULL DEFAULT 0
ALTER TABLE `spawn2` CHANGE COLUMN `expansion` `min_expansion` float NOT NULL DEFAULT 0
ALTER TABLE `spawnentry` CHANGE COLUMN `expansion` `min_expansion` float NOT NULL DEFAULT 0
ALTER TABLE `object` CHANGE COLUMN `expansion` `min_expansion` float NOT NULL DEFAULT 0
ALTER TABLE `doors` CHANGE COLUMN `expansion` `min_expansion` float NOT NULL DEFAULT 0
ALTER TABLE `lootdrop_entries` CHANGE COLUMN `expansion` `min_expansion` float NOT NULL DEFAULT 0
ALTER TABLE `ground_spawns` CHANGE COLUMN `expansion` `min_expansion` float NOT NULL DEFAULT 0

ALTER TABLE `items` ADD COLUMN `max_expansion` float NOT NULL DEFAULT 0 AFTER `min_expansion`;
ALTER TABLE `merchantlist` ADD COLUMN `max_expansion` float NOT NULL DEFAULT 0 AFTER `min_expansion`;
ALTER TABLE `spawn2` ADD COLUMN `max_expansion` float NOT NULL DEFAULT 0 AFTER `min_expansion`;
ALTER TABLE `spawnentry` ADD COLUMN `max_expansion` float NOT NULL DEFAULT 0 AFTER `min_expansion`;
ALTER TABLE `object` ADD COLUMN `max_expansion` float NOT NULL DEFAULT 0 AFTER `min_expansion`;
ALTER TABLE `doors` ADD COLUMN `max_expansion` float NOT NULL DEFAULT 0 AFTER `min_expansion`;
ALTER TABLE `lootdrop_entries` ADD COLUMN `max_expansion` float NOT NULL DEFAULT 0 AFTER `min_expansion`;
ALTER TABLE `ground_spawns` ADD COLUMN `max_expansion` float NOT NULL DEFAULT 0 AFTER `min_expansion`;