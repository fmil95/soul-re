FROM python:3.12-slim-bookworm
ARG USERNAME=user
ARG UID=1000
ARG GID=1000

RUN apt-get update && apt-get install -y --no-install-recommends \
        git \
        git-lfs \
        build-essential \
        binutils-mips-linux-gnu \
        cpp-mips-linux-gnu \
        bchunk \
        p7zip-full \
        sudo \
        && apt-get autoremove -y \
        && apt-get clean \
        && rm -rf /var/lib/apt/lists/* \
        && groupadd -g $GID $USERNAME \
        && useradd -m -u $UID -g $GID -s /bin/bash $USERNAME \
        && echo "USERNAME ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/$USERNAME \
        && chmod 0440 /etc/sudoers.d/$USERNAME

RUN mkdir -p /game

COPY game/SLUS_007.08 /game/SLUS_007.08
COPY game/cinemax.bin /game/cinemax.bin
COPY game/mcardx.bin /game/mcardx.bin
COPY game/hunter.bin /game/hunter.bin
COPY game/skinner.bin /game/skinner.bin
COPY game/wallcr.bin /game/wallcr.bin
COPY game/aluka.bin /game/aluka.bin
COPY game/skinbos.bin /game/skinbos.bin
COPY game/kain.bin /game/kain.bin
COPY game/walboss.bin /game/walboss.bin
COPY game/walbosb.bin /game/walbosb.bin
COPY game/alukabss.bin /game/alukabss.bin
COPY game/roninbss.bin /game/roninbss.bin

COPY requirements.txt /tmp/requirements.txt

RUN pip install --no-cache-dir -r /tmp/requirements.txt \
    && rm -rf /tmp

WORKDIR /app
