alter table character_zone_flags drop column `key_`;
delete from `character_zone_flags`;

update zone set flag_needed = "" where zoneidnumber < 150;
update doors set nokeyring = 1 where zone in ("frozenshadow", "skyshrine") and keyitem > 0;

CREATE TABLE `keyring_data` (
  `key_item` int(11) NOT NULL default '0',
  `key_name` varchar(64) NOT NULL default '',
  `zoneid` int(11) NOT NULL default '0',
  `stage` tinyint(3) NOT NULL default '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `keyring_data` VALUES ('28602', 'Shrine Key', '114', '0');
INSERT INTO `keyring_data` VALUES ('20884', 'Key of Veeshan', '91', '0');
INSERT INTO `keyring_data` VALUES ('20883', 'Trakanon Idol', '95', '0');
INSERT INTO `keyring_data` VALUES ('20600', 'Key to Charasis', '93', '0');
INSERT INTO `keyring_data` VALUES ('20033', 'Tower of Frozen Shadows: 1', '111', '1');
INSERT INTO `keyring_data` VALUES ('20034', 'Tower of Frozen Shadows: 2', '111', '2');
INSERT INTO `keyring_data` VALUES ('20035', 'Tower of Frozen Shadows: 3', '111', '3');
INSERT INTO `keyring_data` VALUES ('20036', 'Tower of Frozen Shadows: 4', '111', '4');
INSERT INTO `keyring_data` VALUES ('20037', 'Tower of Frozen Shadows: 5', '111', '5');
INSERT INTO `keyring_data` VALUES ('20038', 'Tower of Frozen Shadows: 6', '111', '6');
INSERT INTO `keyring_data` VALUES ('20039', 'Tower of Frozen Shadows: 7', '111', '7');
