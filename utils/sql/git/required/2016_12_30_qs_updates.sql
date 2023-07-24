RENAME TABLE `qs_player_trade_log` TO `archived_qs_player_trade_log`;

CREATE TABLE `qs_player_trade_log` (
  `char1_id` int(11) default '0',
  `char1_pp` int(11) default '0',
  `char1_gp` int(11) default '0',
  `char1_sp` int(11) default '0',
  `char1_cp` int(11) default '0',
  `char1_items` mediumint(7) default '0',
  `char2_id` int(11) default '0',
  `char2_pp` int(11) default '0',
  `char2_gp` int(11) default '0',
  `char2_sp` int(11) default '0',
  `char2_cp` int(11) default '0',
  `char2_items` mediumint(7) default '0',
  `time` timestamp NULL default NULL on update CURRENT_TIMESTAMP
  ) ENGINE=InnoDB DEFAULT CHARSET=utf8;
  
CREATE TABLE `qs_player_trade_items_log` (
  `from_id` int(11) default '0',
  `from_slot` mediumint(7) default '0',
  `to_id` int(11) default '0',
  `to_slot` mediumint(7) default '0',
  `item_id` int(11) default '0',
  `charges` mediumint(7) default '0',
  `bagged` int(11) NOT NULL default '0',
  `type` varchar(64) NOT NULL default '',
  `time` timestamp NULL default NULL on update CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

RENAME TABLE `qs_player_handin_log` TO `archived_qs_player_handin_log`;

CREATE TABLE `qs_player_handin_log` (
  `char_id` int(11) default '0',
  `char_pp` int(11) default '0',
  `char_gp` int(11) default '0',
  `char_sp` int(11) default '0',
  `char_cp` int(11) default '0',
  `char_items` mediumint(7) default '0',
  `npc_id` int(11) default '0',
  `time` timestamp NULL default NULL on update CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;