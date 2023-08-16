#!/bin/bash

# build
cd /workspaces
mkdir -p build
cd build
cmake -G 'Unix Makefiles' .. 
make -j `grep -P '^core id\t' /proc/cpuinfo | sort -u | wc -l`


# runtime
# create takp folder on the root of container for runtime files
mkdir -p /takp/logs
mkdir -p /takp/shared

# symlink maps and quests (note submodules name)
ln -s /workspaces/TAKP-Maps /takp/maps
ln -s /workspaces/TAKP-quests /takp/quests

# copy static opcodes
cp /workspaces/loginserver/login_util/*.conf /takp
cp /workspaces/utils/patches/*.conf /takp

# copy env files
envsubst < /workspaces/.devcontainer/app-context/template-eqemu_config.json > /takp/eqemu_config.json
envsubst < /workspaces/.devcontainer/app-context/template-login.ini > /takp/login.ini

# symlink binaries, build/bin/* as individual files in /takp
cp --symbolic-link /workspaces/build/bin/* /takp


# wait for db?
sleep 1
cd /takp
./shared_memory &> logs/shared_memory.log
./loginserver &> logs/login.log &
./world &> logs/world.log &
./eqlaunch 'dynzone1' &> logs/eqlaunch.log &
./eqlaunch 'boats' &> logs/eqlaunch-boats.log &
./queryserv &> logs/queryserv.log &
./ucs &> logs/ucs.log &


exec "$@"