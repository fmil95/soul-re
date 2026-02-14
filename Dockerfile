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

COPY SLUS_007.08 /game/SLUS_007.08
COPY hunter.bin /game/hunter.bin

RUN mkdir -p /tmp/tools/cd-dat-utils
COPY requirements.txt /tmp/requirements.txt
COPY tools/cd-dat-utils/requirements.txt /tmp/tools/cd-dat-utils/requirements.txt

RUN pip install --no-cache-dir -r /tmp/requirements.txt \
    && rm -rf /tmp

WORKDIR /app
