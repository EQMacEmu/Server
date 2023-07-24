-- ----------------------------
-- Table structure for skill_difficulty
-- ----------------------------
CREATE TABLE `skill_difficulty` (
  `skillid` tinyint(3) NOT NULL default '0',
  `difficulty` float NOT NULL default '0',
  `name` varchar(32) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `skill_difficulty` VALUES ('0', '15', 'Skill1HBlunt');
INSERT INTO `skill_difficulty` VALUES ('1', '12', 'Skill1HSlashing');
INSERT INTO `skill_difficulty` VALUES ('2', '12', 'Skill2HBlunt');
INSERT INTO `skill_difficulty` VALUES ('3', '12', 'Skill2HSlashing');
INSERT INTO `skill_difficulty` VALUES ('4', '7', 'SkillAbjuration');
INSERT INTO `skill_difficulty` VALUES ('5', '7', 'SkillAlteration');
INSERT INTO `skill_difficulty` VALUES ('6', '7', 'SkillApplyPoison');
INSERT INTO `skill_difficulty` VALUES ('7', '12', 'SkillArchery');
INSERT INTO `skill_difficulty` VALUES ('8', '3', 'SkillBackstab');
INSERT INTO `skill_difficulty` VALUES ('9', '5', 'SkillBindWound');
INSERT INTO `skill_difficulty` VALUES ('10', '3', 'SkillBash');
INSERT INTO `skill_difficulty` VALUES ('11', '10', 'SkillBlock');
INSERT INTO `skill_difficulty` VALUES ('12', '12', 'SkillBrassInstruments');
INSERT INTO `skill_difficulty` VALUES ('13', '5', 'SkillChanneling');
INSERT INTO `skill_difficulty` VALUES ('14', '7', 'SkillConjuration');
INSERT INTO `skill_difficulty` VALUES ('15', '15', 'SkillDefense');
INSERT INTO `skill_difficulty` VALUES ('16', '5', 'SkillDisarm');
INSERT INTO `skill_difficulty` VALUES ('17', '7', 'SkillDisarmTraps');
INSERT INTO `skill_difficulty` VALUES ('18', '7', 'SkillDivination');
INSERT INTO `skill_difficulty` VALUES ('19', '10', 'SkillDodge');
INSERT INTO `skill_difficulty` VALUES ('20', '10', 'SkillDoubleAttack');
INSERT INTO `skill_difficulty` VALUES ('21', '3', 'SkillDragonPunch');
INSERT INTO `skill_difficulty` VALUES ('22', '12', 'SkillDualWield');
INSERT INTO `skill_difficulty` VALUES ('23', '3', 'SkillEagleStrike');
INSERT INTO `skill_difficulty` VALUES ('24', '7', 'SkillEvocation');
INSERT INTO `skill_difficulty` VALUES ('25', '5', 'SkillFeignDeath');
INSERT INTO `skill_difficulty` VALUES ('26', '3', 'SkillFlyingKick');
INSERT INTO `skill_difficulty` VALUES ('27', '9', 'SkillForage');
INSERT INTO `skill_difficulty` VALUES ('28', '12', 'SkillHandtoHand');
INSERT INTO `skill_difficulty` VALUES ('29', '5', 'SkillHide');
INSERT INTO `skill_difficulty` VALUES ('30', '3', 'SkillKick');
INSERT INTO `skill_difficulty` VALUES ('31', '9', 'SkillMeditate');
INSERT INTO `skill_difficulty` VALUES ('32', '7', 'SkillMend');
INSERT INTO `skill_difficulty` VALUES ('33', '12', 'SkillOffense');
INSERT INTO `skill_difficulty` VALUES ('34', '10', 'SkillParry');
INSERT INTO `skill_difficulty` VALUES ('35', '6', 'SkillPickLock');
INSERT INTO `skill_difficulty` VALUES ('36', '12', 'Skill1HPiercing');
INSERT INTO `skill_difficulty` VALUES ('37', '10', 'SkillRiposte');
INSERT INTO `skill_difficulty` VALUES ('38', '3', 'SkillRoundKick');
INSERT INTO `skill_difficulty` VALUES ('39', '7', 'SkillSafeFall');
INSERT INTO `skill_difficulty` VALUES ('40', '9', 'SkillSenseHeading');
INSERT INTO `skill_difficulty` VALUES ('41', '12', 'SkillSinging');
INSERT INTO `skill_difficulty` VALUES ('42', '5', 'SkillSneak');
INSERT INTO `skill_difficulty` VALUES ('43', '7', 'SkillSpecializeAbjure');
INSERT INTO `skill_difficulty` VALUES ('44', '7', 'SkillSpecializeAlteration');
INSERT INTO `skill_difficulty` VALUES ('45', '7', 'SkillSpecializeConjuration');
INSERT INTO `skill_difficulty` VALUES ('46', '7', 'SkillSpecializeDivination');
INSERT INTO `skill_difficulty` VALUES ('47', '7', 'SkillSpecializeEvocation');
INSERT INTO `skill_difficulty` VALUES ('48', '9', 'SkillPickPockets');
INSERT INTO `skill_difficulty` VALUES ('49', '12', 'SkillStringedInstruments');
INSERT INTO `skill_difficulty` VALUES ('50', '15', 'SkillSwimming');
INSERT INTO `skill_difficulty` VALUES ('51', '7', 'SkillThrowing');
INSERT INTO `skill_difficulty` VALUES ('52', '3', 'SkillTigerClaw');
INSERT INTO `skill_difficulty` VALUES ('53', '15', 'SkillTracking');
INSERT INTO `skill_difficulty` VALUES ('54', '12', 'SkillWindInstruments');
INSERT INTO `skill_difficulty` VALUES ('55', '5', 'SkillFishing');
INSERT INTO `skill_difficulty` VALUES ('56', '0', 'SkillMakePoison');
INSERT INTO `skill_difficulty` VALUES ('57', '0', 'SkillTinkering');
INSERT INTO `skill_difficulty` VALUES ('58', '0', 'SkillResearch');
INSERT INTO `skill_difficulty` VALUES ('59', '0', 'SkillAlchemy');
INSERT INTO `skill_difficulty` VALUES ('60', '0', 'SkillBaking');
INSERT INTO `skill_difficulty` VALUES ('61', '0', 'SkillTailoring');
INSERT INTO `skill_difficulty` VALUES ('62', '7', 'SkillSenseTraps');
INSERT INTO `skill_difficulty` VALUES ('63', '0', 'SkillBlacksmithing');
INSERT INTO `skill_difficulty` VALUES ('64', '0', 'SkillFletching');
INSERT INTO `skill_difficulty` VALUES ('65', '0', 'SkillBrewing');
INSERT INTO `skill_difficulty` VALUES ('66', '7', 'SkillAlcoholTolerance');
INSERT INTO `skill_difficulty` VALUES ('67', '10', 'SkillBegging');
INSERT INTO `skill_difficulty` VALUES ('68', '0', 'SkillJewelryMaking');
INSERT INTO `skill_difficulty` VALUES ('69', '0', 'SkillPottery');
INSERT INTO `skill_difficulty` VALUES ('70', '12', 'SkillPercussionInstruments');
INSERT INTO `skill_difficulty` VALUES ('71', '3', 'SkillIntimidation');
INSERT INTO `skill_difficulty` VALUES ('72', '0', 'SkillBerserking');
INSERT INTO `skill_difficulty` VALUES ('73', '3', 'SkillTaunt');
INSERT INTO `skill_difficulty` VALUES ('74', '3', 'SkillFrenzy');
