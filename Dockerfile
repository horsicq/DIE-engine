ARG image=ubuntu:bionic

FROM ${image} as source-internet

RUN apt-get update --quiet && \
    apt-get install --quiet --assume-yes git
RUN git clone --recursive https://github.com/horsicq/DIE-engine.git

FROM source-internet as builder

RUN apt-get update --quiet
RUN apt-get install --quiet --assume-yes \
      git  \
      build-essential \
      qt5-default \
      qtbase5-dev \
      qttools5-dev-tools

RUN qtchooser -print-env \
 && qmake --version

RUN cd DIE-engine &&  bash -x build_dpkg.sh && bash -x install.sh