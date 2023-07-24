alter table spells_new add column `disabled` tinyint(3) not null default 0 AFTER `not_player_spell`;
update spells_new set disabled = 1 where id in (546, 565, 1375);