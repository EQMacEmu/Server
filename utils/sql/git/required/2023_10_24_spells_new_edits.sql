-- Spell 36 'Gate'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 36;

-- Spell 55 'Cornucopia'
-- Spell 56 'Everfount'
UPDATE spells_new SET formula1 = 110 WHERE id IN (55, 56);

-- Spell 366 'Feign Death'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 366;

-- Spell 907 'DryBoneFireBurst'
UPDATE spells_new SET cast_time = 0 WHERE id = 907;

-- Spell 773 'Anc: Rytan's Dirge of Death'
UPDATE spells_new SET cast_on_you = 'You begin to sing, "I''m deeeeead! I''m deeeeead!"', cast_on_other = ' begins to sing, "I''m deeeaaddd! I''m deeead!"' WHERE id = 773;

-- Spell 1127 'Penance of Execution'
UPDATE spells_new SET cast_on_you = 'Dead man walking¿' WHERE id = 1127;
