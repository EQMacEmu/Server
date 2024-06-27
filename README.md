## EQ Mac - The Al'kabor Project

EQMacEmu is an EverQuest server emulating the original Macintosh version called Al`kabor.

- [Project Website](https://www.takproject.net/)
- [Project Wiki](https://wiki.takp.info/index.php/Main_Page)
- [How to play](https://wiki.takp.info/index.php/Getting_Started)

---

For developers, and advanced users, the entire server stack and content is all open source. This project was originally forked from [EQEmu](https://github.com/EQEmu) but has diverged, however the [documentation](https://docs.eqemu.io/) still has many relevant parts and is overall a good place for information. 


- For a traditional, manual, Windows installation for developing and running the server. You may use EQEmu documentation [for a manual Windows install](https://docs.eqemu.io/server/installation/ground-up-windows-install/). You would use this if you want to learn the most and don't mind directly installing all dependencies on your Windows computer. You would also follow this if you would like to use Visual Studios for C++ development.

- For a manual Linux installation you should just follow the scripts used in the Docker Hub or devcontainer repos described below.

- For an automatic, simplified, process to quickly stand up a server there is a ready-to-play Docker image at [Docker Hub Repository](https://github.com/jcon321/EQMacEmuDockerHub). You would use this if you want to quickly stand up a server for local or LAN play. Since this is docker, no dependencies or additional programs are installed on your computer. This supports Windows and Linux environments.

- For an automatic development environment using vscode there is a preconfigured [devcontainer](https://github.com/EQMacEmu/Server/tree/main/.devcontainer) available. You would use this if you want to quickly stand up a development environment that is configured for building and deploying. 

&nbsp;

Additional repositories related to the server stack
- [Quest Repository](https://github.com/EQMacEmu/quests)
- [Maps Repository](https://github.com/EQMacEmu/Maps)
- [Database Dump](https://github.com/EQMacEmu/Server/tree/main/utils/sql/database_full)
