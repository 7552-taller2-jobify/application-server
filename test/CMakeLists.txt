file(GLOB TEST_FILES *.cpp)

add_executable (runTests ${TEST_FILES})

target_link_libraries(runTests Sources ${GTEST_LIBRARIES} leveldb pthread)
