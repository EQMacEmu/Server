DROP TABLE IF EXISTS `qs_player_fte_events`;
CREATE TABLE `qs_player_fte_events` (
    `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
    `char_id` int(11) DEFAULT '0',
    `guild_name` varchar(255) DEFAULT NULL,
    `mob_name` varchar(255) DEFAULT NULL,
    `engaged` TINYINT(1) DEFAULT '0',
    `time` timestamp NULL DEFAULT NULL,
    PRIMARY KEY (`id`),
    INDEX (`char_id`),
    INDEX (`guild_name`),
    INDEX (`mob_name`),
    INDEX (`time`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1;
