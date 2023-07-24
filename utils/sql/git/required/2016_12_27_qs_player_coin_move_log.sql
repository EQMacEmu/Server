insert into rule_values values (1, "QueryServ:PlayerLogMoneyTransactions", "true", "");
insert into rule_values values (1, "QueryServ:PlayerLogBankTransactions", "false", "");
CREATE TABLE `qs_player_coin_move_log` (
  `time` datetime NOT NULL default '0000-00-00 00:00:00',
  `from_char` int(11) NOT NULL default '0',
  `to_char` int(11) NOT NULL default '0',
  `to_npc` int(11) NOT NULL default '0',
  `type` varchar(64) NOT NULL default '',
  `amount` int(11) NOT NULL default '0',
  `coin_type` varchar(64) NOT NULL default ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1;