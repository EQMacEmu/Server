ALTER TABLE `spawn2` 
MODIFY COLUMN `raid_target_spawnpoint` tinyint(3) UNSIGNED NOT NULL DEFAULT 0 AFTER `max_expansion`;