// "Copyright 2016 <Jobify>"

#include <gtest/gtest.h>
#include "Attendant.h"

class TestAttendant : public ::testing::Test {
 public:
    Login *login;
    RecoveryPass *recoveryPass;

    void SetUp() {
        login = new Login();
    }

    ~TestAttendant() {
        delete login;
    }
};
// TODO modificar este test
TEST_F(TestAttendant, testFindExistingKey) {
    struct Message message;
    message.verb = "GET";
    login->attend(message);
    ASSERT_TRUE(true);
}

