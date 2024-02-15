ALTER TABLE `character_data` 
ADD COLUMN `e_temp_last_name` varchar(64) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT '' AFTER `e_zone_guild_id`;

ALTER TABLE `character_data` 
ADD COLUMN `e_married_character_id` int(11) NOT NULL DEFAULT 0 AFTER `e_temp_last_name`;