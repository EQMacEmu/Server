alter table `doors` add column `islift` tinyint(3) not null default 0;
update doors set islift = 1 where id in (7274, 7301, 7302, 2039, 97, 98, 5037, 5038, 5039, 5041, 5042, 5043, 5044, 5045, 5046);