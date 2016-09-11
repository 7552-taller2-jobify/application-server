add_executable (runTests AllTests.cpp)

target_link_libraries(runTests DataBase ${GTEST_LIBRARIES} leveldb pthread)
