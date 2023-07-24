drop table `character_consent`;
CREATE TABLE `character_consent` (
  `name` varchar(64) NOT NULL default '',
  `consenter_name` varchar(64) NOT NULL default '',
  `corpse_id` int(11) NOT NULL default '0',
  KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
