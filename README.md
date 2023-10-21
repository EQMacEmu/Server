**Instructions**
 - Follow instructions on EQEmu (https://github.com/EQEmu/Server/blob/master/README.md) as other than login we are using compatible EQEmu code.
 - Quest Git: https://github.com/SecretsOTheP/quests
 - Map Git: https://github.com/EQMacEmu/Maps

---

**Database Download**
 - Download & install MariaDB 10.3
 - Connect via favorite interface (CLI, HeidiSQL, etc)
 - Create a new database
 - Download latest [DB dump](https://github.com/SecretsOTheP/EQMacEmu/tree/main/utils/sql/database_full)
 - Unzip DB dump (7zip on windows)
 - Run the SQL files on your new database

---

**Local Testing on PC**

<a href="https://youtu.be/Hjrzdlem3ZA"><img src="https://cdn.freebiesupply.com/logos/large/2x/youtube-icon-logo-png-transparent.png" alt="Youtube Setup Guide" width="200" height="133"></a>

[Watch Getting Started Guide](https://youtu.be/Hjrzdlem3ZA)

Basic Guide Details:

 - Fork: https://github.com/nickgal/EQMacDocker
 - Fork: https://github.com/SecretsOTheP/EQMacEmu
 - Fork: https://github.com/SecretsOTheP/quests
 <br>
 
 - Clone your EQMacDocker repo
 - Merge in PRs #2 and #3 from nickgal's repo if they aren't included. 
   - [Update the reference for quarm db to latest version](https://github.com/nickgal/EQMacDocker/pull/3)
   - [Update environment variables to reference quarm](https://github.com/nickgal/EQMacDocker/pull/2)
 - Change the submodules to point to your forked versions of the Server and Quests submodules
 <br>
 
 - Install WSL 2 
 - Install Docker Desktop
 - Install Docker Compose
 <br>
 
 - Follow readme instructions in EQMacDocker repo to start the server
 - Once connected to the database, run the .sql scripts that are found in Server/utils/sql/git/required
   - Only need to run these if you're having trouble logging in
 <br>
 
 - Download the TAKP client and install into a new dev folder
 - Add the quarm .dll
 - Change eqhost.txt to point to 127.0.0.1 for the login server
 <br>
 
 - Run the game
<br>

 - For common issues and fixes please reference the Getting Started Guide above
