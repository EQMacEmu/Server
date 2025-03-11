I suck at writing up directions, but to quickly summarize.

In the below examples, replace blahblah with the exact name of the file, which will be the date it was created.

New install at a mysql prompt:

create database whatever_db_name_you_want;
use whatever_db_name_you_made;
source alkabor_blahblah.sql;
source player_tables_blahblah.sql;
source login_tables_blahblah.sql;

-----------------------------------------------------

Upgrade install at a mysql prompt:

use whatever_db_name_you_made;
source drop_system.sql;
source alkabor_blahblah.sql;

Upgrading does not remove any user (character) data but will wipe all content data. So, if you added spawns, items, etc back them up before upgrading!

-----------------------------------------------------

Project home is:

http://www.takproject.net/forums/

Git for our code:

https://github.com/EQMacEmu/Server

Quest Git:

https://github.com/EQMacEmu/peqmacquests

PHP Database Editor:

https://github.com/EQMacEmu/takpphpeditor
