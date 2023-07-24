alter table doors add column `close_time` int(11) not null default 5;
update doors set close_time = 9 where zone = "sleeper" AND name in ("KELSEYDOOR", "KELSEYDOOR2");
update doors set close_time = 20 where islift = 1;
update doors set close_time = 0 where islift = 1 and door_param > 100;

alter table doors add column `can_open` tinyint(3) not null default 1;
update doors set can_open = 0 where opentype = 145 or name like "MRU%" or name like "%TENT" or name like "BANDIT%" or name = "CAMPFIRE" or name like "DRAWERS%" or name like "%TABLE%" or name = "KEG" or name = "TIKI" or name like "CRATE%" or name like "TORCH%" or name like "OBJ%" or name like "ORC%" or name like "%LAMP" or name = "SHIP%" or name like "%SHIELD%" or name like "%BLADE%" or name like "%RACK%" or name like "%CHAIR%" or name like "%SHELF%" or name like "%DESK%" or name like "%BARREL%" or name like "%BRAZIER%";
update doors set can_open = 1 where opentype in (120, 125, 130, 140);
update doors set can_open = 1 where dest_x > 0 or dest_y > 0 or dest_z > 0 or triggerdoor > 0 or lockpick != 0 or keyitem > 0;