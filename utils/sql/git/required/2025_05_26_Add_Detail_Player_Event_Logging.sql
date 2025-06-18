ALTER TABLE `player_event_log_settings`
	ADD COLUMN `etl_enabled` TINYINT(1) UNSIGNED NOT NULL DEFAULT '0' AFTER `discord_webhook_id`;
ALTER TABLE `player_event_logs`
	ADD COLUMN `etl_table_id` BIGINT(20) NOT NULL DEFAULT '0' AFTER `event_data`;
UPDATE `player_event_log_settings` SET `etl_enabled` = 1 WHERE `id` = 14;
CREATE TABLE `player_event_loot_items` (
	`id` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
	`item_id` INT(10) UNSIGNED NULL DEFAULT NULL,
	`item_name` VARCHAR(64) NULL DEFAULT NULL COLLATE 'latin1_swedish_ci',
	`charges` INT(11) NULL DEFAULT NULL,
	`npc_id` INT(10) UNSIGNED NULL DEFAULT NULL,
	`corpse_name` VARCHAR(64) NULL DEFAULT NULL COLLATE 'latin1_swedish_ci',
	`created_at` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `item_id_npc_id` (`item_id`, `npc_id`) USING BTREE,
	INDEX `created_at` (`created_at`) USING BTREE
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
AUTO_INCREMENT=1;
UPDATE `player_event_log_settings` SET `etl_enabled` = 1 WHERE `id` = 16;
CREATE TABLE `player_event_merchant_sell` (
	`id` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
	`npc_id` INT(10) UNSIGNED NULL DEFAULT '0',
	`merchant_name` VARCHAR(64) NULL DEFAULT NULL COLLATE 'latin1_swedish_ci',
	`merchant_type` INT(10) UNSIGNED NULL DEFAULT '0',
	`item_id` INT(10) UNSIGNED NULL DEFAULT '0',
	`item_name` VARCHAR(64) NULL DEFAULT NULL COLLATE 'latin1_swedish_ci',
	`charges` INT(11) NULL DEFAULT '0',
	`cost` INT(10) UNSIGNED NULL DEFAULT '0',
	`player_money_balance` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`created_at` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `item_id_npc_id` (`item_id`, `npc_id`) USING BTREE,
	INDEX `created_at` (`created_at`) USING BTREE
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
AUTO_INCREMENT=1;
UPDATE `player_event_log_settings` SET `etl_enabled` = 1 WHERE `id` = 15;
CREATE TABLE `player_event_merchant_purchase` (
	`id` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
	`npc_id` INT(10) UNSIGNED NULL DEFAULT '0',
	`merchant_name` VARCHAR(64) NULL DEFAULT NULL COLLATE 'latin1_swedish_ci',
	`merchant_type` INT(10) UNSIGNED NULL DEFAULT '0',
	`item_id` INT(10) UNSIGNED NULL DEFAULT '0',
	`item_name` VARCHAR(64) NULL DEFAULT NULL COLLATE 'latin1_swedish_ci',
	`charges` INT(11) NULL DEFAULT '0',
	`cost` INT(10) UNSIGNED NULL DEFAULT '0',
	`player_money_balance` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`created_at` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `item_id_npc_id` (`item_id`, `npc_id`) USING BTREE,
	INDEX `created_at` (`created_at`) USING BTREE
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
AUTO_INCREMENT=1;
UPDATE `player_event_log_settings` SET `etl_enabled` = 1 WHERE `id` = 22;
CREATE TABLE `player_event_npc_handin` (
	`id` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
	`npc_id` INT(10) UNSIGNED NULL DEFAULT '0',
	`npc_name` VARCHAR(64) NULL DEFAULT NULL COLLATE 'latin1_swedish_ci',
	`handin_copper` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`handin_silver` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`handin_gold` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`handin_platinum` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`return_copper` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`return_silver` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`return_gold` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`return_platinum` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`is_quest_handin` TINYINT(3) UNSIGNED NULL DEFAULT '0',
	`created_at` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `npc_id_is_quest_handin` (`npc_id`, `is_quest_handin`) USING BTREE,
	INDEX `created_at` (`created_at`) USING BTREE
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
AUTO_INCREMENT=1;
CREATE TABLE `player_event_npc_handin_entries` (
	`id` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
	`player_event_npc_handin_id` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0',
	`type` INT(10) UNSIGNED NULL DEFAULT NULL,
	`item_id` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`charges` INT(11) NOT NULL DEFAULT '0',
	`created_at` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `type_item_id` (`type`, `item_id`) USING BTREE,
	INDEX `player_event_npc_handin_id` (`player_event_npc_handin_id`) USING BTREE,
	INDEX `created_at` (`created_at`) USING BTREE
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
AUTO_INCREMENT=1;
UPDATE `player_event_log_settings` SET `etl_enabled` = 1 WHERE `id` = 27;
CREATE TABLE `player_event_trade` (
	`id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
	`char1_id` INT(10) UNSIGNED NULL DEFAULT '0',
	`char2_id` INT(10) UNSIGNED NULL DEFAULT '0',
	`char1_copper` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`char1_silver` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`char1_gold` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`char1_platinum` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`char2_copper` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`char2_silver` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`char2_gold` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`char2_platinum` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`created_at` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `char1_id_char2_id` (`char1_id`, `char2_id`) USING BTREE,
	INDEX `created_at` (`created_at`) USING BTREE
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
AUTO_INCREMENT=1;
CREATE TABLE `player_event_trade_entries` (
	`id` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
	`player_event_trade_id` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`char_id` INT(10) UNSIGNED NULL DEFAULT '0',
	`slot` SMALLINT(6) NULL DEFAULT '0',
	`item_id` INT(10) UNSIGNED NULL DEFAULT '0',
	`charges` SMALLINT(6) NULL DEFAULT '0',
	`in_bag` TINYINT(4) NULL DEFAULT '0',
	`created_at` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `player_event_trade_id` (`player_event_trade_id`) USING BTREE,
	INDEX `created_at` (`created_at`) USING BTREE
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
AUTO_INCREMENT=1;
UPDATE `player_event_log_settings` SET `etl_enabled` = 0 WHERE `id` = 54;
CREATE TABLE `player_event_speech` (
	`id` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
	`to_char_id` VARCHAR(64) NULL DEFAULT NULL COLLATE 'latin1_swedish_ci',
	`from_char_id` VARCHAR(64) NULL DEFAULT NULL COLLATE 'latin1_swedish_ci',
	`guild_id` INT(10) UNSIGNED NULL DEFAULT '0',
	`type` INT(10) UNSIGNED NULL DEFAULT '0',
	`min_status` INT(10) UNSIGNED NULL DEFAULT '0',
	`message` LONGTEXT NULL DEFAULT NULL COLLATE 'latin1_swedish_ci',
	`created_at` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `to_char_id_from_char_id` (`to_char_id`, `from_char_id`) USING BTREE,
	INDEX `created_at` (`created_at`) USING BTREE
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
AUTO_INCREMENT=1;
UPDATE `player_event_log_settings` SET `etl_enabled` = 1 WHERE `id` = 44;
CREATE TABLE `player_event_killed_npc` (
	`id` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
	`npc_id` INT(10) UNSIGNED NULL DEFAULT '0',
	`npc_name` VARCHAR(64) NULL DEFAULT NULL COLLATE 'latin1_swedish_ci',
	`combat_time_seconds` INT(10) UNSIGNED NULL DEFAULT '0',
	`total_damage_per_second_taken` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`total_heal_per_second_taken` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`created_at` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `npc_id` (`npc_id`) USING BTREE,
	INDEX `created_at` (`created_at`) USING BTREE
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB;
UPDATE `player_event_log_settings` SET `etl_enabled` = 1 WHERE `id` = 45;
CREATE TABLE `player_event_killed_named_npc` (
	`id` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
	`npc_id` INT(10) UNSIGNED NULL DEFAULT '0',
	`npc_name` VARCHAR(64) NULL DEFAULT NULL COLLATE 'latin1_swedish_ci',
	`combat_time_seconds` INT(10) UNSIGNED NULL DEFAULT '0',
	`total_damage_per_second_taken` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`total_heal_per_second_taken` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`created_at` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `npc_id` (`npc_id`) USING BTREE,
	INDEX `created_at` (`created_at`) USING BTREE
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB;
UPDATE `player_event_log_settings` SET `etl_enabled` = 1 WHERE `id` = 46;
CREATE TABLE `player_event_killed_raid_npc` (
	`id` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
	`npc_id` INT(10) UNSIGNED NULL DEFAULT '0',
	`npc_name` VARCHAR(64) NULL DEFAULT NULL COLLATE 'latin1_swedish_ci',
	`combat_time_seconds` INT(10) UNSIGNED NULL DEFAULT '0',
	`total_damage_per_second_taken` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`total_heal_per_second_taken` BIGINT(20) UNSIGNED NULL DEFAULT '0',
	`created_at` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `npc_id` (`npc_id`) USING BTREE,
	INDEX `created_at` (`created_at`) USING BTREE
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB;
UPDATE `player_event_log_settings` SET `etl_enabled` = 1 WHERE `id` = 4;
CREATE TABLE `player_event_aa_purchase` (
	`id` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
	`aa_ability_id` INT(11) NULL DEFAULT '0',
	`cost` INT(11) NULL DEFAULT '0',
	`previous_id` INT(11) NULL DEFAULT '0',
	`next_id` INT(11) NULL DEFAULT '0',
	`created_at` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `created_at` (`created_at`) USING BTREE
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
;
