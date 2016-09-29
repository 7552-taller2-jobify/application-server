#include "TestDataBase.cpp"
#include "TestLogger.cpp"
#include "TestURLMapper.cpp"
#include "TestAttendantsHandler.cpp"
#include "Attendant.cpp"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
