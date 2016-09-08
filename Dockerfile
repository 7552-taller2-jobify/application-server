FROM ubuntu

# Setup a working directory
RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

# Running commands to start our image
RUN \
  apt-get update && \
  apt-get install -y build-essential libssl-dev curl && \
  apt-get install -y cmake libgtest-dev vim && \
  cd /usr/src/gtest && cmake . && cmake --build . && mv libg* /usr/local/lib/ && cd -
