#include "Profile.cpp"
#include <gtest/gtest.h>

const string json_file_input = "test/example_profile_input.json";
const string json_file_output = "test/example_profile_output.json";

class TestProfile : public ::testing::Test { 
public:
	Profile *profile;

	void SetUp() { 
		profile = new Profile(json_file_input);
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
	ASSERT_EQ(profile->getPosition()[0], lat);
	ASSERT_EQ(profile->getPosition()[1], lon);
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
	ASSERT_EQ(profile->getPosition()[0], lat);
	ASSERT_EQ(profile->getPosition()[1], lon);
}

TEST_F(TestProfile, testChangeJobExperience) {
	profile->setJobExperience("2 años en Mercadolibre");
	ASSERT_EQ(profile->getJobExperience(), "2 años en Mercadolibre");
}

TEST_F(TestProfile, testUpdateJson) {
	profile->setName("Juan");
	profile->setSummary("Nueva descripción");
	profile->setPicture("nuevo_link");
	profile->setSkills("Java, PHP");
	string lat = "-12.34567890", lon = "-21.43658709";
	profile->setPosition(lat, lon);
	profile->setJobExperience("2 años en Mercadolibre");
	profile->updateJson(json_file_output);
	Profile *profile_modified = new Profile(json_file_output);

	ASSERT_EQ(profile->getName(), "Juan");
	ASSERT_EQ(profile->getSummary(), "Nueva descripción");
	ASSERT_EQ(profile->getPicture(), "nuevo_link");
	ASSERT_EQ(profile->getSkills(), "Java, PHP");
	ASSERT_EQ(profile->getPosition()[0], lat);
	ASSERT_EQ(profile->getPosition()[1], lon);
	ASSERT_EQ(profile->getJobExperience(), "2 años en Mercadolibre");
}
