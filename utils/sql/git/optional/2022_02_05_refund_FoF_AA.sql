DROP PROCEDURE IF EXISTS sp_2022_02_05_refund_FoF_AA;

DELIMITER //
CREATE PROCEDURE sp_2022_02_05_refund_FoF_AA()
BEGIN

	DECLARE done INT DEFAULT FALSE;
	DECLARE charid INT;
	DECLARE ability_id INT;
	DECLARE refund_points INT;
	DECLARE ability2 CURSOR FOR SELECT id, aa_id FROM character_alternate_abilities WHERE aa_id IN (628, 629);
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

	START TRANSACTION;

	OPEN ability2;

	processChar:LOOP
		FETCH ability2 INTO charid, ability_id;
		IF done THEN
			LEAVE processChar;
		END IF;
		SET refund_points = CASE WHEN ability_id = 629 THEN 6 ELSE 2 END;
		
		UPDATE character_data SET aa_points = aa_points + refund_points WHERE id = charid;
		DELETE FROM character_alternate_abilities WHERE id = charid AND aa_id = ability_id;
	END LOOP;

	CLOSE ability2;

	COMMIT;
END //
DELIMITER ;

CALL sp_2022_02_05_refund_FoF_AA();
DROP PROCEDURE IF EXISTS sp_2022_02_05_refund_FoF_AA;
