ALTER TABLE `character_data` 
ADD COLUMN `e_self_found` tinyint(4) NOT NULL DEFAULT 0 AFTER `showhelm`,
ADD COLUMN `e_solo_only` tinyint(4) NOT NULL DEFAULT 0 AFTER `e_self_found`,
ADD COLUMN `e_hardcore` tinyint(4) NOT NULL DEFAULT 0 AFTER `e_solo_only`;

ALTER TABLE  `character_data` 
ADD COLUMN `e_hardcore_death_time` bigint(22) NOT NULL DEFAULT 0 AFTER `e_hardcore`;