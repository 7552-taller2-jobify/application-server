// "Copyright 2016 <Jobify>"

#include <gtest/gtest.h>
#include <string>
#include "Profile.h"

const char personal_input[] = "files/example_profile_personal_input.json";
const char summary_input[] = "files/example_profile_summary_input.json";
const char expertise_input[] = "files/example_profile_expertise_input.json";
const char skills_input[] = "files/example_profile_skills_input.json";
const char picture_input[] = "files/example_profile_picture_input.json";
const char personal_output[] = "files/example_profile_personal_output.json";
const char summary_output[] = "files/example_profile_summary_output.json";
const char expertise_output[] = "files/example_profile_expertise_output.json";
const char skills_output[] = "files/example_profile_skills_output.json";
const char picture_output[] = "files/example_profile_picture_output.json";

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
        personal->getProfileInfo(personal_input);
        summary->getProfileInfo(summary_input);
        expertise->getProfileInfo(expertise_input);
        skills->getProfileInfo(skills_input);
        picture->getProfileInfo(picture_input);
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
    std::string lat = "-34.61543532", lon = "-58.37213459";
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
    std::string lat = "-12.34567890", lon = "-21.43658709";
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

TEST_F(TestProfile, testUpdateJsonPersonal) {
    personal->setFirstName("Juan");
    personal->setLastName("Pérez");
    personal->setEmail("juanperez@gmail.com");
    personal->setBirthday("10/10/1990");
    std::string lat = "-12.34567890", lon = "-21.43658709";
    personal->setAddress(lat, lon);
    personal->updateJson(personal_output);
    Personal *personal_modified = new Personal();
    personal_modified->getProfileInfo(personal_output);
    ASSERT_EQ(personal_modified->getFirstName(), "Juan");
    ASSERT_EQ(personal_modified->getLastName(), "Pérez");
    ASSERT_EQ(personal_modified->getEmail(), "juanperez@gmail.com");
    ASSERT_EQ(personal_modified->getBirthday(), "10/10/1990");
    ASSERT_EQ(personal_modified->getAddress()[0], lat);
    ASSERT_EQ(personal_modified->getAddress()[1], lon);
}

TEST_F(TestProfile, testUpdateJsonSummary) {
    summary->setSummary("Ingeniero en Informática ssr");
    summary->updateJson(summary_output);
    Summary *summary_modified = new Summary();
    summary_modified->getProfileInfo(summary_output);
    ASSERT_EQ(summary_modified->getSummary(), "Ingeniero en Informática ssr");
}

TEST_F(TestProfile, testUpdateJsonExpertise) {
    expertise->setCompany("Mercadolibre");
    expertise->setPosition("Desarrollador");
    expertise->setFrom("12/12/1999");
    expertise->setTo("03/07/2005");
    expertise->setExpertise("Desarrollo Java");
    expertise->updateJson(expertise_output);
    Expertise *expertise_modified = new Expertise();
    expertise_modified->getProfileInfo(expertise_output);
    ASSERT_EQ(expertise_modified->getCompany(), "Mercadolibre");
    ASSERT_EQ(expertise_modified->getPosition(), "Desarrollador");
    ASSERT_EQ(expertise_modified->getFrom(), "12/12/1999");
    ASSERT_EQ(expertise_modified->getTo(), "03/07/2005");
    ASSERT_EQ(expertise_modified->getExpertise(), "Desarrollo Java");
}

TEST_F(TestProfile, testUpdateJsonSkills) {
    skills->setSkills("Nueva_skill_1, Nueva_skill_2");
    skills->updateJson(skills_output);
    Skills *skills_modified = new Skills();
    skills_modified->getProfileInfo(skills_output);
    ASSERT_EQ(skills_modified->getSkills(), "Nueva_skill_1, Nueva_skill_2");
}

TEST_F(TestProfile, testUpdateJsonPicture) {
    picture->setPicture("link_nueva_foto_de_perfil");
    picture->updateJson(picture_output);
    Picture *picture_modified = new Picture();
    picture_modified->getProfileInfo(picture_output);
    ASSERT_EQ(picture_modified->getPicture(), "link_nueva_foto_de_perfil");
}
