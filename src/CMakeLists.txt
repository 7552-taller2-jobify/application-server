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
    FIND_LIBRARY(LIBCURL curl)
ENDIF()

FIND_LIBRARY(LIBJWTDEV jwt)
IF(NOT LIBJWTDEV)
    execute_process(COMMAND sudo apt-get install -y software-properties-common python-software-properties)
    execute_process(COMMAND sudo add-apt-repository -y ppa:ben-collins/libjwt)
    execute_process(COMMAND sudo apt-get update)
    execute_process(COMMAND sudo apt-get install -y libjwt-dev)
    FIND_LIBRARY(LIBJWTDEV jwt)
ENDIF()

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
    FIND_LIBRARY(LEVELDB leveldb)
ENDIF()

file(GLOB Clases
	"*.h"
	"*.cpp"
)

add_library (Sources ${Clases})

target_link_libraries(Sources LINK_PUBLIC Mongoose ${LEVELDB} pthread ${LIBJWTDEV} ${LIBCURL})

target_include_directories(Sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable (Server ${Clases})

target_link_libraries(Server Mongoose ${LEVELDB} pthread ${LIBJWTDEV} ${LIBCURL})

install(TARGETS Server RUNTIME DESTINATION bin)
