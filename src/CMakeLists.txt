file(GLOB Clases
	"*.h"
	"*.cpp"
)

add_executable (Server ${Clases})

target_link_libraries (Server LINK_PUBLIC Mongoose leveldb pthread jwt curl)

add_library (Sources ${Clases})

target_link_libraries (Sources LINK_PUBLIC Mongoose leveldb pthread jwt curl)

target_include_directories(Sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

FIND_LIBRARY(LEVELDB leveldb)
IF(NOT LEVELDB)
    execute_process(COMMAND sudo wget https://github.com/google/leveldb/archive/v1.19.tar.gz)
    execute_process(COMMAND sudo tar -xzf v1.19.tar.gz)
    execute_process(COMMAND sudo rm v1.19.tar.gz)
    execute_process(COMMAND sudo mv leveldb-1.19 /tmp/leveldb)
    execute_process(COMMAND sudo cd /tmp/leveldb)
    execute_process(COMMAND sudo make)
    execute_process(COMMAND sudo cp --preserve=links out-static/libleveldb.* /usr/local/lib)
    execute_process(COMMAND sudo cp -r include/leveldb /usr/local/include)
    execute_process(COMMAND sudo ldconfig)
ENDIF()

FIND_LIBRARY(LIBJWTDEV jwt)
IF(NOT LIBJWTDEV)
    execute_process(COMMAND sudo apt-get install -y software-properties-common python-software-properties)
    execute_process(COMMAND sudo add-apt-repository -y ppa:ben-collins/libjwt)
    execute_process(COMMAND sudo apt-get update)
    execute_process(COMMAND sudo apt-get install -y libjwt-dev)
ENDIF()

FIND_LIBRARY(LIBCURL curl)
IF(NOT LIBCURL)
    execute_process(COMMAND sudo apt-get install -y autoconf libtool)
    execute_process(COMMAND sudo wget https://curl.haxx.se/download/curl-7.50.3.tar.gz)
    execute_process(COMMAND sudo tar -xzf curl-7.50.3.tar.gz)
    execute_process(COMMAND sudo rm curl-7.50.3.tar.gz)
    execute_process(COMMAND sudo cd curl-7.50.3)
    execute_process(COMMAND sudo ./buildconf)
    execute_process(COMMAND sudo ./configure)
    execute_process(COMMAND sudo make)
    execute_process(COMMAND sudo make install)
ENDIF()

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jobify")

INCLUDE(CPack)
