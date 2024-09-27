ALTER TABLE `skill_caps`
CHANGE COLUMN `skillID` `skill_id` tinyint(3) UNSIGNED NOT NULL DEFAULT 0 FIRST,
CHANGE COLUMN `class` `class_id` tinyint(3) UNSIGNED NOT NULL DEFAULT 0 AFTER `skill_id`,
ADD COLUMN `id` int(3) UNSIGNED NOT NULL AUTO_INCREMENT FIRST,
DROP PRIMARY KEY,
ADD PRIMARY KEY (`id`) USING BTREE,
ADD INDEX `level_skill_cap`(`skill_id`, `class_id`, `level`, `cap`);