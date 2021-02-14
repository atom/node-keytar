FROM debian:buster

RUN dpkg --add-architecture arm64
RUN apt-get update && apt-get install -y --no-install-recommends \
  crossbuild-essential-arm64 \
  python \
  git \
  pkg-config \
  fakeroot \
  rpm \
  ca-certificates \
  libx11-dev:arm64 \
  libx11-xcb-dev:arm64 \
  libxkbfile-dev:arm64 \
  libsecret-1-dev:arm64 \
  curl

ENV AS=/usr/bin/aarch64-linux-gnu-as \
  STRIP=/usr/bin/aarch64-linux-gnu-strip \
  AR=/usr/bin/aarch64-linux-gnu-ar \
  CC=/usr/bin/aarch64-linux-gnu-gcc \
  CPP=/usr/bin/aarch64-linux-gnu-cpp \
  CXX=/usr/bin/aarch64-linux-gnu-g++ \
  LD=/usr/bin/aarch64-linux-gnu-ld \
  FC=/usr/bin/aarch64-linux-gnu-gfortran \
  PKG_CONFIG_PATH=/usr/lib/aarch64-linux-gnu/pkgconfig

RUN curl -sL https://deb.nodesource.com/setup_15.x | bash -
RUN apt-get install -y nodejs
