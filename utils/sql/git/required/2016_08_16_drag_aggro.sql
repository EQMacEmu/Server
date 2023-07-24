ALTER TABLE `zone` ADD column `dragaggro` tinyint(3) NOT NULL DEFAULT '0';

UPDATE zone SET dragaggro = '1' WHERE short_name = 'kael';
UPDATE zone SET dragaggro = '1' WHERE short_name = 'skyshrine';
UPDATE zone SET dragaggro = '1' WHERE short_name = 'templeveeshan';
UPDATE zone SET dragaggro = '1' WHERE short_name = 'mischiefplane';
UPDATE zone SET dragaggro = '1' WHERE short_name = 'thurgadinb';