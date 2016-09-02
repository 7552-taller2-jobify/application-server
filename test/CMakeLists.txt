add_executable (runTests TestSample.cpp)

target_link_libraries(runTests Sample ${GTEST_LIBRARIES} pthread)
