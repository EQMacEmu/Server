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
    uuid-dev \
    dos2unix


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
    vim \
    net-tools \
    dos2unix

# devcontainter.json & docker-compose.yml mounts our local workspace inside container at /workspaces
# /workspaces is not available at docker build time & is out of scope of docker context

COPY app-context/takp-init.sh /
RUN chmod +x /takp-init.sh
RUN dos2unix /takp-init.sh
ENTRYPOINT [ "/takp-init.sh" ]
# todo server startup
CMD [ "sleep", "infinity" ]
