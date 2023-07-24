-- ----------------------------
-- Table structure for launcher
-- ----------------------------
DROP TABLE IF EXISTS `launcher`;
CREATE TABLE `launcher` (
  `name` varchar(64) NOT NULL DEFAULT '',
  `dynamics` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of launcher
-- ----------------------------
INSERT INTO `launcher` VALUES ('boats', '0');
INSERT INTO `launcher` VALUES ('zone1', '0');
INSERT INTO `launcher` VALUES ('zone2', '0');
INSERT INTO `launcher` VALUES ('zone3', '0');
INSERT INTO `launcher` VALUES ('dynzone1', '0');
INSERT INTO `launcher` VALUES ('dynzone2', '0');
