NAME := eqemu-server

DIR_SRC := /src
DIR_DEVCONT := ${DIR_SRC}/.devcontainer

DOCKER_ARGS := --rm  --name ${NAME} -v $$PWD:${DIR_SRC} -w ${DIR_SRC} ${NAME}
DOCKER_ARM64_ARGS := --rm --platform linux/arm64 --name ${NAME}-arm64 -v $$PWD:${DIR_SRC} -w ${DIR_SRC} ${NAME}-arm64

.PHONY: build
build:
	cd ${DIR_SRC}/build$$BUILD_SUFFIX && cmake --build . --config Release --target all --

.PHONY: cmake
cmake:
	mkdir -p ${DIR_SRC}/build$$BUILD_SUFFIX
	@cd ${DIR_SRC}/build$$BUILD_SUFFIX && cmake -DEQEMU_BUILD_LOGIN=ON \
			-DEQEMU_BUILD_TESTS=ON \
			-DCMAKE_CXX_COMPILER_LAUNCHER=ccache -G Ninja ..

clean:
	rm -rf ${DIR_SRC}/build
	
clean-runtime:
	rm -rf ${DIR_DEVCONT}/bin
	
clean-assets:
	rm -rf ${DIR_DEVCONT}/base/Maps
	rm -rf ${DIR_DEVCONT}/base/quests
	rm -rf ${DIR_DEVCONT}/base/db/alkabor*
	
clean-all: clean clean-runtime clean-assets

docker-cmake:
	cd ${DIR_SRC} && git submodule update --init --recursive
	cd ${DIR_SRC} && docker run ${DOCKER_ARGS} make cmake

docker-build:
	cd ${DIR_SRC} && docker run ${DOCKER_ARGS} make build

# Build image if it doesn't exist
docker-image-build:
ifeq ($(shell docker images -q ${NAME} 2> /dev/null),)
	@echo "Docker image not found. Building..."
	cd ${DIR_DEVCONT} && docker build -f Dockerfile.debian.dev -t ${NAME} .
endif

docker-arm-cmake: docker-arm-image-build
	cd ${DIR_SRC} && git submodule update --init --recursive
	cd ${DIR_SRC} && docker run ${DOCKER_ARM64_ARGS} make cmake BUILD_SUFFIX=arm64

docker-arm-build: docker-arm-image-build
	cd ${DIR_SRC} && docker run ${DOCKER_ARM64_ARGS} make build BUILD_SUFFIX=arm64

docker-arm-image-build:
ifeq ($(shell docker images -q ${NAME}-arm64 2> /dev/null),)
	@echo "Docker image not found. Building..."
	cd ${DIR_DEVCONT} && docker build -f Dockerfile.debian.arm.dev -t ${NAME}-arm64 .
endif

docker-clean: clean

# Initializes a fresh database based on EQMacEmu latest
.PHONY: inject-mariadb
inject-mariadb:
	-sudo service mariadb start
	-mkdir -p ${DIR_DEVCONT}/base/db
	-sudo mariadb -e 'DROP DATABASE IF EXISTS peq;'
	-sudo mariadb -e 'CREATE DATABASE peq;'
	-sudo mariadb -e "CREATE USER IF NOT EXISTS 'peq'@'127.0.0.1' IDENTIFIED BY 'peqpass';"
	-sudo mariadb -e "GRANT ALL PRIVILEGES ON *.* TO 'peq'@'127.0.0.1';"
ifeq (,$(wildcard ${DIR_DEVCONT}/base/db/alkabor_latest.zip))
	@echo "${DIR_DEVCONT}/base/db/alkabor_latest.zip not found. Downloading latest from EQMacEmu"
	wget -nc https://github.com/EQMacEmu/Server/raw/main/utils/sql/database_full/alkabor_latest.zip -O ${DIR_DEVCONT}/base/db/alkabor_latest.zip
	@-cd ${DIR_DEVCONT}/base/db && unzip alkabor_latest.zip
endif
	@echo "Sourcing db may take a while, please wait..."
	@echo "Sourcing alkabor..."
	@cd ${DIR_DEVCONT}/base/db/alkabor* && for f in alkabor_*.sql; do sudo mariadb --database=peq -e "source $$f"; done
	@echo "Sourcing data_tables..."
	@cd ${DIR_DEVCONT}/base/db/alkabor* && for f in data_tables_*.sql; do sudo mariadb --database=peq -e "source $$f"; done
	@echo "Sourcing login_tables..."
	@cd ${DIR_DEVCONT}/base/db/alkabor* && for f in login_tables_*.sql; do sudo mariadb --database=peq -e "source $$f"; done
	@echo "Sourcing player_tables..."
	@cd ${DIR_DEVCONT}/base/db/alkabor* && for f in player_tables_*.sql; do sudo mariadb --database=peq -e "source $$f"; done
	@echo "Sourcing loginserver..."
	@cd ${DIR_SRC}/loginserver/login_util && sudo mariadb --database peq -e "source tblloginserversettings.sql"
	@echo "Sourcing local development scripts"
	@cd ${DIR_DEVCONT}/base/db/local && for f in *.sql; do sudo mariadb --database=peq -e "source $$f"; done

	@echo "MariaDB is now injected."

# clones or pulls Maps (EQEmuConfig::MapDir is hard coded with capital M)
.PHONY: maps
maps:
	@cd ${DIR_DEVCONT}/base && ([ -d "Maps" ] && (cd Maps && git pull) || git clone https://github.com/EQMacEmu/Maps Maps)

# clones or pulls quests
.PHONY: quests
quests:
	@cd ${DIR_DEVCONT}/base && ([ -d "quests" ] && (cd quests && git pull) || git clone https://github.com/EQMacEmu/quests.git quests)

# prep a separate runtime location so "clean" can be done on build/bin
.PHONY: prep
prep:
	@echo "Preparing ${DIR_DEVCONT}/bin/ for runtime files..."
	@rm -rf ${DIR_DEVCONT}/bin
	@mkdir -p ${DIR_DEVCONT}/bin/logs
	@mkdir ${DIR_DEVCONT}/bin/shared

	@echo "Symlinking Maps and quests..."
	@-ln -s ${DIR_DEVCONT}/base/Maps ${DIR_DEVCONT}/bin/Maps
	@-ln -s ${DIR_DEVCONT}/base/quests ${DIR_DEVCONT}/bin/quests

	@echo "Copying static opcodes..."
	@cp -Rup ${DIR_SRC}/loginserver/login_util/*.conf ${DIR_DEVCONT}/bin
	@cp -Rup ${DIR_SRC}/utils/patches/*.conf ${DIR_DEVCONT}/bin

	@echo "Copying env files..."
	@cp -Rup ${DIR_DEVCONT}/base/eqemu_config.json ${DIR_DEVCONT}/bin
	@cp -Rup ${DIR_DEVCONT}/base/login.json ${DIR_DEVCONT}/bin

	@echo "Symlinking binaries..."
	@find ${DIR_SRC}/build/bin -type f ! -name "*.*" -exec cp --symbolic-link {} ${DIR_DEVCONT}/bin \;

	@echo "Done. Re-run this anytime ${DIR_DEVCONT}/base/eqemu_config.json changes."

# launches stack
.PHONY: start
start:
	cd ${DIR_DEVCONT}/bin && ./shared_memory &> ${DIR_DEVCONT}/bin/logs/shared_memory.log
# shared_memory will complete prior to flushing disk
	sleep 3
	cd ${DIR_DEVCONT}/bin && ./loginserver &> ${DIR_DEVCONT}/bin/logs/login.log &
	cd ${DIR_DEVCONT}/bin && ./world &> ${DIR_DEVCONT}/bin/logs/world.log &
	cd ${DIR_DEVCONT}/bin && ./eqlaunch 'dynzone1' &> ${DIR_DEVCONT}/bin/logs/eqlaunch.log &
	cd ${DIR_DEVCONT}/bin && ./eqlaunch 'boats' &> ${DIR_DEVCONT}/bin/logs/eqlaunch-boats.log &
	cd ${DIR_DEVCONT}/bin && ./queryserv &> ${DIR_DEVCONT}/bin/logs/queryserv.log &
	cd ${DIR_DEVCONT}/bin && ./ucs &> ${DIR_DEVCONT}/bin/logs/ucs.log &

# kills stack (ignores errors about process not running)
.PHONY: stop
stop:
	@-pkill queryserv 2> /dev/null || true
	@-pkill ucs 2> /dev/null || true
	@-pkill eqlaunch 2> /dev/null || true
	@-pkill zone 2> /dev/null || true
	@-pkill world 2> /dev/null || true
	@-pkill shared_memory 2> /dev/null || true
	@-pkill loginserver 2> /dev/null || true

# Runs tests
.PHONY: test
test:
	cd ${DIR_SRC}/build/bin && ./tests

# Runs login binary
.PHONY: login
login:
	cd ${DIR_SRC}/build/bin && ./loginserver

# Runs shared_memory binary
.PHONY: shared
shared:
	@cd ${DIR_SRC}/build/bin && mkdir -p shared
	cd ${DIR_SRC}/build/bin && ./shared_memory

# Runs zone binary
.PHONY: zone
zone:
	@-rm ${DIR_SRC}/build/bin/logs/zone/zone*.log
	cd ${DIR_SRC}/build/bin && ./zone

# Runs world binary
.PHONY: world
world:
	@-rm ${DIR_SRC}/build/bin/logs/world*.log
	cd ${DIR_SRC}/build/bin && ./world

# Runs ucs binary
.PHONY: ucs
ucs:
	@-rm ${DIR_SRC}/build/bin/logs/ucs*.log
	cd ${DIR_SRC}/build/bin && ./ucs

# Runs queryserv binary
.PHONY: queryserv
queryserv:
	@-rm ${DIR_SRC}/build/bin/logs/query_server*.log
	cd ${DIR_SRC}/build/bin && ./queryserv

valgrind-%:
	cd ${DIR_SRC}/build/bin && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=logs/$*.valgrind.log ./$*

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
	mkdir -p ${DIR_SRC}/build/depends
	@if [ ! -f "${DIR_SRC}/build/depends/dependency_graph.py" ]; then \
		wget https://raw.githubusercontent.com/pvigier/dependency-graph/master/dependency_graph.py -O build/depends/dependency_graph.py; \
	fi
	@echo "Generating dependency graphs (This may take a while)..."
	@echo "Login..."
	time python3 ${DIR_SRC}/build/depends/dependency_graph.py -f png login ${DIR_SRC}/build/depends/login.dot
	@echo "World..."
	time python3 ${DIR_SRC}/build/depends/dependency_graph.py -f png world ${DIR_SRC}/build/depends/world.dot
	@echo "Zone..."
	time python3 ${DIR_SRC}/build/depends/dependency_graph.py -f png zone ${DIR_SRC}/build/depends/zone.dot
	@echo "Common..."
	time python3 ${DIR_SRC}/build/depends/dependency_graph.py -f png common ${DIR_SRC}/build/depends/common.dot

backup:
	@mkdir -p ${DIR_SRC}/build/bin/backup
	cd ${DIR_SRC}/build/bin && ./world database:dump --compress --player-tables --state-tables --system-tables --query-serv-tables

cpu-zone:
	@cd ${DIR_SRC}/build/bin && mkdir -p tmp
	cd ${DIR_SRC}/build/bin && CPUPROFILE=prof.out ./zone

pprof-zone:
	cd ${DIR_SRC}/build/bin && google-pprof --pdf zone prof.out > prof.pdf
pprof-web-zone:
	cd ${DIR_SRC}/build/bin && google-pprof --web zone prof.out
pprof-gv-zone:
	cd ${DIR_SRC}/build/bin && google-pprof --gv zone prof.out > prof.gv
heap-zone:
	@cd ${DIR_SRC}/build/bin && mkdir -p tmp
	cd ${DIR_SRC}/build/bin && HEAPPROFILE=prof.out ./zone
