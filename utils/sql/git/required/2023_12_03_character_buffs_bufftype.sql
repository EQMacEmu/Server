ALTER TABLE character_buffs ADD bufftype INT NOT NULL AFTER bard_modifier;
UPDATE character_buffs SET bufftype = 2;
