ALTER TABLE `doors` 
ADD COLUMN `guildzonedoor` tinyint(3) NOT NULL DEFAULT 0 AFTER `max_expansion`;

ALTER TABLE `respawn_times` 
ADD COLUMN `guild_id` int(11) UNSIGNED NOT NULL DEFAULT 4294967295 AFTER `duration`,
DROP PRIMARY KEY,
ADD PRIMARY KEY (`id`, `guild_id`) USING BTREE;

ALTER TABLE `character_data` 
ADD COLUMN `e_zone_guild_id` int(11) UNSIGNED NOT NULL DEFAULT 4294967295 AFTER `e_betabuff_gear_flag`

ALTER TABLE `character_corpses` 
ADD COLUMN `zone_guild_id` int(11) UNSIGNED NOT NULL DEFAULT 4294967295 AFTER `rezzable`;

ALTER TABLE `character_corpses_backup` 
ADD COLUMN `zone_guild_id` int(11) UNSIGNED NOT NULL DEFAULT 4294967295 AFTER `rezzable`;