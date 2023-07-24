-- these effects have specific logic in Client::CalcMaxHP and don't use simple bonus values anymore
UPDATE aa_effects SET base1 = 0 WHERE aaid IN (107,108,109,279,423,424,425) AND effectid = 214;
