alter table character_corpses add column `rezzable` tinyint(11) not null default 1;
alter table character_corpses_backup add column `rezzable` tinyint(11) not null default 1;