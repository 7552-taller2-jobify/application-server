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
    Attendant* attendant = attendants_handler->find("/users/.*@.*/profile/expertise");
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

TEST_F(TestAttendantsHandler, testFindLogout) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/logout");
    ASSERT_TRUE(dynamic_cast<Logout*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindRecoveryPass) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/recovery_pass");
    ASSERT_TRUE(dynamic_cast<RecoveryPass*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindContacs) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/contact");
    ASSERT_TRUE(dynamic_cast<Contact*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindAccept) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/accept");
    ASSERT_TRUE(dynamic_cast<Accept*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindReject) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/reject");
    ASSERT_TRUE(dynamic_cast<Reject*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindProfilePersonal) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/profile/personal");
    ASSERT_TRUE(dynamic_cast<ProfilePersonal*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindProfileSummary) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/profile/summary");
    ASSERT_TRUE(dynamic_cast<ProfileSummary*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindProfileSkills) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/profile/skills");
    ASSERT_TRUE(dynamic_cast<ProfileSkills*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindProfilePhoto) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/profile/picture");
    ASSERT_TRUE(dynamic_cast<ProfilePhoto*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindVoto) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/vote");
    ASSERT_TRUE(dynamic_cast<Vote*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindProfileFriends) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/profile/contacts");
    ASSERT_TRUE(dynamic_cast<ProfileFriends*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindProfileExpertise) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/profile/expertise");
    ASSERT_TRUE(dynamic_cast<ProfileExpertise*> (attendant));
}

TEST_F(TestAttendantsHandler, testProfileSkills) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/profile/skills/category.");
    ASSERT_TRUE(dynamic_cast<ProfileSkills*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindProfileOwnRecommendations) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/own_recommendations");
    ASSERT_TRUE(dynamic_cast<ProfileOwnRecommendations*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindProfileOthersRecommendations) {
    Attendant* attendant = attendants_handler->find("/users/.*@.*/others_recommendations");
    ASSERT_TRUE(dynamic_cast<ProfileOthersRecommendations*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindMostPopularUsers) {
    Attendant* attendant = attendants_handler->find("/users/pop");
    ASSERT_TRUE(dynamic_cast<MostPopularUsers*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindSearch) {
    Attendant* attendant = attendants_handler->find("/users");
    ASSERT_TRUE(dynamic_cast<Search*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindNonExistingKeyReturnsNull) {
    Attendant* attendant = attendants_handler->find("non_existing_key");
    ASSERT_TRUE(attendant == NULL);
}
