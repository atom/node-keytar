FROM debian:buster

RUN dpkg --add-architecture armhf
RUN apt-get update && apt-get install -y --no-install-recommends \
  crossbuild-essential-armhf \
  python \
  git \
  pkg-config \
  fakeroot \
  rpm \
  ca-certificates \
  libx11-dev:armhf \
  libx11-xcb-dev:armhf \
  libxkbfile-dev:armhf \
  libsecret-1-dev:armhf \
  curl

ENV AS=/usr/bin/arm-linux-gnueabihf-as \
  STRIP=/usr/bin/arm-linux-gnueabihf-strip \
  AR=/usr/bin/arm-linux-gnueabihf-ar \
  CC=/usr/bin/arm-linux-gnueabihf-gcc \
  CPP=/usr/bin/arm-linux-gnueabihf-cpp \
  CXX=/usr/bin/arm-linux-gnueabihf-g++ \
  LD=/usr/bin/arm-linux-gnueabihf-ld \
  FC=/usr/bin/arm-linux-gnueabihf-gfortran \
  PKG_CONFIG_PATH=/usr/lib/arm-linux-gnueabihf/pkgconfig

RUN curl -sL https://deb.nodesource.com/setup_15.x | bash -
RUN apt-get install -y nodejs
