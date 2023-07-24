alter table spawn2 add column `boot_respawntime` int(11) not null default 0;
alter table spawn2 add column `clear_timer_onboot` tinyint(3) not null default 0;
alter table spawnentry add column `mintime` smallint(4) not null default 0;
alter table spawnentry add column `maxtime` smallint(4) not null default 0;

alter table `doors` add column `altkeyitem` int(11) not null default 0 AFTER `keyitem`;
