// "Copyright 2016 <Jobify>"

#include <gtest/gtest.h>
#include "ProfileHandler.h"

class TestProfileHandler : public ::testing::Test {
 public:
    ProfileHandler *profile_handler;

    void SetUp() {
        profile_handler = new ProfileHandler();
    }

    ~TestProfileHandler() {
        delete profile_handler;
    }
};

TEST_F(TestProfileHandler, testFindExistingKey) {
    Profile *profile = profile_handler->find("PERSONAL_juanperez@gmail.com");
    ASSERT_TRUE(dynamic_cast<Personal*> (profile));
}

TEST_F(TestProfileHandler, testFindExistingKeyLogin) {
    Profile *profile = profile_handler->find("juanperez@gmail.com");
    ASSERT_TRUE(dynamic_cast<LoginInformation*> (profile));
}

TEST_F(TestProfileHandler, testFindNonExistingKeyReturnsNull) {
    Profile *profile = profile_handler->find("non_existing_key");
    ASSERT_TRUE(profile == NULL);
}
