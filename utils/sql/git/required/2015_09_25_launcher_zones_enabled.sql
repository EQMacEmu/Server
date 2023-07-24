alter table `launcher_zones` add column `expansion` varchar(64) DEFAULT NULL;
alter table `launcher_zones` add column `enabled` tinyint(1) unsigned zerofill DEFAULT '0';