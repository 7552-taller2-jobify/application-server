// "Copyright 2016 <Jobify>"

#include <gtest/gtest.h>
#include <fstream>
#include "AttendantsHandler.h"

class TestAttendantsHandler : public ::testing::Test {
 public:
    AttendantsHandler *attendants_handler;
    std::ifstream file;

    void SetUp() {
        attendants_handler = new AttendantsHandler();
    }

    void TearDown() {
        delete attendants_handler;
    }
};

TEST_F(TestAttendantsHandler, testFindExistingKey) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/perfil/expertise");
    ASSERT_TRUE(dynamic_cast<ProfileExpertise*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindLogin) {
    Attendant* attendant = attendants_handler->find("/users/login");
    ASSERT_TRUE(dynamic_cast<Login*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindRegister) {
    Attendant* attendant = attendants_handler->find("/users/register");
    ASSERT_TRUE(dynamic_cast<Register*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindNonExistingKeyReturnsNull) {
    Attendant* attendant = attendants_handler->find("non_existing_key");
    ASSERT_TRUE(attendant == NULL);
}
