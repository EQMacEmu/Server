alter table doors add column `nokeyring` tinyint(3) not null default 1 AFTER `altkeyitem`;
update doors set nokeyring = 0 where id in (7739,7145);

CREATE TABLE `character_keyring` (
  `char_id` int(11) NOT NULL default '0',
  `item_id` int(11) NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;