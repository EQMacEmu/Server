alter table character_data add column `forum_id` int(10) not null default 0 AFTER `account_id`;
alter table account add column `forum_id` int(10) not null default 0 AFTER `lsaccount_id`;