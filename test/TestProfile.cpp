#include "Profile.cpp"
#include <gtest/gtest.h>

const string json_personal_file_input = "files/example_profile_personal_input.json";
const string json_summary_file_input = "files/example_profile_summary_input.json";
const string json_expertise_file_input = "files/example_profile_expertise_input.json";
const string json_skills_file_input = "files/example_profile_skills_input.json";
const string json_picture_file_input = "files/example_profile_picture_input.json";
const string json_personal_file_output = "files/example_profile_personal_output.json";
const string json_summary_file_output = "files/example_profile_summary_output.json";
const string json_expertise_file_output = "files/example_profile_expertise_output.json";
const string json_skills_file_output = "files/example_profile_skills_output.json";
const string json_picture_file_output = "files/example_profile_picture_output.json";

class TestProfile : public ::testing::Test { 
public:
	Personal *personal;
	Summary *summary;
	Expertise *expertise;
	Skills *skills;
	Picture *picture;

	void SetUp() {
		personal = new Personal();
		summary = new Summary();
		expertise = new Expertise();
		skills = new Skills();
		picture = new Picture();
		personal->getProfileInfo(json_personal_file_input);
		summary->getProfileInfo(json_summary_file_input);
		expertise->getProfileInfo(json_expertise_file_input);
		skills->getProfileInfo(json_skills_file_input);
		picture->getProfileInfo(json_picture_file_input);
	}
 
	~TestProfile() {
		delete personal;
		delete summary;
		delete expertise;
		delete skills;
		delete picture;
	}
};
 
TEST_F(TestProfile, testPersonalWellCreated) {
	ASSERT_EQ(personal->getFirstName(), "Sergio Matías");
	ASSERT_EQ(personal->getLastName(), "Piano");
	ASSERT_EQ(personal->getEmail(), "smpiano@gmail.com");
	ASSERT_EQ(personal->getBirthday(), "24/07/1984");
	string lat = "-34.61543532", lon = "-58.37213459";
	ASSERT_EQ(personal->getAddress()[0], lat);
	ASSERT_EQ(personal->getAddress()[1], lon);
}

TEST_F(TestProfile, testSummaryWellCreated) {
	ASSERT_EQ(summary->getSummary(), "Ingeniero en informática senior");
}

TEST_F(TestProfile, testExpertiseWellCreated) {
	ASSERT_EQ(expertise->getCompany(), "Lalala");
	ASSERT_EQ(expertise->getPosition(), "Líder técnico");
	ASSERT_EQ(expertise->getFrom(), "01/01/2010");
	ASSERT_EQ(expertise->getTo(), "26/09/2016");
	ASSERT_EQ(expertise->getExpertise(), "aaer");
}

TEST_F(TestProfile, testSkillsWellCreated) {
	ASSERT_EQ(skills->getSkills(), "skill1, skill2, skill3, skill4");
}

TEST_F(TestProfile, testPictureWellCreated) {
	ASSERT_EQ(picture->getPicture(), "link");
}

TEST_F(TestProfile, testChangeFirstName) {
	personal->setFirstName("Juan");
	ASSERT_EQ(personal->getFirstName(), "Juan");
}

TEST_F(TestProfile, testChangeLastName) {
	personal->setLastName("Pérez");
	ASSERT_EQ(personal->getLastName(), "Pérez");
}

TEST_F(TestProfile, testChangeEmail) {
	personal->setEmail("juanperez@gmail.com");
	ASSERT_EQ(personal->getEmail(), "juanperez@gmail.com");
}

TEST_F(TestProfile, testChangeBirthday) {
	personal->setBirthday("10/10/1990");
	ASSERT_EQ(personal->getBirthday(), "10/10/1990");
}

TEST_F(TestProfile, testChangeAddress) {
	string lat = "-12.34567890", lon = "-21.43658709";
	personal->setAddress(lat, lon);
	ASSERT_EQ(personal->getAddress()[0], lat);
	ASSERT_EQ(personal->getAddress()[1], lon);
}

TEST_F(TestProfile, testChangeSummary) {
	summary->setSummary("Nueva descripción");
	ASSERT_EQ(summary->getSummary(), "Nueva descripción");
}

TEST_F(TestProfile, testChangeCompany) {
	expertise->setCompany("Nueva compañía");
	ASSERT_EQ(expertise->getCompany(), "Nueva compañía");
}

TEST_F(TestProfile, testChangePosition) {
	expertise->setPosition("Nueva posición");
	ASSERT_EQ(expertise->getPosition(), "Nueva posición");
}

TEST_F(TestProfile, testChangeFrom) {
	expertise->setFrom("20/10/2000");
	ASSERT_EQ(expertise->getFrom(), "20/10/2000");
}

TEST_F(TestProfile, testChangeTo) {
	expertise->setTo("02/01/2015");
	ASSERT_EQ(expertise->getTo(), "02/01/2015");
}

TEST_F(TestProfile, testChangeExpertise) {
	expertise->setExpertise("Nueva experiencia");
	ASSERT_EQ(expertise->getExpertise(), "Nueva experiencia");
}

TEST_F(TestProfile, testChangeSkills) {
	skills->setSkills("Java, PHP");
	ASSERT_EQ(skills->getSkills(), "Java, PHP");
}

TEST_F(TestProfile, testChangePicture) {
	picture->setPicture("nuevo_link");
	ASSERT_EQ(picture->getPicture(), "nuevo_link");
}

/*TEST_F(TestProfile, testUpdateJson) {
	profile->setName("Juan");
	profile->setSummary("Nueva descripción");
	profile->setPicture("nuevo_link");
	profile->setSkills("Java, PHP");
	string lat = "-12.34567890", lon = "-21.43658709";
	profile->setPosition(lat, lon);
	profile->setJobExperience("2 años en Mercadolibre");
	profile->updateJson(json_file_output);
	Profile *profile_modified = new Profile(json_file_output);

	ASSERT_EQ(profile_modified->getName(), "Juan");
	ASSERT_EQ(profile_modified->getSummary(), "Nueva descripción");
	ASSERT_EQ(profile_modified->getPicture(), "nuevo_link");
	ASSERT_EQ(profile_modified->getSkills(), "Java, PHP");
	ASSERT_EQ(profile_modified->getPosition()[0], lat);
	ASSERT_EQ(profile_modified->getPosition()[1], lon);
	ASSERT_EQ(profile_modified->getJobExperience(), "2 años en Mercadolibre");
}*/
