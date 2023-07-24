alter table npc_types add column `chesttexture` tinyint(2) not null default 0 AFTER `raid_target`;
update npc_types set chesttexture = texture WHERE armtexture != 0 OR bracertexture != 0 OR handtexture != 0 OR legtexture != 0 OR feettexture !=0;
update npc_types set texture = 0 where chesttexture != 0;
update npc_types set texture = 255 WHERE armtexture != 0 OR bracertexture != 0 OR handtexture != 0 OR legtexture != 0 OR feettexture !=0;
update npc_types set texture = chesttexture WHERE chesttexture = armtexture AND chesttexture = bracertexture AND chesttexture = handtexture AND chesttexture = legtexture AND chesttexture = feettexture AND chesttexture != 0;