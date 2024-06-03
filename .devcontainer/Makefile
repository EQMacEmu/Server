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

# Initializes a fresh database based on EQMacEmu latest
.PHONY: inject-mariadb
inject-mariadb:
	-sudo service mariadb start
	-mkdir -p /src/.devcontainer/base/db
	-sudo mariadb -e 'DROP DATABASE IF EXISTS peq;'
	-sudo mariadb -e 'CREATE DATABASE peq;'
	-sudo mariadb -e "CREATE USER IF NOT EXISTS 'peq'@'127.0.0.1' IDENTIFIED BY 'peqpass';"
	-sudo mariadb -e "GRANT ALL PRIVILEGES ON *.* TO 'peq'@'127.0.0.1';"
ifeq (,$(wildcard /src/.devcontainer/base/db/alkabor_latest.zip))
	@echo "/src/.devcontainer/base/db/alkabor_latest.zip not found. Downloading latest from EQMacEmu"
	wget -nc https://github.com/EQMacEmu/Server/raw/main/utils/sql/database_full/alkabor_latest.zip -O /src/.devcontainer/base/db/alkabor_latest.zip
	@-cd /src/.devcontainer/base/db && unzip alkabor_latest.zip
endif
	@echo "Sourcing db may take a while, please wait..."
	@echo "Sourcing alkabor..."
	@cd /src/.devcontainer/base/db/alkabor* && for f in alkabor_*.sql; do sudo mariadb --database=peq -e "source $$f"; done
	@echo "Sourcing data_tables..."
	@cd /src/.devcontainer/base/db/alkabor* && for f in data_tables_*.sql; do sudo mariadb --database=peq -e "source $$f"; done
	@echo "Sourcing login_tables..."
	@cd /src/.devcontainer/base/db/alkabor* && for f in login_tables_*.sql; do sudo mariadb --database=peq -e "source $$f"; done
	@echo "Sourcing player_tables..."
	@cd /src/.devcontainer/base/db/alkabor* && for f in player_tables_*.sql; do sudo mariadb --database=peq -e "source $$f"; done
	@echo "Sourcing loginserver..."
	@cd /src/loginserver/login_util && sudo mariadb --database peq -e "source tblloginserversettings.sql"
	@echo "Sourcing eqlaunch boats..."
	@cd /src/.devcontainer/base/db && sudo mariadb --database peq -e "source launcher_boats.sql"

	@echo "MariaDB is now injected."

# clones or pulls Maps (EQEmuConfig::MapDir is hard coded with capital M)
.PHONY: maps
maps:
	@cd /src/.devcontainer/base && ([ -d "Maps" ] && (cd Maps && git pull) || git clone https://github.com/EQMacEmu/Maps Maps)

# clones or pulls quests
.PHONY: quests
quests:
	@cd /src/.devcontainer/base && ([ -d "quests" ] && (cd quests && git pull) || git clone https://github.com/EQMacEmu/quests.git quests)

# prep a separate runtime location so "clean" can be done on build/bin
.PHONY: prep
prep:
	@echo "Preparing /src/.devcontainer/bin/ for runtime files..."
	@rm -rf /src/.devcontainer/bin
	@mkdir -p /src/.devcontainer/bin/logs
	@mkdir /src/.devcontainer/bin/shared

	@echo "Symlinking Maps and quests..."
	@-ln -s /src/.devcontainer/base/Maps /src/.devcontainer/bin/Maps
	@-ln -s /src/.devcontainer/base/quests /src/.devcontainer/bin/quests

	@echo "Copying static opcodes..."
	@cp -Rup /src/loginserver/login_util/*.conf /src/.devcontainer/bin
	@cp -Rup /src/utils/patches/*.conf /src/.devcontainer/bin

# Todo - source this from a properties file
	@echo "Copying env files..."
	@cp -Rup /src/.devcontainer/base/eqemu_config.json /src/.devcontainer/bin
	@cp -Rup /src/.devcontainer/base/login.ini /src/.devcontainer/bin

	@echo "Symlinking binaries..."
	@find /src/build/bin -type f ! -name "*.*" -exec cp --symbolic-link {} /src/.devcontainer/bin \;

# launches stack
.PHONY: start
start:
	/src/.devcontainer/bin/shared_memory &> /src/.devcontainer/bin/logs/shared_memory.log
	/src/.devcontainer/bin/loginserver &> /src/.devcontainer/bin/logs/login.log &
	/src/.devcontainer/bin/world &> /src/.devcontainer/bin/logs/world.log &
	/src/.devcontainer/bin/eqlaunch 'dynzone1' &> /src/.devcontainer/bin/logs/eqlaunch.log &
	/src/.devcontainer/bin/eqlaunch 'boats' &> /src/.devcontainer/bin/logs/eqlaunch-boats.log &
	/src/.devcontainer/bin/queryserv &> /src/.devcontainer/bin/logs/queryserv.log &
	/src/.devcontainer/bin/ucs &> /src/.devcontainer/bin/logs/ucs.log &

# Runs tests
.PHONY: test
test:
	cd /src/build/bin && ./tests

# Runs login binary
.PHONY: login
login:
	cd /src/build/bin && ./loginserver

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
