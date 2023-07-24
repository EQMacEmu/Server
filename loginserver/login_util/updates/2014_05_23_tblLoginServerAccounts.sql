alter table tblLoginServerAccounts add column `client_unlock` tinyint(3) NOT NULL DEFAULT '1';
alter table tblLoginServerAccounts add column `created_by` tinyint(3) NOT NULL DEFAULT '0';