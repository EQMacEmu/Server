ALTER TABLE `account` 
ADD COLUMN `revokeduntil` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00' AFTER `mule`;