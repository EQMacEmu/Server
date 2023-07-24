ALTER TABLE `npc_types`	ADD COLUMN `engage_notice` TINYINT(2) NOT NULL DEFAULT '0' AFTER `greed`;

UPDATE `npc_types` SET `engage_notice`='1' WHERE  `name`="Lady_Vox";
UPDATE `npc_types` SET `engage_notice`='1' WHERE  `name`="Lord_Nagafen";
UPDATE `npc_types` SET `engage_notice`='1' WHERE  `name`="Dread";
UPDATE `npc_types` SET `engage_notice`='1' WHERE  `name`="Fright";
UPDATE `npc_types` SET `engage_notice`='1' WHERE  `name`="Terror";