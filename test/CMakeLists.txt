add_executable (runTests AllTests.cpp TestDataBase.cpp TestLogger.cpp TestURLMapper.cpp TestAttendantsHandler.cpp TestProfile.cpp TestProfileHandler.cpp)

target_link_libraries(runTests Sources ${GTEST_LIBRARIES} leveldb pthread)
