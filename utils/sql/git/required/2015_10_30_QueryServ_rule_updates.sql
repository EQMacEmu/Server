INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('1', 'QueryServ:PlayerLogAAPurchases', 'false');
INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('2', 'QueryServ:PlayerLogAAPurchases', 'false');
INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('11', 'QueryServ:PlayerLogAAPurchases', 'false');

INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('1', 'QueryServ:PlayerLogDeaths', 'false');
INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('2', 'QueryServ:PlayerLogDeaths', 'false');
INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('11', 'QueryServ:PlayerLogDeaths', 'false');

INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('1', 'QueryServ:PlayerLogTradeSkillEvents', 'false');
INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('2', 'QueryServ:PlayerLogTradeSkillEvents', 'false');
INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('11', 'QueryServ:PlayerLogTradeSkillEvents', 'false');

INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('1', 'QueryServ:PlayerLogQGlobalUpdate', 'false');
INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('2', 'QueryServ:PlayerLogQGlobalUpdate', 'false');
INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('11', 'QueryServ:PlayerLogQGlobalUpdate', 'false');

INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('1', 'QueryServ:PlayerLogMerchantTransactions', 'false');
INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('2', 'QueryServ:PlayerLogMerchantTransactions', 'false');
INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('11', 'QueryServ:PlayerLogMerchantTransactions', 'false');

INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('1', 'QueryServ:PlayerLogLoot', 'false');
INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('2', 'QueryServ:PlayerLogLoot', 'false');
INSERT INTO `rule_values` (`ruleset_id`, `rule_name`, `rule_value`) VALUES ('11', 'QueryServ:PlayerLogLoot', 'false');

DELETE FROM `rule_values` WHERE `rule_name`='QueryServ:MerchantLogTransactions';