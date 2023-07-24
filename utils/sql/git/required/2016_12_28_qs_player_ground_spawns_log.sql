insert into rule_values values (1, "QueryServ:PlayerLogGroundSpawn", "true", "");
CREATE TABLE `qs_player_ground_spawns_log` (
  `time` datetime NOT NULL default '0000-00-00 00:00:00',
  `characterid` int(11) NOT NULL default '0',
  `itemid` int(11) NOT NULL default '0',
  `quantity` int(11) NOT NULL default '0',
  `bagged` int(11) NOT NULL default '0',
  `zone` int(11) NOT NULL default '0',
  `type` varchar(64) NOT NULL default ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1;