alter table faction_list drop column `old_base`;
alter table faction_list add column `min_cap` smallint(6) not null default -2000;
alter table faction_list add column `max_cap` smallint(6) not null default 2000;
