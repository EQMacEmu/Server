DROP TABLE IF EXISTS `mb_messages`;
CREATE TABLE `mb_messages` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `category` smallint(7) NOT NULL DEFAULT '0',
  `date` varchar(16) NOT NULL DEFAULT '',
  `author` varchar(64) NOT NULL DEFAULT '',
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `subject` varchar(29) NOT NULL DEFAULT '',
  `charid` int(11) NOT NULL DEFAULT '0',
  `language` tinyint(3) NOT NULL DEFAULT '0',
  `message` varchar(2048) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=latin1;