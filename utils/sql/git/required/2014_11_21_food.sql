alter table character_data add column `famished` int(11) not null default 0;
replace into rule_values (rule_name, rule_value, notes) values ("Character:FoodLossPerUpdate", 75,"");