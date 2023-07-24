-- instant spells don't need this
UPDATE spells_new SET nodispell = 0 WHERE nodispell = 1 AND buffdurationformula = 0;

-- some recourses and familiars
UPDATE spells_new SET nodispell = 0 WHERE id IN (1361, 2464, 2468, 2473, 2475, 2553, 2555, 2557, 2560, 2758, 3264, 3404, 3774, 3980);

-- this is also checked in code but updating the flag to match what the code does
UPDATE spells_new SET nodispell = 1 WHERE id IN (323, 960, 1357, 1720, 2736, 2737, 3603, 3769);
