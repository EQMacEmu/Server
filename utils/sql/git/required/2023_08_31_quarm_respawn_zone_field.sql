ALTER TABLE `zone` 
ADD COLUMN `reducedspawntimers` tinyint(3) UNSIGNED NOT NULL DEFAULT 0 AFTER `max_z`;