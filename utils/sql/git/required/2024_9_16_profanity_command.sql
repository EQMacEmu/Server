DROP TABLE IF EXISTS `profanity_list`;

CREATE TABLE `profanity_list` (
	`word` VARCHAR(16) NOT NULL
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
;