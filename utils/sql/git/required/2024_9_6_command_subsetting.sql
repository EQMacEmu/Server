CREATE TABLE `command_subsettings` (
`id` int UNSIGNED NOT NULL AUTO_INCREMENT,
`parent_command` varchar(32) NOT NULL,
`sub_command` varchar(32) NOT NULL,
`access_level` int(11) UNSIGNED NOT NULL DEFAULT 0,
`top_level_aliases` varchar(255) NOT NULL,
PRIMARY KEY (`id`),
UNIQUE INDEX `command`(`parent_command`, 
`sub_command`));
