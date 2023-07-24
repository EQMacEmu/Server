ALTER TABLE `items` ADD `soulbound` tinyint( 4 ) NOT NULL DEFAULT '0';
UPDATE items set soulbound = 1 where gmflag = -1 OR id in (20600, 20883);