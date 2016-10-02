#include "TestDataBase.cpp"
#include "TestLogger.cpp"
#include "TestProfile.cpp"
#include "TestProfileHandler.cpp"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
