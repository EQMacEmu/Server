alter table npc_types change `see_hide` `see_sneak` tinyint(4) not null default 0;
update npc_types set see_sneak = 0;