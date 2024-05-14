NAME := eqemu-server

DOCKER_ARGS := --rm  --name ${NAME} -v $$PWD:/src -w /src ${NAME}
DOCKER_ARM64_ARGS := --rm --platform linux/arm64 --name ${NAME}-arm64 -v $$PWD:/src -w /src ${NAME}-arm64

.PHONY: build
build:
	cd build$$BUILD_SUFFIX && cmake --build . --config Release --target all --

.PHONY: cmake
cmake:
	@echo "working directory: $$PWD"
	mkdir -p build$$BUILD_SUFFIX
	@cd build$$BUILD_SUFFIX && cmake -DEQEMU_BUILD_LOGIN=ON \
			-DEQEMU_BUILD_TESTS=ON \
			-DCMAKE_CXX_COMPILER_LAUNCHER=ccache -G Ninja ..

clean:
	rm -rf build

docker-cmake:
	@echo "working directory: $$PWD"
	git submodule update --init --recursive
	docker run ${DOCKER_ARGS} make cmake

docker-build:
	docker run ${DOCKER_ARGS} make build

# Build image if it doesn't exist
docker-image-build:
ifeq ($(shell docker images -q ${NAME} 2> /dev/null),)
	@echo "Docker image not found. Building..."
	docker build -f Dockerfile.debian.dev -t ${NAME} .
endif

docker-arm-cmake: docker-arm-image-build
	git submodule update --init --recursive
	docker run ${DOCKER_ARM64_ARGS} make cmake BUILD_SUFFIX=arm64

docker-arm-build: docker-arm-image-build
	docker run ${DOCKER_ARM64_ARGS} make build BUILD_SUFFIX=arm64

docker-arm-image-build:
ifeq ($(shell docker images -q ${NAME}-arm64 2> /dev/null),)
	@echo "Docker image not found. Building..."
	docker build -f Dockerfile.debian.arm.dev -t ${NAME}-arm64 .
endif

docker-clean: clean

.PHONY: prep
prep:
	@echo "Preparing build/bin for usage..."
	mkdir -p build/bin/assets/patches
	cp -R -u -p .devcontainer/base/eqemu_config.json build/bin/eqemu_config.json
	cp -R -u -p .devcontainer/base/login.ini build/bin/login.ini
	cp -R -u -p loginserver/login_util/* build/bin/assets/patches/
	mkdir -p build/bin/assets
	cp -R -u -p utils/patches build/bin/assets/
	@-ln -s .devcontainer/base/assets/ build/bin/
	@-cd build/bin && ln -s assets/patches/login_opcodes_oldver.conf login_opcodes_oldver.conf
	-unlink build/bin/lua_modules
	cd build/bin && ln -s quests/lua_modules lua_modules
	-unlink build/bin/mods
	cd build/bin && ln -s quests/mods mods
	-unlink build/bin/maps
	cd build/bin && ln -s ../../base/maps maps
	mkdir -p build/bin/logs
	mkdir -p build/bin/shared
	@echo "Eqemu is prepared. Edit build/bin/eqemu_config.json to configure."

maps:
	@cd .devcontainer/base && git clone https://github.com/EQMacEmu/Maps maps

quests:
	@cd build/bin && git clone https://github.com/EQMacEmu/quests.git quests

# Runs tests
.PHONY: test
test:
	cd build/bin && ./tests

# Runs login binary
.PHONY: login
login:
	cd build/bin && ./loginserver

# Runs shared_memory binary
.PHONY: shared
shared:
	@cd build/bin && mkdir -p shared
	cd build/bin && ./shared_memory

# Runs zone binary
.PHONY: zone
zone:
	@-rm build/bin/logs/zone/zone*.log
	cd build/bin && ./zone

# Runs world binary
.PHONY: world
world:
	@-rm build/bin/logs/world*.log
	cd build/bin && ./world

# Runs ucs binary
.PHONY: ucs
ucs:
	@-rm build/bin/logs/ucs*.log
	cd build/bin && ./ucs

# Runs queryserv binary
.PHONY: queryserv
queryserv:
	@-rm build/bin/logs/query_server*.log
	cd build/bin && ./queryserv

valgrind-%:
	cd build/bin && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=logs/$*.valgrind.log ./$*

# Start mariaDB standalone
.PHONY: mariadb
mariadb:
	@sudo service mariadb start

.PHONY: inject-mariadb
inject-mariadb:
	-sudo service mariadb start
	-mkdir -p .devcontainer/base/db/
	-sudo mariadb -e 'DROP DATABASE IF EXISTS peq;'
	-sudo mariadb -e 'CREATE DATABASE peq;'
	-sudo mariadb -e "CREATE USER IF NOT EXISTS 'peq'@'127.0.0.1' IDENTIFIED BY 'peqpass';"
	-sudo mariadb -e "GRANT ALL PRIVILEGES ON *.* TO 'peq'@'127.0.0.1';"
ifeq (,$(wildcard .devcontainer/base/db/db.sql.zip))
	@echo ".devcontainer/base/db.sql.zip not found. Downloading latest from https://db.projecteq.net/"
	wget -nc https://github.com/EQMacEmu/Server/raw/main/utils/sql/database_full/alkabor_2024-04-21-15_36.tar.gz -O .devcontainer/base/db/db.sql.zip
	@-cd .devcontainer/base/db && unzip db.sql.zip
endif
	@echo "Sourcing db may take a while, please wait..."
	@cd .devcontainer/base/db/alkabor_2024-04-21-15_36 && sudo mariadb --database peq -e "source alkabor_2024-04-21-15_36.sql"
	@cd .devcontainer/base/db/alkabor_2024-04-21-15_36 && sudo mariadb --database peq -e "source data_tables_2024-04-21-15_36.sql"
	@cd .devcontainer/base/db/alkabor_2024-04-21-15_36 && sudo mariadb --database peq -e "source login_tables_2024-04-21-15_36.sql"
	@cd .devcontainer/base/db/alkabor_2024-04-21-15_36 && sudo mariadb --database peq -e "source player_tables_2024-04-21-15_36.sql"
	@cd utils/sql/git/required/ && sudo mariadb --database peq -e "source 2024_2_10_server_scheduled_events.sql"

	@echo "MariaDB is now injected."


.PHONY: gm-%
gm-%:
	sudo mariadb --database peq -e "UPDATE account SET status=255 WHERE name = '$*';"
	@echo "Account $* is now a GM. /camp to have it go into effect."

depends:
	sudo apt install graphviz pip time
	pip3 install graphviz
	mkdir -p build/depends
	@if [ ! -f "build/depends/dependency_graph.py" ]; then \
		wget https://raw.githubusercontent.com/pvigier/dependency-graph/master/dependency_graph.py -O build/depends/dependency_graph.py; \
	fi
	@echo "Generating dependency graphs (This may take a while)..."
	@echo "Login..."
	time python3 build/depends/dependency_graph.py -f png login build/depends/login.dot
	@echo "World..."
	time python3 build/depends/dependency_graph.py -f png world build/depends/world.dot
	@echo "Zone..."
	time python3 build/depends/dependency_graph.py -f png zone build/depends/zone.dot
	@echo "Common..."
	time python3 build/depends/dependency_graph.py -f png common build/depends/common.dot

backup:
	@mkdir -p build/bin/backup
	cd build/bin && ./world database:dump --compress --player-tables --state-tables --system-tables --query-serv-tables

cpu-zone:
	@cd build/bin && mkdir -p tmp
	cd build/bin && CPUPROFILE=prof.out ./zone

pprof-zone:
	cd build/bin && google-pprof --pdf zone prof.out > prof.pdf
pprof-web-zone:
	cd build/bin && google-pprof --web zone prof.out
pprof-gv-zone:
	cd build/bin && google-pprof --gv zone prof.out > prof.gv
heap-zone:
	@cd build/bin && mkdir -p tmp
	cd build/bin && HEAPPROFILE=prof.out ./zone
