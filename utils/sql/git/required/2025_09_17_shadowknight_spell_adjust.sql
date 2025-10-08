-- these spells were previously altered to work around an error in the code.  the code has been fixed and these need to be changed back to match the spell file.

-- Spell 1376 'Shroud of Undeath'
UPDATE spells_new SET effect_base_value1 = 1470 WHERE id = 1376;

-- Spell 1459 'Shroud of Death'
UPDATE spells_new SET effect_base_value1 = 1470 WHERE id = 1459;

-- Spell 2576 'Mental Corruption'
UPDATE spells_new SET effect_base_value1 = 2716 WHERE id = 2576;

-- Spell 3227 'Shroud of Chaos'
UPDATE spells_new SET effect_base_value1 = 3227 WHERE id = 3227;
