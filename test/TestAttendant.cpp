// "Copyright 2016 <Jobify>"

#include <gtest/gtest.h>
#include "Attendant.h"

class TestAttendant : public ::testing::Test {
 public:
    Login *login;
    RecoveryPass *recoveryPass;

    void SetUp() {
        login = new Login();
        recoveryPass = new RecoveryPass();
    }

    ~TestAttendant() {
        delete login;
        delete recoveryPass;
    }
};
// TODO modificar este test
TEST_F(TestAttendant, testFindExistingKey) {
    struct Message message;
    message.verb_http = "GET";
    login->attend(message);
    recoveryPass->attend(message);
    ASSERT_TRUE(true);
}
