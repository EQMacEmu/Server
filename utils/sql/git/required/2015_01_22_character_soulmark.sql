DROP TABLE IF EXISTS `character_soulmarks`;
CREATE TABLE `character_soulmarks` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `charid` int(11) NOT NULL DEFAULT '0',
  `charname` varchar(64) NOT NULL DEFAULT '',
  `acctname` varchar(32) NOT NULL DEFAULT '',
  `gmname` varchar(64) NOT NULL DEFAULT '',
  `gmacctname` varchar(32) NOT NULL DEFAULT '',
  `utime` int(11) NOT NULL DEFAULT '0',
  `type` int(11) NOT NULL DEFAULT '0',
  `desc` varchar(256) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=263 DEFAULT CHARSET=latin1;
