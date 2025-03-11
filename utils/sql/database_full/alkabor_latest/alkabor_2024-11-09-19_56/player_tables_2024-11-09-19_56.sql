/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(30) NOT NULL DEFAULT '',
  `charname` varchar(64) NOT NULL DEFAULT '',
  `sharedplat` int(11) unsigned NOT NULL DEFAULT 0,
  `password` varchar(50) NOT NULL DEFAULT '',
  `status` int(5) NOT NULL DEFAULT 0,
  `lsaccount_id` int(11) unsigned DEFAULT NULL,
  `gmspeed` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `revoked` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `karma` int(5) unsigned NOT NULL DEFAULT 0,
  `minilogin_ip` varchar(32) NOT NULL DEFAULT '',
  `hideme` tinyint(4) NOT NULL DEFAULT 0,
  `rulesflag` tinyint(1) unsigned NOT NULL DEFAULT 0,
  `suspendeduntil` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `time_creation` int(10) unsigned NOT NULL DEFAULT 0,
  `expansion` tinyint(4) NOT NULL DEFAULT 12,
  `ban_reason` text DEFAULT NULL,
  `suspend_reason` text DEFAULT NULL,
  `active` tinyint(4) NOT NULL DEFAULT 0,
  `ip_exemption_multiplier` int(5) DEFAULT 1,
  `gminvul` tinyint(4) NOT NULL DEFAULT 0,
  `flymode` tinyint(4) NOT NULL DEFAULT 0,
  `ignore_tells` tinyint(4) NOT NULL DEFAULT 0,
  `mule` tinyint(4) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`) USING BTREE,
  UNIQUE KEY `lsaccount_id` (`lsaccount_id`) USING BTREE
) ENGINE=MyISAM AUTO_INCREMENT=70699 DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account_flags` (
  `p_accid` int(10) unsigned NOT NULL,
  `p_flag` varchar(50) NOT NULL,
  `p_value` varchar(80) NOT NULL,
  PRIMARY KEY (`p_accid`,`p_flag`),
  KEY `p_accid` (`p_accid`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account_ip` (
  `accid` int(11) NOT NULL DEFAULT 0,
  `ip` varchar(32) NOT NULL DEFAULT '',
  `count` int(11) NOT NULL DEFAULT 1,
  `lastused` timestamp NOT NULL DEFAULT current_timestamp(),
  UNIQUE KEY `ip` (`accid`,`ip`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account_rewards` (
  `account_id` int(10) unsigned NOT NULL,
  `reward_id` int(10) unsigned NOT NULL,
  `amount` int(10) unsigned NOT NULL,
  UNIQUE KEY `account_reward` (`account_id`,`reward_id`) USING BTREE,
  KEY `account_id` (`account_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_data` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `account_id` int(11) NOT NULL DEFAULT 0,
  `name` varchar(64) NOT NULL DEFAULT '',
  `last_name` varchar(64) NOT NULL DEFAULT '',
  `title` varchar(32) NOT NULL DEFAULT '',
  `suffix` varchar(32) NOT NULL DEFAULT '',
  `zone_id` int(11) unsigned NOT NULL DEFAULT 0,
  `y` float NOT NULL DEFAULT 0,
  `x` float NOT NULL DEFAULT 0,
  `z` float NOT NULL DEFAULT 0,
  `heading` float NOT NULL DEFAULT 0,
  `gender` tinyint(11) unsigned NOT NULL DEFAULT 0,
  `race` smallint(11) unsigned NOT NULL DEFAULT 0,
  `class` tinyint(11) unsigned NOT NULL DEFAULT 0,
  `level` int(11) unsigned NOT NULL DEFAULT 0,
  `deity` int(11) unsigned NOT NULL DEFAULT 0,
  `birthday` int(11) unsigned NOT NULL DEFAULT 0,
  `last_login` int(11) unsigned NOT NULL DEFAULT 0,
  `time_played` int(11) unsigned NOT NULL DEFAULT 0,
  `level2` tinyint(11) unsigned NOT NULL DEFAULT 0,
  `anon` tinyint(11) unsigned NOT NULL DEFAULT 0,
  `gm` tinyint(11) unsigned NOT NULL DEFAULT 0,
  `face` int(11) unsigned NOT NULL DEFAULT 0,
  `hair_color` tinyint(11) unsigned NOT NULL DEFAULT 0,
  `hair_style` tinyint(11) unsigned NOT NULL DEFAULT 0,
  `beard` tinyint(11) unsigned NOT NULL DEFAULT 0,
  `beard_color` tinyint(11) unsigned NOT NULL DEFAULT 0,
  `eye_color_1` tinyint(11) unsigned NOT NULL DEFAULT 0,
  `eye_color_2` tinyint(11) unsigned NOT NULL DEFAULT 0,
  `exp` int(11) unsigned NOT NULL DEFAULT 0,
  `aa_points_spent` int(11) unsigned NOT NULL DEFAULT 0,
  `aa_exp` int(11) unsigned NOT NULL DEFAULT 0,
  `aa_points` int(11) unsigned NOT NULL DEFAULT 0,
  `points` int(11) unsigned NOT NULL DEFAULT 0,
  `cur_hp` int(11) unsigned NOT NULL DEFAULT 0,
  `mana` int(11) unsigned NOT NULL DEFAULT 0,
  `endurance` int(11) unsigned NOT NULL DEFAULT 0,
  `intoxication` int(11) unsigned NOT NULL DEFAULT 0,
  `str` int(11) unsigned NOT NULL DEFAULT 0,
  `sta` int(11) unsigned NOT NULL DEFAULT 0,
  `cha` int(11) unsigned NOT NULL DEFAULT 0,
  `dex` int(11) unsigned NOT NULL DEFAULT 0,
  `int` int(11) unsigned NOT NULL DEFAULT 0,
  `agi` int(11) unsigned NOT NULL DEFAULT 0,
  `wis` int(11) unsigned NOT NULL DEFAULT 0,
  `zone_change_count` int(11) unsigned NOT NULL DEFAULT 0,
  `hunger_level` int(11) unsigned NOT NULL DEFAULT 0,
  `thirst_level` int(11) unsigned NOT NULL DEFAULT 0,
  `pvp_status` tinyint(11) unsigned NOT NULL DEFAULT 0,
  `air_remaining` int(11) unsigned NOT NULL DEFAULT 0,
  `autosplit_enabled` int(11) unsigned NOT NULL DEFAULT 0,
  `mailkey` char(16) NOT NULL DEFAULT '',
  `firstlogon` tinyint(3) NOT NULL DEFAULT 0,
  `e_aa_effects` int(11) unsigned NOT NULL DEFAULT 0,
  `e_percent_to_aa` int(11) unsigned NOT NULL DEFAULT 0,
  `e_expended_aa_spent` int(11) unsigned NOT NULL DEFAULT 0,
  `boatid` int(11) unsigned NOT NULL DEFAULT 0,
  `boatname` varchar(25) DEFAULT NULL,
  `famished` int(11) NOT NULL DEFAULT 0,
  `is_deleted` tinyint(4) NOT NULL DEFAULT 0,
  `showhelm` tinyint(4) NOT NULL DEFAULT 1,
  `fatigue` int(11) DEFAULT 0,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`) USING BTREE,
  KEY `account_id` (`account_id`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_currency` (
  `id` int(11) unsigned NOT NULL DEFAULT 0,
  `platinum` int(11) unsigned NOT NULL DEFAULT 0,
  `gold` int(11) unsigned NOT NULL DEFAULT 0,
  `silver` int(11) unsigned NOT NULL DEFAULT 0,
  `copper` int(11) unsigned NOT NULL DEFAULT 0,
  `platinum_bank` int(11) unsigned NOT NULL DEFAULT 0,
  `gold_bank` int(11) unsigned NOT NULL DEFAULT 0,
  `silver_bank` int(11) unsigned NOT NULL DEFAULT 0,
  `copper_bank` int(11) unsigned NOT NULL DEFAULT 0,
  `platinum_cursor` int(11) unsigned NOT NULL DEFAULT 0,
  `gold_cursor` int(11) unsigned NOT NULL DEFAULT 0,
  `silver_cursor` int(11) unsigned NOT NULL DEFAULT 0,
  `copper_cursor` int(11) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  KEY `id` (`id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_alternate_abilities` (
  `id` int(11) unsigned NOT NULL DEFAULT 0,
  `slot` smallint(11) unsigned NOT NULL DEFAULT 0,
  `aa_id` smallint(11) unsigned NOT NULL DEFAULT 0,
  `aa_value` smallint(11) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`,`slot`),
  KEY `id` (`id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_bind` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `is_home` tinyint(11) unsigned NOT NULL DEFAULT 0,
  `zone_id` smallint(11) unsigned NOT NULL DEFAULT 0,
  `x` float NOT NULL DEFAULT 0,
  `y` float NOT NULL DEFAULT 0,
  `z` float NOT NULL DEFAULT 0,
  `heading` float NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`,`is_home`),
  KEY `id` (`id`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_consent` (
  `name` varchar(64) NOT NULL DEFAULT '',
  `consenter_name` varchar(64) NOT NULL DEFAULT '',
  `corpse_id` int(11) NOT NULL DEFAULT 0,
  KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_corpses` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `charid` int(11) unsigned NOT NULL DEFAULT 0,
  `charname` varchar(64) NOT NULL DEFAULT '',
  `zone_id` smallint(5) NOT NULL DEFAULT 0,
  `x` float NOT NULL DEFAULT 0,
  `y` float NOT NULL DEFAULT 0,
  `z` float NOT NULL DEFAULT 0,
  `heading` float NOT NULL DEFAULT 0,
  `time_of_death` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `rez_time` int(11) NOT NULL DEFAULT 0,
  `is_rezzed` tinyint(3) unsigned DEFAULT 0,
  `is_buried` tinyint(3) NOT NULL DEFAULT 0,
  `was_at_graveyard` tinyint(3) NOT NULL DEFAULT 0,
  `is_locked` tinyint(11) DEFAULT 0,
  `exp` int(11) unsigned DEFAULT 0,
  `gmexp` int(11) NOT NULL DEFAULT 0,
  `size` int(11) unsigned DEFAULT 0,
  `level` int(11) unsigned DEFAULT 0,
  `race` int(11) unsigned DEFAULT 0,
  `gender` int(11) unsigned DEFAULT 0,
  `class` int(11) unsigned DEFAULT 0,
  `deity` int(11) unsigned DEFAULT 0,
  `texture` int(11) unsigned DEFAULT 0,
  `helm_texture` int(11) unsigned DEFAULT 0,
  `copper` int(11) unsigned DEFAULT 0,
  `silver` int(11) unsigned DEFAULT 0,
  `gold` int(11) unsigned DEFAULT 0,
  `platinum` int(11) unsigned DEFAULT 0,
  `hair_color` int(11) unsigned DEFAULT 0,
  `beard_color` int(11) unsigned DEFAULT 0,
  `eye_color_1` int(11) unsigned DEFAULT 0,
  `eye_color_2` int(11) unsigned DEFAULT 0,
  `hair_style` int(11) unsigned DEFAULT 0,
  `face` int(11) unsigned DEFAULT 0,
  `beard` int(11) unsigned DEFAULT 0,
  `wc_1` int(11) unsigned DEFAULT 0,
  `wc_2` int(11) unsigned DEFAULT 0,
  `wc_3` int(11) unsigned DEFAULT 0,
  `wc_4` int(11) unsigned DEFAULT 0,
  `wc_5` int(11) unsigned DEFAULT 0,
  `wc_6` int(11) unsigned DEFAULT 0,
  `wc_7` int(11) unsigned DEFAULT 0,
  `wc_8` int(11) unsigned DEFAULT 0,
  `wc_9` int(11) unsigned DEFAULT 0,
  `killedby` tinyint(11) NOT NULL DEFAULT 0,
  `rezzable` tinyint(11) NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`),
  KEY `zoneid` (`zone_id`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_corpses_backup` (
  `id` int(11) unsigned NOT NULL DEFAULT 0,
  `charid` int(11) unsigned NOT NULL DEFAULT 0,
  `charname` varchar(64) NOT NULL DEFAULT '',
  `zone_id` smallint(5) NOT NULL DEFAULT 0,
  `x` float NOT NULL DEFAULT 0,
  `y` float NOT NULL DEFAULT 0,
  `z` float NOT NULL DEFAULT 0,
  `heading` float NOT NULL DEFAULT 0,
  `time_of_death` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `rez_time` int(11) NOT NULL DEFAULT 0,
  `is_rezzed` tinyint(3) unsigned DEFAULT 0,
  `is_buried` tinyint(3) NOT NULL DEFAULT 0,
  `was_at_graveyard` tinyint(3) NOT NULL DEFAULT 0,
  `is_locked` tinyint(11) DEFAULT 0,
  `exp` int(11) unsigned DEFAULT 0,
  `gmexp` int(11) NOT NULL DEFAULT 0,
  `size` int(11) unsigned DEFAULT 0,
  `level` int(11) unsigned DEFAULT 0,
  `race` int(11) unsigned DEFAULT 0,
  `gender` int(11) unsigned DEFAULT 0,
  `class` int(11) unsigned DEFAULT 0,
  `deity` int(11) unsigned DEFAULT 0,
  `texture` int(11) unsigned DEFAULT 0,
  `helm_texture` int(11) unsigned DEFAULT 0,
  `copper` int(11) unsigned DEFAULT 0,
  `silver` int(11) unsigned DEFAULT 0,
  `gold` int(11) unsigned DEFAULT 0,
  `platinum` int(11) unsigned DEFAULT 0,
  `hair_color` int(11) unsigned DEFAULT 0,
  `beard_color` int(11) unsigned DEFAULT 0,
  `eye_color_1` int(11) unsigned DEFAULT 0,
  `eye_color_2` int(11) unsigned DEFAULT 0,
  `hair_style` int(11) unsigned DEFAULT 0,
  `face` int(11) unsigned DEFAULT 0,
  `beard` int(11) unsigned DEFAULT 0,
  `wc_1` int(11) unsigned DEFAULT 0,
  `wc_2` int(11) unsigned DEFAULT 0,
  `wc_3` int(11) unsigned DEFAULT 0,
  `wc_4` int(11) unsigned DEFAULT 0,
  `wc_5` int(11) unsigned DEFAULT 0,
  `wc_6` int(11) unsigned DEFAULT 0,
  `wc_7` int(11) unsigned DEFAULT 0,
  `wc_8` int(11) unsigned DEFAULT 0,
  `wc_9` int(11) unsigned DEFAULT 0,
  `killedby` tinyint(11) NOT NULL DEFAULT 0,
  `rezzable` tinyint(11) NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_corpse_items_backup` (
  `corpse_id` int(11) unsigned NOT NULL,
  `equip_slot` int(11) unsigned NOT NULL,
  `item_id` int(11) unsigned DEFAULT NULL,
  `charges` int(11) unsigned DEFAULT NULL,
  `aug_1` int(11) unsigned DEFAULT 0,
  `aug_2` int(11) unsigned DEFAULT 0,
  `aug_3` int(11) unsigned DEFAULT 0,
  `aug_4` int(11) unsigned DEFAULT 0,
  `aug_5` int(11) unsigned DEFAULT 0,
  `serialnumber` int(10) NOT NULL DEFAULT 0,
  PRIMARY KEY (`corpse_id`,`equip_slot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_corpse_items` (
  `corpse_id` int(11) unsigned NOT NULL,
  `equip_slot` int(11) unsigned NOT NULL,
  `item_id` int(11) unsigned DEFAULT NULL,
  `charges` int(11) unsigned DEFAULT NULL,
  `aug_1` int(11) unsigned DEFAULT 0,
  `aug_2` int(11) unsigned DEFAULT 0,
  `aug_3` int(11) unsigned DEFAULT 0,
  `aug_4` int(11) unsigned DEFAULT 0,
  `aug_5` int(11) unsigned DEFAULT 0,
  `serialnumber` int(10) NOT NULL DEFAULT 0,
  PRIMARY KEY (`corpse_id`,`equip_slot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_faction_values` (
  `id` int(11) NOT NULL DEFAULT 0,
  `faction_id` int(4) NOT NULL DEFAULT 0,
  `current_value` smallint(6) NOT NULL DEFAULT 0,
  `temp` tinyint(3) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`,`faction_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_inventory` (
  `id` int(11) NOT NULL DEFAULT 0,
  `slotid` mediumint(7) unsigned NOT NULL DEFAULT 0,
  `itemid` int(11) unsigned DEFAULT 0,
  `charges` smallint(3) unsigned DEFAULT 0,
  `custom_data` text DEFAULT NULL,
  `serialnumber` int(10) NOT NULL DEFAULT 0,
  `initialserial` tinyint(3) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`,`slotid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_keyring` (
  `id` int(11) NOT NULL DEFAULT 0,
  `item_id` int(11) NOT NULL DEFAULT 0
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_languages` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `lang_id` smallint(11) unsigned NOT NULL DEFAULT 0,
  `value` smallint(11) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`,`lang_id`),
  KEY `id` (`id`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_lookup` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `account_id` int(11) NOT NULL DEFAULT 0,
  `name` varchar(64) NOT NULL DEFAULT '',
  `timelaston` int(11) unsigned DEFAULT 0,
  `x` float NOT NULL DEFAULT 0,
  `y` float NOT NULL DEFAULT 0,
  `z` float NOT NULL DEFAULT 0,
  `zonename` varchar(30) NOT NULL DEFAULT '',
  `zoneid` smallint(6) NOT NULL DEFAULT 0,
  `instanceid` smallint(5) unsigned NOT NULL DEFAULT 0,
  `pktime` int(8) NOT NULL DEFAULT 0,
  `groupid` int(10) unsigned NOT NULL DEFAULT 0,
  `class` tinyint(4) NOT NULL DEFAULT 0,
  `level` mediumint(8) unsigned NOT NULL DEFAULT 0,
  `lfp` tinyint(1) unsigned NOT NULL DEFAULT 0,
  `lfg` tinyint(1) unsigned NOT NULL DEFAULT 0,
  `mailkey` char(16) NOT NULL,
  `xtargets` tinyint(3) unsigned NOT NULL DEFAULT 5,
  `firstlogon` tinyint(3) NOT NULL DEFAULT 0,
  `inspectmessage` varchar(256) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`) USING BTREE,
  KEY `account_id` (`account_id`) USING BTREE
) ENGINE=MyISAM AUTO_INCREMENT=171020 DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_skills` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `skill_id` smallint(11) unsigned NOT NULL DEFAULT 0,
  `value` smallint(11) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`,`skill_id`),
  KEY `id` (`id`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_soulmarks` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `charid` int(11) NOT NULL DEFAULT 0,
  `charname` varchar(64) NOT NULL DEFAULT '',
  `acctname` varchar(32) NOT NULL DEFAULT '',
  `gmname` varchar(64) NOT NULL DEFAULT '',
  `gmacctname` varchar(32) NOT NULL DEFAULT '',
  `utime` int(11) NOT NULL DEFAULT 0,
  `type` int(11) NOT NULL DEFAULT 0,
  `desc` varchar(256) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_spells` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `slot_id` smallint(11) unsigned NOT NULL DEFAULT 0,
  `spell_id` smallint(11) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`,`slot_id`),
  KEY `id` (`id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_timers` (
  `id` int(11) NOT NULL DEFAULT 0,
  `type` mediumint(8) unsigned NOT NULL DEFAULT 0,
  `start` int(10) unsigned NOT NULL DEFAULT 0,
  `duration` int(10) unsigned NOT NULL DEFAULT 0,
  `enable` tinyint(4) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`,`type`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_memmed_spells` (
  `id` int(11) unsigned NOT NULL DEFAULT 0,
  `slot_id` smallint(11) unsigned NOT NULL DEFAULT 0,
  `spell_id` smallint(11) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`,`slot_id`),
  KEY `id` (`id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_inspect_messages` (
  `id` int(11) unsigned NOT NULL DEFAULT 0,
  `inspect_message` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  KEY `id` (`id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_buffs` (
  `id` int(10) NOT NULL DEFAULT 0,
  `slot_id` tinyint(3) unsigned NOT NULL,
  `spell_id` smallint(10) unsigned NOT NULL,
  `caster_level` tinyint(3) unsigned NOT NULL,
  `caster_name` varchar(64) NOT NULL,
  `ticsremaining` int(10) unsigned NOT NULL,
  `counters` int(10) unsigned NOT NULL,
  `melee_rune` int(10) unsigned NOT NULL,
  `magic_rune` int(10) unsigned NOT NULL,
  `persistent` tinyint(3) unsigned NOT NULL,
  `ExtraDIChance` int(10) NOT NULL DEFAULT 0,
  `bard_modifier` tinyint(3) unsigned NOT NULL DEFAULT 10,
  `bufftype` int(11) NOT NULL,
  PRIMARY KEY (`id`,`slot_id`),
  KEY `character_id` (`id`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_pet_buffs` (
  `char_id` int(11) NOT NULL,
  `pet` int(11) NOT NULL,
  `slot` int(11) NOT NULL,
  `spell_id` int(11) NOT NULL,
  `caster_level` tinyint(4) NOT NULL DEFAULT 0,
  `castername` varchar(64) NOT NULL DEFAULT '',
  `ticsremaining` int(11) NOT NULL DEFAULT 0,
  `counters` int(11) NOT NULL DEFAULT 0,
  `numhits` int(11) NOT NULL DEFAULT 0,
  `rune` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`char_id`,`pet`,`slot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_pet_info` (
  `char_id` int(11) NOT NULL,
  `pet` int(11) NOT NULL,
  `petname` varchar(64) NOT NULL DEFAULT '',
  `petpower` int(11) NOT NULL DEFAULT 0,
  `spell_id` int(11) NOT NULL DEFAULT 0,
  `hp` int(11) NOT NULL DEFAULT 0,
  `mana` int(11) NOT NULL DEFAULT 0,
  `size` float NOT NULL DEFAULT 0,
  PRIMARY KEY (`char_id`,`pet`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_pet_inventory` (
  `char_id` int(11) NOT NULL,
  `pet` int(11) NOT NULL,
  `slot` int(11) NOT NULL,
  `item_id` int(11) NOT NULL,
  PRIMARY KEY (`char_id`,`pet`,`slot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_zone_flags` (
  `id` int(11) NOT NULL DEFAULT 0,
  `zoneID` int(11) NOT NULL DEFAULT 0,
  `key_` tinyint(4) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`,`zoneID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `friends` (
  `charid` int(10) unsigned NOT NULL,
  `type` tinyint(1) unsigned NOT NULL DEFAULT 1 COMMENT '1 = Friend, 0 = Ignore',
  `name` varchar(64) NOT NULL,
  PRIMARY KEY (`charid`,`type`,`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mail` (
  `msgid` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `charid` int(10) unsigned NOT NULL DEFAULT 0,
  `timestamp` int(11) NOT NULL DEFAULT 0,
  `from` varchar(100) NOT NULL DEFAULT '',
  `subject` varchar(200) NOT NULL DEFAULT '',
  `body` text NOT NULL,
  `to` text NOT NULL,
  `status` tinyint(4) NOT NULL DEFAULT 0,
  PRIMARY KEY (`msgid`),
  KEY `charid` (`charid`) USING BTREE
) ENGINE=MyISAM AUTO_INCREMENT=2478 DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player_titlesets` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `char_id` int(11) unsigned NOT NULL,
  `title_set` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `id` (`id`) USING BTREE
) ENGINE=MyISAM AUTO_INCREMENT=717 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `trader` (
  `char_id` int(10) unsigned NOT NULL DEFAULT 0,
  `item_id` int(10) unsigned NOT NULL DEFAULT 0,
  `i_slotid` int(10) NOT NULL DEFAULT 0,
  `charges` int(11) NOT NULL DEFAULT 0,
  `item_cost` int(10) unsigned NOT NULL DEFAULT 0,
  `slot_id` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`char_id`,`slot_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `banned_ips` (
  `ip_address` varchar(20) NOT NULL DEFAULT '0',
  `notes` text DEFAULT NULL,
  PRIMARY KEY (`ip_address`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bugs` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `zone` varchar(32) NOT NULL,
  `name` varchar(64) NOT NULL,
  `ui` varchar(128) NOT NULL,
  `x` float NOT NULL DEFAULT 0,
  `y` float NOT NULL DEFAULT 0,
  `z` float NOT NULL DEFAULT 0,
  `type` varchar(64) NOT NULL,
  `flag` tinyint(3) unsigned NOT NULL,
  `target` varchar(64) DEFAULT NULL,
  `bug` varchar(1024) NOT NULL,
  `date` date NOT NULL,
  `status` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `_can_duplicate` tinyint(1) NOT NULL DEFAULT 0,
  `_crash_bug` tinyint(1) NOT NULL DEFAULT 0,
  `_target_info` tinyint(1) NOT NULL DEFAULT 0,
  `_character_flags` tinyint(1) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chatchannels` (
  `name` varchar(64) NOT NULL DEFAULT '',
  `owner` varchar(64) NOT NULL DEFAULT '',
  `password` varchar(64) NOT NULL DEFAULT '',
  `minstatus` int(5) NOT NULL DEFAULT 0,
  PRIMARY KEY (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `data_buckets` (
  `id` bigint(11) unsigned NOT NULL AUTO_INCREMENT,
  `key` varchar(100) DEFAULT NULL,
  `value` text DEFAULT NULL,
  `expires` int(11) unsigned DEFAULT 0,
  PRIMARY KEY (`id`),
  KEY `key_index` (`key`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `discovered_items` (
  `item_id` int(11) unsigned NOT NULL DEFAULT 0,
  `char_name` varchar(64) NOT NULL DEFAULT '',
  `discovered_date` int(11) unsigned NOT NULL DEFAULT 0,
  `account_status` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`item_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `eventlog` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `accountname` varchar(30) NOT NULL DEFAULT '',
  `accountid` int(10) unsigned DEFAULT 0,
  `status` int(5) NOT NULL DEFAULT 0,
  `charname` varchar(64) NOT NULL DEFAULT '',
  `target` varchar(64) DEFAULT 'None',
  `time` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  `descriptiontype` varchar(50) NOT NULL DEFAULT '',
  `description` text NOT NULL,
  `event_nid` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=5450 DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gm_ips` (
  `name` varchar(64) NOT NULL,
  `account_id` int(11) NOT NULL,
  `ip_address` varchar(15) NOT NULL,
  UNIQUE KEY `account_id` (`account_id`,`ip_address`) USING BTREE,
  UNIQUE KEY `account_id_2` (`account_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `group_id` (
  `groupid` int(4) NOT NULL,
  `charid` int(4) NOT NULL,
  `name` varchar(64) NOT NULL,
  `accountid` int(4) NOT NULL DEFAULT 0,
  PRIMARY KEY (`groupid`,`charid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `group_leaders` (
  `gid` int(4) NOT NULL DEFAULT 0,
  `leadername` varchar(64) NOT NULL DEFAULT '',
  `oldleadername` varchar(64) NOT NULL DEFAULT '',
  PRIMARY KEY (`gid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `guilds` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(32) NOT NULL DEFAULT '',
  `leader` int(11) NOT NULL DEFAULT 0,
  `minstatus` smallint(5) NOT NULL DEFAULT 0,
  `motd` text NOT NULL,
  `tribute` int(10) unsigned NOT NULL DEFAULT 0,
  `motd_setter` varchar(64) NOT NULL DEFAULT '',
  `channel` varchar(128) NOT NULL DEFAULT '',
  `url` varchar(512) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`) USING BTREE,
  UNIQUE KEY `leader` (`leader`) USING BTREE
) ENGINE=MyISAM AUTO_INCREMENT=403 DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `guild_ranks` (
  `guild_id` mediumint(8) unsigned NOT NULL DEFAULT 0,
  `rank` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `title` varchar(128) NOT NULL DEFAULT '',
  `can_hear` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `can_speak` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `can_invite` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `can_remove` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `can_promote` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `can_demote` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `can_motd` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `can_warpeace` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guild_id`,`rank`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `guild_members` (
  `char_id` int(11) NOT NULL DEFAULT 0,
  `guild_id` mediumint(8) unsigned NOT NULL DEFAULT 0,
  `rank` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `tribute_enable` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `total_tribute` int(10) unsigned NOT NULL DEFAULT 0,
  `last_tribute` int(10) unsigned NOT NULL DEFAULT 0,
  `banker` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `public_note` text NOT NULL,
  `alt` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`char_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `hackers` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `account` text NOT NULL,
  `name` text NOT NULL,
  `hacked` text NOT NULL,
  `zone` text DEFAULT NULL,
  `date` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1554357 DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `merchantlist_temp` (
  `npcid` int(10) unsigned NOT NULL DEFAULT 0,
  `slot` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `itemid` int(10) unsigned NOT NULL DEFAULT 0,
  `charges` int(10) unsigned NOT NULL DEFAULT 1,
  `quantity` tinyint(4) NOT NULL DEFAULT 0,
  PRIMARY KEY (`npcid`,`slot`),
  KEY `npcid_2` (`npcid`,`itemid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `name_filter` (
  `name` varchar(30) NOT NULL DEFAULT '',
  PRIMARY KEY (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `object_contents` (
  `zoneid` int(11) unsigned NOT NULL DEFAULT 0,
  `parentid` int(11) unsigned NOT NULL DEFAULT 0,
  `bagidx` int(11) unsigned NOT NULL DEFAULT 0,
  `itemid` int(11) unsigned NOT NULL DEFAULT 0,
  `charges` smallint(3) NOT NULL DEFAULT 0,
  `droptime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`parentid`,`bagidx`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `petitions` (
  `dib` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `petid` int(10) unsigned NOT NULL DEFAULT 0,
  `charname` varchar(32) NOT NULL DEFAULT '',
  `accountname` varchar(32) NOT NULL DEFAULT '',
  `lastgm` varchar(32) NOT NULL DEFAULT '',
  `petitiontext` text NOT NULL,
  `gmtext` text DEFAULT NULL,
  `zone` varchar(32) NOT NULL DEFAULT '',
  `urgency` int(11) NOT NULL DEFAULT 0,
  `charclass` int(11) NOT NULL DEFAULT 0,
  `charrace` int(11) NOT NULL DEFAULT 0,
  `charlevel` int(11) NOT NULL DEFAULT 0,
  `checkouts` int(11) NOT NULL DEFAULT 0,
  `unavailables` int(11) NOT NULL DEFAULT 0,
  `ischeckedout` tinyint(4) NOT NULL DEFAULT 0,
  `senttime` bigint(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`dib`),
  KEY `petid` (`petid`) USING BTREE
) ENGINE=MyISAM AUTO_INCREMENT=2157 DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_merchant_transaction_log` (
  `char_id` int(11) DEFAULT 0,
  `char_slot` mediumint(7) DEFAULT 0,
  `item_id` int(11) DEFAULT 0,
  `charges` mediumint(7) DEFAULT 0,
  `zone_id` int(11) DEFAULT 0,
  `merchant_id` int(11) DEFAULT 0,
  `merchant_pp` int(11) DEFAULT 0,
  `merchant_gp` int(11) DEFAULT 0,
  `merchant_sp` int(11) DEFAULT 0,
  `merchant_cp` int(11) DEFAULT 0,
  `merchant_items` mediumint(7) DEFAULT 0,
  `char_pp` int(11) DEFAULT 0,
  `char_gp` int(11) DEFAULT 0,
  `char_sp` int(11) DEFAULT 0,
  `char_cp` int(11) DEFAULT 0,
  `char_items` mediumint(7) DEFAULT 0,
  `time` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_aa_purchase_log` (
  `char_id` int(11) DEFAULT 0,
  `aa_type` varchar(255) DEFAULT '0',
  `aa_name` varchar(255) DEFAULT '0',
  `aa_id` int(11) DEFAULT 0,
  `aa_cost` int(11) DEFAULT 0,
  `zone_id` int(11) DEFAULT 0,
  `instance_id` int(11) DEFAULT 0,
  `time` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_aa_rate_hourly` (
  `char_id` int(11) NOT NULL DEFAULT 0,
  `hour_time` int(11) NOT NULL,
  `aa_count` varchar(11) DEFAULT NULL,
  PRIMARY KEY (`char_id`,`hour_time`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_coin_move_log` (
  `time` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `from_char` int(11) NOT NULL DEFAULT 0,
  `to_char` int(11) NOT NULL DEFAULT 0,
  `to_npc` int(11) NOT NULL DEFAULT 0,
  `type` varchar(64) NOT NULL DEFAULT '',
  `amount` int(11) NOT NULL DEFAULT 0,
  `coin_type` varchar(64) NOT NULL DEFAULT ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_events` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `char_id` int(11) DEFAULT 0,
  `event` int(11) unsigned DEFAULT 0,
  `event_desc` varchar(255) DEFAULT NULL,
  `time` int(11) unsigned DEFAULT 0,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_ground_spawns_log` (
  `time` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `characterid` int(11) NOT NULL DEFAULT 0,
  `itemid` int(11) NOT NULL DEFAULT 0,
  `quantity` int(11) NOT NULL DEFAULT 0,
  `bagged` int(11) NOT NULL DEFAULT 0,
  `zone` int(11) NOT NULL DEFAULT 0,
  `type` varchar(64) NOT NULL DEFAULT ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_handin_log` (
  `char_id` int(11) DEFAULT 0,
  `char_pp` int(11) DEFAULT 0,
  `char_gp` int(11) DEFAULT 0,
  `char_sp` int(11) DEFAULT 0,
  `char_cp` int(11) DEFAULT 0,
  `char_items` mediumint(7) DEFAULT 0,
  `npc_id` int(11) DEFAULT 0,
  `time` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_item_delete_log` (
  `char_id` int(11) DEFAULT 0,
  `char_slot` mediumint(7) DEFAULT 0,
  `item_id` int(11) DEFAULT 0,
  `charges` mediumint(7) DEFAULT 0,
  `stack_size` mediumint(7) DEFAULT 0,
  `char_items` mediumint(7) DEFAULT 0,
  `time` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_item_desyncs_log` (
  `char_id` int(11) DEFAULT 0,
  `error` varchar(512) DEFAULT NULL,
  `time` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp(),
  `zone_id` int(10) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_item_move_log` (
  `char_id` int(11) DEFAULT 0,
  `from_slot` mediumint(7) DEFAULT 0,
  `to_slot` mediumint(7) DEFAULT 0,
  `item_id` int(11) DEFAULT 0,
  `charges` mediumint(7) DEFAULT 0,
  `stack_size` mediumint(7) DEFAULT 0,
  `char_items` mediumint(7) DEFAULT 0,
  `postaction` tinyint(1) DEFAULT 0,
  `time` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_killed_by_log` (
  `char_id` int(11) DEFAULT 0,
  `zone_id` int(11) DEFAULT 0,
  `killed_by` varchar(255) DEFAULT NULL,
  `spell` int(11) DEFAULT 0,
  `damage` int(11) DEFAULT 0,
  `time` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp(),
  `type` varchar(255) DEFAULT 'UNKNOWN'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_loot_records_log` (
  `char_id` int(11) DEFAULT 0,
  `corpse_name` varchar(255) DEFAULT NULL,
  `type` varchar(255) DEFAULT NULL,
  `zone_id` int(11) DEFAULT 0,
  `item_id` int(11) DEFAULT 0,
  `item_name` varchar(255) DEFAULT NULL,
  `charges` mediumint(7) DEFAULT 0,
  `platinum` int(11) DEFAULT 0,
  `gold` int(11) DEFAULT 0,
  `silver` int(11) DEFAULT 0,
  `copper` int(11) DEFAULT 0,
  `time` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_npc_kill_log` (
  `char_id` int(11) DEFAULT 0,
  `npc_id` int(11) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `aggro_charid` int(11) NOT NULL DEFAULT 0,
  `zone_id` int(11) DEFAULT NULL,
  `time` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_qglobal_updates_log` (
  `char_id` int(11) DEFAULT 0,
  `action` varchar(255) DEFAULT NULL,
  `zone_id` int(11) DEFAULT 0,
  `varname` varchar(255) DEFAULT NULL,
  `newvalue` varchar(255) DEFAULT NULL,
  `time` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_speech` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `from` varchar(64) NOT NULL,
  `to` varchar(64) NOT NULL,
  `message` varchar(256) NOT NULL,
  `minstatus` smallint(5) NOT NULL,
  `guilddbid` int(11) NOT NULL,
  `type` tinyint(3) NOT NULL,
  `timerecorded` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_trade_items_log` (
  `from_id` int(11) DEFAULT 0,
  `from_slot` mediumint(7) DEFAULT 0,
  `to_id` int(11) DEFAULT 0,
  `to_slot` mediumint(7) DEFAULT 0,
  `item_id` int(11) DEFAULT 0,
  `charges` mediumint(7) DEFAULT 0,
  `bagged` int(11) NOT NULL DEFAULT 0,
  `type` varchar(64) NOT NULL DEFAULT '',
  `time` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_trade_log` (
  `char1_id` int(11) DEFAULT 0,
  `char1_pp` int(11) DEFAULT 0,
  `char1_gp` int(11) DEFAULT 0,
  `char1_sp` int(11) DEFAULT 0,
  `char1_cp` int(11) DEFAULT 0,
  `char1_items` mediumint(7) DEFAULT 0,
  `char2_id` int(11) DEFAULT 0,
  `char2_pp` int(11) DEFAULT 0,
  `char2_gp` int(11) DEFAULT 0,
  `char2_sp` int(11) DEFAULT 0,
  `char2_cp` int(11) DEFAULT 0,
  `char2_items` mediumint(7) DEFAULT 0,
  `time` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_player_ts_event_log` (
  `char_id` int(11) DEFAULT 0,
  `zone_id` int(11) DEFAULT 0,
  `results` varchar(255) DEFAULT NULL,
  `recipe_id` int(11) DEFAULT 0,
  `tradeskill` int(11) DEFAULT 0,
  `trivial` int(11) DEFAULT 0,
  `chance` float DEFAULT 0,
  `time` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qs_trader_audit` (
  `time` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `seller` varchar(64) NOT NULL DEFAULT '',
  `buyer` varchar(64) NOT NULL DEFAULT '',
  `itemname` varchar(64) NOT NULL DEFAULT '',
  `quantity` int(11) NOT NULL DEFAULT 0,
  `totalcost` int(11) NOT NULL DEFAULT 0,
  `itemid` int(11) NOT NULL DEFAULT 0,
  `serialnumber` int(10) NOT NULL DEFAULT 0
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_globals` (
  `charid` int(11) NOT NULL DEFAULT 0,
  `npcid` int(11) NOT NULL DEFAULT 0,
  `zoneid` int(11) NOT NULL DEFAULT 0,
  `name` varchar(65) NOT NULL DEFAULT '',
  `value` varchar(128) NOT NULL DEFAULT '?',
  `expdate` int(11) DEFAULT NULL,
  PRIMARY KEY (`charid`,`npcid`,`zoneid`,`name`),
  UNIQUE KEY `qname` (`name`,`charid`,`npcid`,`zoneid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `raid_details` (
  `raidid` int(4) NOT NULL DEFAULT 0,
  `loottype` int(4) NOT NULL DEFAULT 0,
  `locked` tinyint(1) NOT NULL DEFAULT 0,
  PRIMARY KEY (`raidid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `raid_members` (
  `raidid` int(4) NOT NULL DEFAULT 0,
  `charid` int(4) NOT NULL DEFAULT 0,
  `groupid` int(4) unsigned NOT NULL DEFAULT 0,
  `_class` tinyint(4) NOT NULL DEFAULT 0,
  `level` tinyint(4) NOT NULL DEFAULT 0,
  `name` varchar(64) NOT NULL DEFAULT '',
  `isgroupleader` tinyint(1) NOT NULL DEFAULT 0,
  `israidleader` tinyint(1) NOT NULL DEFAULT 0,
  `islooter` tinyint(1) NOT NULL DEFAULT 0,
  PRIMARY KEY (`charid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `reports` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(64) DEFAULT NULL,
  `reported` varchar(64) DEFAULT NULL,
  `reported_text` text DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `respawn_times` (
  `id` int(11) NOT NULL DEFAULT 0,
  `start` int(11) NOT NULL DEFAULT 0,
  `duration` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `server_scheduled_events` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `description` varchar(255) DEFAULT NULL,
  `event_type` varchar(100) DEFAULT NULL,
  `event_data` text DEFAULT NULL,
  `minute_start` int(11) DEFAULT 0,
  `hour_start` int(11) DEFAULT 0,
  `day_start` int(11) DEFAULT 0,
  `month_start` int(11) DEFAULT 0,
  `year_start` int(11) DEFAULT 0,
  `minute_end` int(11) DEFAULT 0,
  `hour_end` int(11) DEFAULT 0,
  `day_end` int(11) DEFAULT 0,
  `month_end` int(11) DEFAULT 0,
  `year_end` int(11) DEFAULT 0,
  `cron_expression` varchar(100) DEFAULT NULL,
  `created_at` datetime DEFAULT NULL,
  `deleted_at` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_globals` (
  `spellid` int(11) NOT NULL,
  `spell_name` varchar(64) NOT NULL DEFAULT '',
  `qglobal` varchar(65) NOT NULL DEFAULT '',
  `value` varchar(65) NOT NULL DEFAULT '',
  PRIMARY KEY (`spellid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `client_version` (
  `account_id` int(11) NOT NULL DEFAULT 0,
  `version_` tinyint(5) NOT NULL DEFAULT 0,
  PRIMARY KEY (`account_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `commands_log` (
  `entry_id` int(11) NOT NULL AUTO_INCREMENT,
  `char_name` varchar(65) DEFAULT NULL,
  `acct_name` varchar(65) DEFAULT NULL,
  `y` float NOT NULL DEFAULT 0,
  `x` float NOT NULL DEFAULT 0,
  `z` float NOT NULL DEFAULT 0,
  `command` varchar(100) DEFAULT NULL,
  `target_type` varchar(65) DEFAULT NULL,
  `target` varchar(65) DEFAULT NULL,
  `tar_y` float NOT NULL DEFAULT 0,
  `tar_x` float NOT NULL DEFAULT 0,
  `tar_z` float NOT NULL DEFAULT 0,
  `zone_id` int(11) DEFAULT NULL,
  `zone_name` varchar(65) DEFAULT NULL,
  `time` datetime DEFAULT NULL,
  PRIMARY KEY (`entry_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `webdata_character` (
  `id` int(11) NOT NULL,
  `name` varchar(32) DEFAULT NULL,
  `last_login` int(11) DEFAULT NULL,
  `last_seen` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `webdata_servers` (
  `id` int(11) NOT NULL,
  `name` varchar(32) DEFAULT NULL,
  `connected` tinyint(3) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
