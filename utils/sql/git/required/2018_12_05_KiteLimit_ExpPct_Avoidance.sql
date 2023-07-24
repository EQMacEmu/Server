alter table npc_types add column `avoidance` smallint(5) not null default 0;
alter table npc_types add column `exp_pct` smallint(5) unsigned not null default 100;
alter table zone add column `pull_limit` smallint(5) unsigned not null default 80;
