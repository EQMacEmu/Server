FROM debian:12-slim

ENV DEBIAN_FRONTEND noninteractive

# packages for build
RUN apt update && apt install -y \
    bash \
    build-essential \
    cmake \
    cpp \
    debconf-utils \
    g++ \
    gcc \
    libboost-dev \
    libcurl4-openssl-dev \
    liblua5.1-dev \
    libmariadb-dev \
    libsodium-dev \
    libssl-dev \
    lua5.1 \
    lua-bitop \
    make \
    mariadb-client \
    uuid-dev


# packages for runtime
RUN apt install -y \
    bash \
    gdb \
    gettext-base \
    git \
    liblua5.1-0 \
    lua5.1 \
    lua-bitop \
    mariadb-client \
    vim

# .devcontainer mounts our local workspace inside container at /workspaces
# /workspaces is not available at docker build time

# create a folder on the root of container for runtime files
RUN mkdir -p /takp/logs
RUN mkdir -p /takp/shared
# ln does not care if /workspaces exists at this time
#   Note submodules name
RUN ln -s /workspaces/TAKP-Maps /takp/maps
RUN ln -s /workspaces/TAKP-quests /takp/quests


# Todo Copying build files from workspace folder... maybe task runs the copy to /takp?

# Todo conf files