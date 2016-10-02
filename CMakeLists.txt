cmake_minimum_required (VERSION 2.8.11)
project (SERVER)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(
	${LEVELDB_PREFIX}/include
)

link_directories(
	${LEVELDB_PREFIX}/lib
)

include_directories(
	includes
)

add_subdirectory(libs)
add_subdirectory(src)
add_subdirectory(test)
