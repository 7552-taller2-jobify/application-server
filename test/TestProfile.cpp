#include "Profile.cpp"
#include <gtest/gtest.h>

const string json_file = "test/example_profile.json";

class TestProfile : public ::testing::Test { 
public:
	Profile *profile;

	void SetUp() { 
		profile = new Profile(json_file);
	}
 
	~TestProfile() {
		delete profile;
	}
};
 
TEST_F(TestProfile, testProfileWellCreated) {
	ASSERT_EQ(profile->getName(), "Matías");
	ASSERT_EQ(profile->getSummary(), "Ésta es la descripción de Matías");
	ASSERT_EQ(profile->getPicture(), "Link");
	ASSERT_EQ(profile->getSkills(), "Java, Android");
	string lat = "-34.61543532", lon = "-58.37213459";
	ASSERT_EQ(profile->getPosition()[0], atof(lat.c_str()));
	ASSERT_EQ(profile->getPosition()[1], atof(lon.c_str()));
	ASSERT_EQ(profile->getJobExperience(), "3 años en Mercadolibre");
}

TEST_F(TestProfile, testChangeName) {
	profile->setName("Juan");
	ASSERT_EQ(profile->getName(), "Juan");
}

TEST_F(TestProfile, testChangeSummary) {
	profile->setSummary("Nueva descripción");
	ASSERT_EQ(profile->getSummary(), "Nueva descripción");
}

TEST_F(TestProfile, testChangePicture) {
	profile->setPicture("nuevo_link");
	ASSERT_EQ(profile->getPicture(), "nuevo_link");
}

TEST_F(TestProfile, testChangeSkills) {
	profile->setSkills("Java, PHP");
	ASSERT_EQ(profile->getSkills(), "Java, PHP");
}

TEST_F(TestProfile, testChangePosition) {
	string lat = "-12.34567890", lon = "-21.43658709";
	profile->setPosition(lat, lon);
	ASSERT_EQ(profile->getPosition()[0], atof(lat.c_str()));
	ASSERT_EQ(profile->getPosition()[1], atof(lon.c_str()));
}

TEST_F(TestProfile, testChangeJobExperience) {
	profile->setJobExperience("2 años en Mercadolibre");
	ASSERT_EQ(profile->getJobExperience(), "2 años en Mercadolibre");
}
