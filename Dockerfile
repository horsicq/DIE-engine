ARG image=ubuntu:bionic

FROM ${image} as source-internet

FROM source-internet as builder

RUN apt-get update --quiet
RUN apt-get install --quiet --assume-yes \
      git  \
      build-essential \
      qt5-default \
      qtbase5-dev \
      qtscript5-dev \
      qttools5-dev-tools

RUN git clone --recursive https://github.com/horsicq/DIE-engine.git

RUN cd DIE-engine &&  bash -x build_dpkg.sh && bash -x install.sh