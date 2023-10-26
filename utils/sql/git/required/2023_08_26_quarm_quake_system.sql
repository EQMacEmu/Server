CREATE TABLE `quake_data`  (
  `start_timestamp` int(11) UNSIGNED NOT NULL DEFAULT 0,
  `next_timestamp` int(11) UNSIGNED NOT NULL DEFAULT 0,
  `ruleset` int(11) NOT NULL DEFAULT 0
);

INSERT INTO `rule_values`(`ruleset_id`, `rule_name`, `rule_value`, `notes`) VALUES (1, 'Quarm:EnableQuakes', 'true', 'Enable quakes');
INSERT INTO `rule_values`(`ruleset_id`, `rule_name`, `rule_value`, `notes`) VALUES (1, 'Quarm:QuakeMinVariance', '604800', 'Min Quake Variance');
INSERT INTO `rule_values`(`ruleset_id`, `rule_name`, `rule_value`, `notes`) VALUES (1, 'Quarm:QuakeMaxVariance', '864000', 'Max Quake Variance');
INSERT INTO `rule_values`(`ruleset_id`, `rule_name`, `rule_value`, `notes`) VALUES (1, 'Quarm:QuakeRepopDelay', '900', 'NPC Repop Delay');
INSERT INTO `rule_values`(`ruleset_id`, `rule_name`, `rule_value`, `notes`) VALUES (1, 'Quarm:QuakeEndTimeDuration', '84600', 'End Time Duration');
