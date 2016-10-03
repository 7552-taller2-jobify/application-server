#include "AttendantsHandler.cpp"
#include <gtest/gtest.h>

class TestAttendantsHandler : public ::testing::Test { 
public:
	AttendantsHandler *attendants_handler;
	ifstream file;

	void SetUp() { 
		attendants_handler = new AttendantsHandler();
	}
 
	void TearDown() {
		delete attendants_handler;
	}
};
 
TEST_F(TestAttendantsHandler, testFindExistingKey) {
	Attendant* attendant = attendants_handler->find("ProfileExpertise");
	ASSERT_TRUE(dynamic_cast<ProfileExpertise*> (attendant));
}

TEST_F(TestAttendantsHandler, testFindNonExistingKeyReturnsNull) {
	Attendant* attendant = attendants_handler->find("non_existing_key");
	ASSERT_TRUE(attendant == NULL);
}
