ALTER TABLE `items` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `soulbound`;
ALTER TABLE `merchantlist` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `quantity`;
ALTER TABLE `spawn2` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `force_z`;
ALTER TABLE `spawnentry` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `maxtime`;
ALTER TABLE `object` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `incline`;
ALTER TABLE `doors` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `can_open`;
ALTER TABLE `lootdrop_entries` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `disabled_chance`;
ALTER TABLE `ground_spawns` ADD COLUMN `expansion` int(11) NOT NULL DEFAULT 0 AFTER `respawn_timer`;
REPLACE INTO command_settings`(`command`, `access`) VALUES ('setnpcexpansion', 100);