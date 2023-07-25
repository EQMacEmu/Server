DROP TABLE IF EXISTS tblloginserversettings;
CREATE TABLE `tblloginserversettings` (
	`type` VARCHAR(50) NOT NULL COLLATE 'latin1_swedish_ci',
	`value` VARCHAR(50) NULL DEFAULT NULL COLLATE 'latin1_swedish_ci',
	`category` VARCHAR(20) NOT NULL COLLATE 'latin1_swedish_ci',
	`description` VARCHAR(99) NOT NULL COLLATE 'latin1_swedish_ci',
	`defaults` VARCHAR(50) NULL DEFAULT NULL COLLATE 'latin1_swedish_ci'
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
;

INSERT INTO `tblloginserversettings` VALUES ('allow_PCT', 'TRUE', 'options', 'allow PC ticketed client', 'TRUE');
INSERT INTO `tblloginserversettings` VALUES ('allow_OSX', 'TRUE', 'options', 'allow OSX client', 'TRUE');
INSERT INTO `tblloginserversettings` VALUES ('allow_PC', 'TRUE', 'options', 'allow PC client', 'TRUE');
INSERT INTO `tblloginserversettings` VALUES ('pop_count', '0', 'options', '0 to only display UP or DOWN or 1 to show population count in server select.', '0');
INSERT INTO `tblloginserversettings` VALUES ('ticker', 'Welcome to EQMacEmu', 'options', 'Sets the welcome message in server select.', 'Welcome to EQMacEmu');
