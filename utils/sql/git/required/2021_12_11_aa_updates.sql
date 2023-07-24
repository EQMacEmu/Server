-- update some AA data

-- Magician Elemental Forms
UPDATE altadv_vars SET spell_refresh = 4320 WHERE skill_id IN (168,171,174,177); -- this column is unused by our AA code but just updating to avoid confusion
UPDATE aa_actions SET reuse_time = 4320 WHERE aaid IN (168,171,174,177); -- this is the value our server uses
-- Magician Host of the Elements
UPDATE altadv_vars SET spell_refresh = 1320 WHERE skill_id IN (616);
UPDATE aa_actions SET reuse_time = 1320 WHERE aaid IN (616);
-- Magician Frenzied Burnout
UPDATE altadv_vars SET spell_refresh = 4320 WHERE skill_id IN (167);
UPDATE aa_actions SET reuse_time = 4320 WHERE aaid IN (167);
-- Magician Turn Summoned
UPDATE altadv_vars SET spell_refresh = 4320, spellid = 2779, max_level = 1, cost = 5, prereq_skill = 92, prereq_minpoints = 3 WHERE skill_id IN (181);
UPDATE aa_actions SET reuse_time = 4320 WHERE aaid IN (181);
UPDATE aa_actions SET spell_id = 2779, target = 1 WHERE aaid = 181 AND rank = 0;
DELETE FROM aa_actions WHERE aaid = 181 AND rank = 1;
DELETE FROM aa_actions WHERE aaid = 181 AND rank = 2;
-- Magician Suspended Minion
DELETE FROM aa_actions WHERE aaid = 526 AND rank = 2;
UPDATE aa_actions SET spell_id = 3248, nonspell_action = 0 WHERE aaid = 526 AND rank = 0;
UPDATE aa_actions SET spell_id = 3249, nonspell_action = 0 WHERE aaid = 526 AND rank = 1;
-- Dire Charm
UPDATE aa_actions SET nonspell_action = 0 WHERE aaid = 145;
-- Rogue Purge Poison
UPDATE altadv_vars SET cost = 5 WHERE skill_id = 254;
-- Warrior Hastened Instigation
UPDATE altadv_vars SET cost = 3 WHERE skill_id = 489;
-- Cleric Hastened Turning
UPDATE altadv_vars SET max_level = 3, cost = 2, `type` = 5, spellid = 4294967295, prereq_skill = 133, prereq_minpoints = 1, aa_expansion = 4, classes = 4, class_type = 63, special_category = 4294967295 WHERE skill_id = 1039;
UPDATE aa_actions SET redux_aa = 1039, redux_rate = 10 WHERE aaid = 133;
-- Spell Casting Subtlety
UPDATE altadv_vars SET classes = 31008 WHERE skill_id = 98;
-- Spell Casting Deftness
UPDATE altadv_vars SET classes = 30752 WHERE skill_id = 104;
-- Quick Damage
UPDATE altadv_vars SET classes = 4160 WHERE skill_id = 141;
-- Suspended Minion
UPDATE altadv_vars SET classes = 60416 WHERE skill_id = 526;
-- Mental Clarity
UPDATE altadv_vars SET classes = 64892 WHERE skill_id = 1000;
-- Ferocity
UPDATE altadv_vars SET `type` = 5 WHERE skill_id = 564;
-- Wizard Mana Burn
UPDATE altadv_vars SET spell_refresh = 8640, prereq_skill = 1000 WHERE skill_id = 154;
UPDATE aa_actions SET reuse_time = 8640 WHERE aaid = 154;
-- Monk Purify Body
UPDATE altadv_vars SET spell_refresh = 4320 WHERE skill_id = 233;
UPDATE aa_actions SET reuse_time = 4320 WHERE aaid = 233;
-- Druid Spirit of the Wood
UPDATE altadv_vars SET spell_refresh = 1320 WHERE skill_id = 548;
-- Shaman Cannibalization
UPDATE altadv_vars SET prereq_skill = 1000 WHERE skill_id = 146;
-- Enchanter Gather Mana
UPDATE altadv_vars SET prereq_skill = 1000 WHERE skill_id = 162;
-- Magician Hastened Banishment
UPDATE altadv_vars SET prereq_skill = 181 WHERE skill_id = 486;
-- Wizard Fury of Magic Mastery
UPDATE altadv_vars SET prereq_minpoints = 1 WHERE skill_id = 640;
-- Rogue Hastened Purification
UPDATE aa_actions SET redux_aa = 501, redux_rate = 10 WHERE aaid = 254;

-- old PEQ table contained inaccurate data and not needed for TAKP
DROP TABLE IF EXISTS aa_required_level_cost;


