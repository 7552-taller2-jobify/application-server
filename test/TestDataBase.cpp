// "Copyright 2016 <Jobify>"

#include <gtest/gtest.h>
#include "DataBase.h"

class TestDataBase : public ::testing::Test {
 public:
    void SetUp() {
        DataBase::getInstance().put("10", "Diez");
        DataBase::getInstance().put("5", "Cinco");
        DataBase::getInstance().put("7", "Siete");
    }

    void TearDown() {
        DataBase::getInstance().erase("5");
        DataBase::getInstance().erase("10");
        DataBase::getInstance().erase("7");
        DataBase::getInstance().erase("8");
    }
};

TEST_F(TestDataBase, testDBWellCreated) {
    ASSERT_EQ(DataBase::getInstance().get("10"), "Diez");
    ASSERT_EQ(DataBase::getInstance().get("5"), "Cinco");
    ASSERT_EQ(DataBase::getInstance().get("7"), "Siete");
}

TEST_F(TestDataBase, testDeleteKey) {
    DataBase::getInstance().erase("5");
    ASSERT_EQ(DataBase::getInstance().get("5"), "");
    ASSERT_EQ(DataBase::getInstance().get("10"), "Diez");
    ASSERT_EQ(DataBase::getInstance().get("7"), "Siete");
}

TEST_F(TestDataBase, testAddKey) {
    DataBase::getInstance().put("8", "Ocho");
    ASSERT_EQ(DataBase::getInstance().get("8"), "Ocho");
    ASSERT_EQ(DataBase::getInstance().get("10"), "Diez");
    ASSERT_EQ(DataBase::getInstance().get("5"), "Cinco");
    ASSERT_EQ(DataBase::getInstance().get("7"), "Siete");
}
