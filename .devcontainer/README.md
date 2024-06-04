# Dev Container Linux Setup

Based off of https://docs.eqemu.io/server/installation/dev-container/

Dev containers uses Docker and VSCode to create a development environment that is consistent across all developers. This is a great way to ensure that everyone is using the same tools and versions. You can also use this to develop on Windows, Mac, or Linux. The entire project directory is mounted within the VSCode dev container yet build and runtime files reside within the project directory on the host. This means building, submodules, and configuration persist across docker container reboots, image deletion, and even volume deletion. Database data does not persist after volume deletion.


## Prerequisites
1. Install [Docker](https://www.docker.com/products/docker-desktop)
2. Install [VSCode](https://code.visualstudio.com/)


### Setup 1: Initialize repo and dev container
1. Clone this repo
2. Open the repository in VSCode. It should auto-prompt to reopen in a container as VSCode detects the .devcontainer folder. If not, select the menu in bottom left corner and selct "Reopen in Container".
3. Wait for the container to build and start. VSCode will auto-prompt to view container logs in terminal.
4. Once complete, select New-Terminal (Ctr-Shift-\`) which will open a shell within the container. You know you're in the container if the prompt looks like... `vscode -> /src`


### Setup 2: Building
1. Execute `make cmake` to configure the environment
2. Execute `make build` to build - may take a little bit.
3. Verify your `build/bin` folder within VSCode or `/src/build/bin` within the terminal for binaries (zone, world, queryserv, etc.)


### Setup 3: Inject the Database
1. Execute `make inject-mariadb` to DROP and CREATE the database and source the latest backup, with a few extra sql files to facilitate local dev.

	* - A sql file is executed to enable auto-creation of accounts at the login server.


### Setup 4: Clone Quests and Maps
1. Execute `make quests` and `make maps` to clone the repos or pull latest if they exist.


### Setup 5: Prep the environment
1. Execute `make prep` which will create a runtime location in `/src/.devcontainer/bin`. This will copy static config files and symlink binaries. Cleaning the project's build dir will not require re-running above steps. If you do modify values in `/src/.devcontainer/eqemu_config.json` then you need to rerun this command.


### Setup 6: Start the server
1. Execute `make start` to launch the server stack


### Setup 7: Log in and set yourself GM
1. Be sure your EQ client's eqhosts.txt file matches `/src/.devcontainer/base/login.ini` which defaults to `"127.0.0.1:6000"`.
2. Enter in any username/password (twice) to auto-create an account and login.
3. To set your account as a GM, execute `make gm-accountname`. You can do it prior to logging in.


### Setup 8: Connect to Database
1. Use preferred SQL client to connect to the database based on parameters in `/src/.devcontainer/eqemu_config.json`
2. You may also connect within the dev container terminal via `mysql -h127.0.0.1`


### Debugging
You can optionally debug by attaching via gdb to the running processes. This is only recommended if you are familiar with gdb and debugging in general.
1. Add breakpoints in zone or world that you want to debug
2. Press CTRL+SHIFT+D or the debug icon on left pane, and select a (gdb) [processType] for what you want to debug
3. When your code reaches said break point, you will get a call stack and variable dump on left
4. While the break point is triggered, all code is frozen, so a connection client will slowly disconnect if you're not quick to resume the breakpoint


### Troubleshooting
- Errors during image creation and container boot will be shown as logs within the VSCode terminal. Sometimes restarting the docker daemon (Docker Desktop in Windows) can resolve errors.
- Errors during any make task (such as building) will appear as logs within VSCode terminal.
- Errors during server startup and runtime will appear as logs within VSCode terminal and are persisted in log files under `/src/.devcontainer/bin/logs`
- Specific zone crashes are also logged under logs directory.


### Cleaning 
- Execute `make clean` to clean C++ binaries in `/src/build/bin`. Step 2 above reverts this.
- Execute `make clean-assets` to remove downloaded assets from `/src/.devcontainer/base`. Step 3 & 4 above reverts this, which means this will drop your database.
- Execute `make clean-runtime` to clean runtime files in `/src/.devcontainer/bin/`, such as config and logs. Step 5 above reverts this.
- Execute `make clean-all` to execute above clean tasks, good for starting fresh.
- If you want to start completely fresh, or want to remove anything docker downloaded for this you can delete all containers, images, and volumes. This will drop your database. If on Windows, you can use Docker on Desktop GUI. This will drop all other docker data you have.
	- `docker rm -f $(docker ps -aq)` to remove all containers
	- `docker rmi -f $(docker images -aq)` to remove all images
	- `docker volume rm $(docker volume ls -q)` to remove all volumes (this will drop your database)
