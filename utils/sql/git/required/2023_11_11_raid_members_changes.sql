ALTER TABLE `raid_members` 
ADD COLUMN `guild_id` int(11) UNSIGNED NOT NULL DEFAULT 4294967295 AFTER `islooter`,
ADD COLUMN `is_officer` tinyint(1) UNSIGNED NOT NULL DEFAULT 0 AFTER `guildid`;