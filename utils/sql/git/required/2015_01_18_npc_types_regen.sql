ALTER TABLE `npc_types` ADD `combat_hp_regen` INT(11) NOT NULL DEFAULT 0 AFTER `hp_regen_rate`;
ALTER TABLE `npc_types` ADD `combat_mana_regen` INT(11) NOT NULL DEFAULT 0 AFTER `mana_regen_rate`;