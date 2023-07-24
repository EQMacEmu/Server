-- Magician Elemental Forms
UPDATE altadv_vars SET spell_refresh = 900 WHERE skill_id IN (168,171,174,177); -- this column is unused by our AA code but just updating to avoid confusion
UPDATE aa_actions SET reuse_time = 900 WHERE aaid IN (168,171,174,177); -- this is the value our server uses

