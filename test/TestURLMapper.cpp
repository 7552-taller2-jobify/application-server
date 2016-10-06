// "Copyright 2016 <Jobify>"

#include <gtest/gtest.h>
#include <string>
#include "URLMapper.h"

class TestURLMapper : public ::testing::Test {
 public:
    URLMapper *url_mapper;
    std::ifstream file;

    void SetUp() {
        url_mapper = new URLMapper();
    }

    void TearDown() {
        delete url_mapper;
    }
};

TEST_F(TestURLMapper, testFindExistingKey) {
    std::string id = url_mapper->find("http://users/.*@.*/perfil/expertise");
    ASSERT_EQ(id, "ProfileExpertise");
}

TEST_F(TestURLMapper, testFindExistingLogin) {
    std::string id = url_mapper->find("/users/login");
    ASSERT_EQ(id, "Login");
}

TEST_F(TestURLMapper, testFindNonExistingKeyReturnsNull) {
    std::string id = url_mapper->find("non_existing_key");
    ASSERT_TRUE(id.empty());
}
