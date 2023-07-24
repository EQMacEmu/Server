alter table spawngroup add column `rand_spawns` int(11) not null default 0;
alter table spawngroup add column `rand_respawntime` int(11) not null default 1200;
alter table spawngroup add column `rand_variance` int(11) not null default 0;
alter table spawngroup add column `rand_condition_` int(11) not null default 0;