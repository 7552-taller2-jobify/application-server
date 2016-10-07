cmake_minimum_required (VERSION 2.8.11)
project (SERVER)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(
	${LEVELDB_PREFIX}/include
	libs/curl-master/include
)

link_directories(
	${LEVELDB_PREFIX}/lib
	libs/curl-master/include
)

include_directories(
	libs
)

add_subdirectory(libs)
add_subdirectory(src)
add_subdirectory(test)

set(SOURCE_FILES ${CMAKE_BINARY_DIR}/src)

set(TEST_FILES ${CMAKE_BINARY_DIR}/test)

file(GLOB ALLFILES ${SOURCE_FILES}/*.h ${SOURCE_FILES}/*.cpp ${TEST_FILES}/*.cpp)

set(CPPLINT_COMMAND ${CMAKE_BINARY_DIR}/cpplint.py --linelength=120 --extensions=h,cpp --root=application-server/src ${ALLFILES})

execute_process(COMMAND ${CPPLINT_COMMAND})
