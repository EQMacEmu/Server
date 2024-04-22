ALTER TABLE `lootdrop_entries` ADD `min_expansion` tinyint(4) NOT NULL DEFAULT -1;
ALTER TABLE `lootdrop_entries` ADD `max_expansion` tinyint(4) NOT NULL DEFAULT -1;
ALTER TABLE `lootdrop_entries` ADD `content_flags` varchar(100) NULL;
ALTER TABLE `lootdrop_entries` ADD `content_flags_disabled` varchar(100) NULL;