CREATE TABLE IF NOT EXISTS `qs_player_ks_log` (
  `npc_type_id` int(11) unsigned DEFAULT 0,
  `zone_id` int(11) unsigned DEFAULT 0,
  `killed_by_id` int(11) unsigned DEFAULT 0,
  `killed_by_group_id` int(11) unsigned DEFAULT 0,
  `killed_by_raid_id` int(11) unsigned DEFAULT 0,
  `initial_engage_ids` text DEFAULT '{}',
  `npc_lootables` text DEFAULT '{}',
  `time` timestamp NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
