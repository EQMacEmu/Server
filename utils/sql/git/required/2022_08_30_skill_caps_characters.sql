-- this creates a log table that we can reference later in case we made a mistake updating the values for a character
CREATE TABLE IF NOT EXISTS admin_log_character_skills_2022_08_30 (charid INT, charname VARCHAR(64), `level` INT, level2 INT, skill_id INT, skillname VARCHAR(32), skillval INT, skillcap INT, ts TIMESTAMP);

DROP PROCEDURE IF EXISTS sp_2022_08_03_skill_caps_characters;

DELIMITER //
CREATE PROCEDURE sp_2022_08_03_skill_caps_characters()
BEGIN
	DECLARE done INT DEFAULT FALSE;
	DECLARE charid INT;
	DECLARE charname VARCHAR(64);
	DECLARE charlevel INT;
	DECLARE charlevel2 INT;
	DECLARE skill_id SMALLINT;
	DECLARE skillname VARCHAR(32);
	DECLARE skillval INT;
	DECLARE skillcap INT;
	
	DECLARE cur CURSOR FOR 
		SELECT cd.id, cd.name, cd.level, cd.level2, cs.skill_id, sd.name, cs.value, s.cap 
		FROM character_data cd 
		JOIN character_skills cs ON cs.id = cd.id 
		JOIN skill_caps s ON cs.skill_id = s.skillid AND cd.level2 = s.level AND cd.class = s.class 
		JOIN skill_difficulty sd ON sd.skillid = s.skillid
		WHERE cs.skill_id NOT IN (50,42,40,39,29,27, 56,57,58,59, 60,61,63,64,65,68,69) 
		AND cs.value > s.cap 
		AND cd.gm = 0
		AND cd.account_id NOT IN (67514,67638,69005, 1,67394,67570,68151,68150,67877,67875,67688,67703,67995,67917,67906,67900,67885,67884,67879,67878,68322,68323,68324,68343,68388,68620,68621,68665,68786,68860,68880,68944,68945,68946,69008,69034,69508,69653,70261,70477,70572,71138,72252,72249,72519,72568,72641,72732,72989,73005,74151,73192,73639,73638,73637,73778,73706,73748,73749,73750,73780,73779,74152,88918)
		ORDER BY cd.account_id,cd.id,cs.skill_id;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

	START TRANSACTION;
	OPEN cur;

	processChar:LOOP
		FETCH cur INTO charid, charname, charlevel, charlevel2, skill_id, skillname, skillval, skillcap;
		IF done THEN
			LEAVE processChar;
		END IF;
		
		-- record the values in a log table before modifying
		INSERT admin_log_character_skills_2022_08_30 VALUES (charid, charname, charlevel, charlevel2, skill_id, skillname, skillval, skillcap, NULL);
		
		-- update skill value to match cap
		UPDATE character_skills cs SET `value` = skillcap WHERE cs.skill_id = skill_id AND cs.id = charid;
	END LOOP;

	CLOSE cur;

	COMMIT;
END //
DELIMITER ;

CALL sp_2022_08_03_skill_caps_characters();
DROP PROCEDURE IF EXISTS sp_2022_08_03_skill_caps_characters;

-- show the log table contents
SELECT * FROM admin_log_character_skills_2022_08_30;
