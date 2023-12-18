ALTER TABLE `npc_types` 
ADD COLUMN `loot_lockout` int(11) UNSIGNED NOT NULL DEFAULT 0 AFTER `flymode`;


CREATE TABLE `character_loot_lockouts`  (
  `character_id` int(11) NOT NULL,
  `npctype_id` int(11) NOT NULL,
  `expiry` bigint(24) NOT NULL,
  PRIMARY KEY (`character_id`, `npctype_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;


ALTER TABLE `character_loot_lockouts` 
ADD COLUMN `npc_name` varchar(64) NOT NULL DEFAULT ' ' AFTER `expiry`