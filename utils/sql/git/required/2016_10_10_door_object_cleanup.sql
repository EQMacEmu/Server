alter table doors drop column `guild`;
alter table zone_points drop column `buffer`;
alter table object_contents drop column `augslot1`;
alter table object_contents drop column `augslot2`;
alter table object_contents drop column `augslot3`;
alter table object_contents drop column `augslot4`;
alter table object_contents drop column `augslot5`;

alter table object change `unknown08` `size` mediumint(5) not null default 0;
alter table object change `unknown10` `solid` mediumint(5) not null default 0;
alter table object change `unknown20` `incline` int(11) not null default 0;
alter table object drop column `unknown24`;
alter table object drop column `unknown60`;
alter table object drop column `unknown64`;
alter table object drop column `unknown68`;
alter table object drop column `unknown72`;
alter table object drop column `unknown76`;
alter table object drop column `unknown84`;
update object set size = 0, solid = 0, incline = 0; -- These columns all seem to have junk data. Todo: Check packets for values.

alter table character_inventory drop column `augslot1`;
alter table character_inventory drop column `augslot2`;
alter table character_inventory drop column `augslot3`;
alter table character_inventory drop column `augslot4`;
alter table character_inventory drop column `augslot5`;
