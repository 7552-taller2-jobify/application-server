#include "URLMapper.cpp"
#include <gtest/gtest.h>

class TestURLMapper : public ::testing::Test { 
public:
	URLMapper *url_mapper;
	ifstream file;

	void SetUp() { 
		url_mapper = new URLMapper();
	}
 
	void TearDown() {
		delete url_mapper;
	}
};
 
TEST_F(TestURLMapper, testFindExistingKey) {
	string id = url_mapper->find("http://<applicationserver>/users/.*@.*/perfil/expertise");
	ASSERT_EQ(id, "ProfileExpertise");
}

TEST_F(TestURLMapper, testFindNonExistingKeyReturnsNull) {
	string id = url_mapper->find("non_existing_key");
	ASSERT_TRUE(id.empty());
}
