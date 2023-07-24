alter table `spawn2` add column force_z tinyint(3) not null default 0;
update spawn2 set force_z = 1 where id in (364841, 364842);