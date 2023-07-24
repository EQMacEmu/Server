SELECT * FROM qs_player_trade_record
INTO OUTFILE '/tmp/qs_player_trade_record.txt'
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n';

SELECT * FROM qs_player_trade_record_entries
INTO OUTFILE '/tmp/qs_player_trade_record_entries.txt'
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n';

SELECT * FROM qs_player_handin_record
INTO OUTFILE '/tmp/qs_player_handin_record.txt'
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n';

SELECT * FROM qs_player_handin_record_entries
INTO OUTFILE '/tmp/qs_player_handin_record_entries.txt'
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n';

SELECT * FROM qs_player_npc_kill_record
INTO OUTFILE '/tmp/qs_player_npc_kill_record.txt'
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n';

SELECT * FROM qs_player_npc_kill_record_entries
INTO OUTFILE '/tmp/qs_player_npc_kill_record_entries.txt'
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n';

SELECT * FROM qs_merchant_transaction_record
INTO OUTFILE '/tmp/qs_merchant_transaction_record.txt'
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n';

SELECT * FROM qs_merchant_transaction_record_entries
INTO OUTFILE '/tmp/qs_merchant_transaction_record_entries.txt'
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n';

SELECT * FROM qs_player_delete_record
INTO OUTFILE '/tmp/qs_player_delete_record.txt'
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n';

SELECT * FROM qs_player_delete_record_entries
INTO OUTFILE '/tmp/qs_player_delete_record_entries.txt'
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n';

SELECT * FROM qs_player_move_record
INTO OUTFILE '/tmp/qs_player_move_record.txt'
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n';

SELECT * FROM qs_player_move_record_entries
INTO OUTFILE '/tmp/qs_player_move_record_entries.txt'
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n';

drop table if exists `qs_player_trade_record`;
drop table if exists `qs_player_trade_record_entries`;
drop table if exists `qs_player_handin_record`;
drop table if exists `qs_player_handin_record_entries`;
drop table if exists `qs_player_npc_kill_record`;
drop table if exists `qs_player_npc_kill_record_entries`;
drop table if exists `qs_merchant_transaction_record`;
drop table if exists `qs_merchant_transaction_record_entries`;
drop table if exists `qs_player_delete_record`;
drop table if exists `qs_player_delete_record_entries`;
drop table if exists `qs_player_move_record`;
drop table if exists `qs_player_move_record_entries`;
