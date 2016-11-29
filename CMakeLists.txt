cmake_minimum_required (VERSION 2.8.11)
project (Server)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(
	${LEVELDB_PREFIX}/include
	/usr/local/include
)

link_directories(
	${LEVELDB_PREFIX}/lib
	/usr/local/include
)

include_directories(
	libs
	/usr/local/include
)

add_subdirectory(libs)
add_subdirectory(src)
add_subdirectory(test)

set(SOURCE_FILES ${CMAKE_BINARY_DIR}/src)

set(TEST_FILES ${CMAKE_BINARY_DIR}/test)

file(GLOB ALLFILES ${SOURCE_FILES}/*.h ${SOURCE_FILES}/*.cpp ${TEST_FILES}/*.cpp)

set(CPPLINT_COMMAND ${CMAKE_BINARY_DIR}/cpplint.py --linelength=120 --extensions=h,cpp --root=src ${ALLFILES})

execute_process(COMMAND ${CPPLINT_COMMAND})

#set(CPACK_DEBIAN_PACKAGE_DEPENDS ${LIBCURL} ${LEVELDB} ${LIBJWTDEV}) #"curl,jwt,leveldb")




set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_BINARY_DIR})


#FIND_PACKAGE(leveldb REQUIRED leveldb)
#SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_NAME "App-Server")
SET(CPACK_DEBIAN_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "curl,jwt")#,leveldb")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jobify <jobify@jobify.com>")
SET(CPACK_PACKAGE_EXECUTABLES "MyServer")
#set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "Do apt-get -f install && apt-get install -y software-properties-common python-software-properties && add-apt-repository -y ppa:ben-collins/libjwt && apt-get update && apt-get install -y libjwt-dev")
#set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA ./preinst ./postinst)
#set(CPACK_DEBIAN_PACKAGE_PREDEPENDS "libcurl3,libjwt0")
INCLUDE(CPack)



