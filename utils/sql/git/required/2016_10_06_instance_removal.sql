alter table character_data drop column `zone_instance`;
alter table character_bind drop column `instance_id`;
alter table character_corpses drop column `instance_id`;
alter table character_corpses_backup drop column `instance_id`;

delete from spawn_condition_values where zone = "potimeb";
alter table spawn_condition_values drop column `instance_id`;
alter table respawn_times drop column `instance_id`;
alter table object drop column `version`;
alter table doors drop column `dest_instance`;
alter table doors drop column `version`;
alter table npc_types drop column `version`;
alter table npc_types drop column `npcspecialattks`;
alter table spawn2 drop column `version`;
alter table traps drop column `version`;
alter table zone drop column `version`;
alter table zone drop column `insttype`;
alter table zone_points drop column `version`;
alter table zone_points drop column `zoneinst`;
alter table zone_points drop column `target_instance`;
alter table ground_spawns drop column `version`;
alter table spells_new change `nimbuseffect` `field193` int(11) default 0;

alter table qs_player_killed_by_log drop column `instance_id`;
alter table qs_player_qglobal_updates_log drop column `instance_id`;
alter table qs_player_ts_event_log drop column `instance_id`;

drop table `instance_list`;
drop table `instance_list_player`;