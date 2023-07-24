alter table `character_data` drop `lfp`;
alter table `character_data` drop `lfg`;
alter table `character_data` drop `ability_time_seconds`;
alter table `character_data` drop `ability_number`;
alter table `character_data` drop `ability_time_minutes`;
alter table `character_data` drop `ability_time_hours`;
alter table `character_data` drop `ability_up`;
alter table `character_data` drop `leadership_exp_on`;
alter table `character_data` drop `toxicity`;
alter table `character_currency` drop `radiant_crystals`;
alter table `character_currency` drop `career_radiant_crystals`;
alter table `character_currency` drop `ebon_crystals`;
alter table `character_currency` drop `career_ebon_crystals`;
alter table `character_keyring` change `char_id` `id` int(11) not null default 0;
alter table `character_buffs` change `character_id` `id` int(10) not null default 0;

rename table `zone_flags` to `character_zone_flags`;
alter table `character_zone_flags` change `charID` `id` int(11) not null default 0;
rename table `inventory` to `character_inventory`;
alter table `character_inventory` change `charid` `id` int(11) not null default 0;
rename table `faction_values` to `character_faction_values`;
alter table `character_faction_values` change `char_id` `id` int(11) not null default 0;
rename table `timers` to `character_timers`;
alter table `character_timers` change `char_id` `id` int(11) not null default 0;

drop table if exists `sharedbank`;
drop table if exists `fear_hints`;
drop table if exists `buyer`;
drop table if exists `class_skill`;