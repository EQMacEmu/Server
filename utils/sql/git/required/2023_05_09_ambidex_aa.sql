-- Update Ambidexterity from 9 to 32 to match changed calculation in Mob::GetDualWieldChance()
UPDATE aa_effects SET base1 = 32 WHERE aaid = 198 AND slot = 1;
