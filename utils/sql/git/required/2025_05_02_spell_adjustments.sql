-- these spells previously had their data altered to make the duration 7500 but this effect was changed to always be 7500 duration as seen in client decompiles so this script just restores the original values

-- 303 'Whirl till you hurl'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 303;

-- 619 'Dyn`s Dizzying Draught'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 619;

-- 806 'Magi Curse'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 806;

-- 826 'Whirlwind'
UPDATE spells_new SET effect_base_value2 = 1 WHERE id = 826;

-- 896 'Vortex'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 896;

-- 899 'Whirl until you hurl'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 899;

-- 1010 'Clockwork Poison'
UPDATE spells_new SET effect_base_value4 = 1 WHERE id = 1010;

-- 1035 'Lure of the Storm'
UPDATE spells_new SET effect_base_value4 = 1 WHERE id = 1035;

-- 1044 'Rage of the Rainkeeper'
UPDATE spells_new SET effect_base_value2 = 1 WHERE id = 1044;

-- 1101 'Spin the Bottle'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 1101;

-- 1246 'Magi Curse'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 1246;

-- 1380 'Stream of Acid'
UPDATE spells_new SET effect_base_value2 = 1 WHERE id = 1380;

-- 1478 'Bellowing Winds'
UPDATE spells_new SET effect_base_value3 = 1 WHERE id = 1478;

-- 1495 'Rodricks Gift'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 1495;

-- 1809 'One Hundred Blows'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 1809;

-- 2059 'Death Roll'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 2059;

-- 2096 'Crushing Fist of the Wind'
UPDATE spells_new SET effect_base_value2 = 1 WHERE id = 2096;

-- 2108 'Donlo's Dance Party'
UPDATE spells_new SET effect_base_value4 = 6000 WHERE id = 2108;

-- 2132 'Acidic Current'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 2132;

-- 2265 'Jaydrox Rocks'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 2265;

-- 2810 'Glowing Glyphs'
UPDATE spells_new SET effect_base_value3 = 1 WHERE id = 2810;

-- 2827 'Golem Smash'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 2827;

-- 2829 'Golem Smash2'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 2829;

-- 2933 'Fungal Spore Cloud'
UPDATE spells_new SET effect_base_value4 = 1 WHERE id = 2933;

-- 2953 'Cral Ligi Attack'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 2953;

-- 2954 'Cral Ligi Attack'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 2954;

-- 2955 'Attack of the Cral Ligi'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 2955;

-- 2981 'Diminutive Stature'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 2981;

-- 3051 'Fiery Assault'
UPDATE spells_new SET effect_base_value3 = 1 WHERE id = 3051;

-- 3155 'Elemental Judgement'
UPDATE spells_new SET effect_base_value3 = 1 WHERE id = 3155;

-- 3674 'Turmoil of Charyb'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 3674;

-- 3675 'Vortex of Cetacea'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 3675;

-- 3676 'Whirlpool'
UPDATE spells_new SET effect_base_value1 = 1 WHERE id = 3676;

