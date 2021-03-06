cmake_minimum_required (VERSION 2.8.11)
project (Server)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include(CodeCoverage.cmake)
set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
SETUP_TARGET_FOR_COVERAGE(coverage runTests coverage-output)

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

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_BINARY_DIR})
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_NAME "App-Server")
set(CPACK_DEBIAN_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jobify <jobify@jobify.com>")
set(CPACK_PACKAGE_EXECUTABLES "MyServer")
include(CPack)
