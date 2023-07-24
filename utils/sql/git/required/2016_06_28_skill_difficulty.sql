update skill_difficulty set difficulty = 4 where skillid in (69);
update skill_difficulty set difficulty = 3 where skillid in (61,63);
update skill_difficulty set difficulty = 2 where skillid in (64, 68, 60, 65, 57, 59, 58, 56, 9);

-- Fishing, Begging
update skill_difficulty set difficulty = 8 where skillid in (55);
update skill_difficulty set difficulty = 28 where skillid in (67);