alter table character_corpses add column `gmexp` int(11) not null default 0 AFTER `exp`;
update character_corpses set gmexp = exp where exp > 0;
insert into rule_values values (1, "Character:EmptyCorpseDecayTimeMS", 10800000, "");