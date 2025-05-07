-- we don't have an easy way to override the range for an npc spell but unfortunately this spell has its range set to 0 in spells_en.txt.
UPDATE spells_new SET `range` = 60 WHERE id = 915;
