-- Ayonae's Tutelage
INSERT aa_effects (aaid, slot, effectid, base1, base2) VALUES (571, 0, 261, 1, 0)
INSERT aa_effects (aaid, slot, effectid, base1, base2) VALUES (572, 0, 261, 2, 0)
INSERT aa_effects (aaid, slot, effectid, base1, base2) VALUES (573, 0, 261, 3, 0)
-- Ferocity
UPDATE altadv_vars SET cost_inc = 0, level_inc = 2 WHERE skill_id = 564
-- Advanced Healing Gift
UPDATE altadv_vars SET cost_inc = 1, level_inc = 1 WHERE skill_id = 437

