CREATE TABLE `account_name_reservation` (
    `account_id` int(10) unsigned NOT NULL,
    `name` varchar(64) NOT NULL,
    PRIMARY KEY (`account_id`, `name`) USING BTREE,
    INDEX `idx_name` (`name`) USING BTREE
) ENGINE = InnoDB
CHARACTER SET = utf8mb4
COLLATE = utf8mb4_unicode_ci
ROW_FORMAT = Dynamic;