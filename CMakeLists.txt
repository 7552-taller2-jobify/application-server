cmake_minimum_required (VERSION 2.8.11)
project (SERVER)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(libs)
add_subdirectory(src)

