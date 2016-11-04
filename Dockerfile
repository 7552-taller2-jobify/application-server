FROM ubuntu

# Setup a working directory
RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

ENV WORKDIR=/usr/src/app

# Running commands to start our image
RUN \
  apt-get update && \
  apt-get install -y build-essential libssl-dev wget curl && \
  apt-get install -y cmake libgtest-dev vim

#Google Tests
RUN \
  cd /usr/src/gtest && cmake . && cmake --build . && mv libg* /usr/local/lib/

#LevelDB
RUN \
  wget https://github.com/google/leveldb/archive/v1.19.tar.gz && tar -xzf v1.19.tar.gz && rm v1.19.tar.gz && mv leveldb-1.19 /tmp/leveldb && cd /tmp/leveldb && make && cp --preserve=links out-static/libleveldb.* /usr/local/lib && cp -r include/leveldb /usr/local/include && ldconfig

#cURL
RUN \
  apt-get install -y autoconf libtool && \
  wget https://curl.haxx.se/download/curl-7.50.3.tar.gz && tar -xzf curl-7.50.3.tar.gz && rm curl-7.50.3.tar.gz && cd curl-7.50.3 && ./buildconf && ./configure && make && make install

#JWT.io
RUN \
  apt-get install -y software-properties-common && \
  add-apt-repository ppa:ben-collins/libjwt && \
  apt-get update
