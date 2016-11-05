// "Copyright 2016 <Jobify>"

#include <gtest/gtest.h>
#include <string>
#include "Profile.h"

const char personal_input[] = "files/example_profile_personal_input.json";
const char summary_input[] = "files/example_profile_summary_input.json";
const char expertise_input[] = "files/example_profile_expertise_input.json";
const char skills_input[] = "files/example_profile_skills_input.json";
const char picture_input[] = "files/example_profile_picture_input.json";
const char friends_input[] = "files/example_profile_friends_input.json";
const char own_recommendations_input[] = "files/example_profile_own_recommendations_input.json";
const char others_recommendations_input[] = "files/example_profile_others_recommendations_input.json";

const char personal_output[] = "files/example_profile_personal_output.json";
const char summary_output[] = "files/example_profile_summary_output.json";
const char expertise_output[] = "files/example_profile_expertise_output.json";
const char skills_output[] = "files/example_profile_skills_output.json";
const char picture_output[] = "files/example_profile_picture_output.json";
const char friends_output[] = "files/example_profile_friends_output.json";
const char own_recommendations_output[] = "files/example_profile_own_recommendations_output.json";
const char others_recommendations_output[] = "files/example_profile_others_recommendations_output.json";

class TestProfile : public ::testing::Test {
 public:
    Personal *personal;
    Summary *summary;
    Expertise *expertise;
    Skills *skills;
    Picture *picture;
    Friends *friends;
    OwnRecommendations *own_recommendations;
    OthersRecommendations *others_recommendations;

    void SetUp() {
        personal = new Personal();
        summary = new Summary();
        expertise = new Expertise();
        skills = new Skills();
        picture = new Picture();
        friends = new Friends();
        own_recommendations = new OwnRecommendations();
        others_recommendations = new OthersRecommendations();

        personal->getProfileInfo(personal_input);
        summary->getProfileInfo(summary_input);
        expertise->getProfileInfo(expertise_input);
        skills->getProfileInfo(skills_input);
        picture->getProfileInfo(picture_input);
        friends->getProfileInfo(friends_input);
        own_recommendations->getProfileInfo(own_recommendations_input);
        others_recommendations->getProfileInfo(others_recommendations_input);
    }

    ~TestProfile() {
        delete personal;
        delete summary;
        delete expertise;
        delete skills;
        delete picture;
        delete friends;
        delete own_recommendations;
        delete others_recommendations;
    }
};

TEST_F(TestProfile, testPersonalWellCreated) {
    ASSERT_EQ(personal->getDeviceId(), 1);
    ASSERT_EQ(personal->getFirstName(), "Sergio Matías");
    ASSERT_EQ(personal->getLastName(), "Piano");
    ASSERT_EQ(personal->getEmail(), "smpiano@gmail.com");
    ASSERT_EQ(personal->getGender(), "M");
    ASSERT_EQ(personal->getBirthday(), "24/07/1984");
    std::string lat = "-34.61543532", lon = "-58.37213459";
    ASSERT_EQ(personal->getAddress()[0], lat);
    ASSERT_EQ(personal->getAddress()[1], lon);
    ASSERT_EQ(personal->getCity(), "La Plata");
}

TEST_F(TestProfile, testSummaryWellCreated) {
    ASSERT_EQ(summary->getSummary(), "Ingeniero en informática senior");
}

TEST_F(TestProfile, testExpertiseWellCreated) {
    ASSERT_EQ(expertise->getCompany(0), "Lalala");
    ASSERT_EQ(expertise->getPosition(0), "Líder técnico");
    ASSERT_EQ(expertise->getFrom(0), "01/01/2010");
    ASSERT_EQ(expertise->getTo(0), "26/09/2016");
    ASSERT_EQ(expertise->getExpertise(0), "aaer");
    ASSERT_EQ(expertise->getCategory(0), "software");
    ASSERT_EQ(expertise->getCompany(1), "Lololo");
    ASSERT_EQ(expertise->getPosition(1), "Especialista técnico");
    ASSERT_EQ(expertise->getFrom(1), "02/02/2012");
    ASSERT_EQ(expertise->getTo(1), "02/02/2016");
    ASSERT_EQ(expertise->getExpertise(1), "aaer");
    ASSERT_EQ(expertise->getCategory(1), "hardware");
}

TEST_F(TestProfile, testSkillsWellCreated) {
    ASSERT_EQ(skills->getSkills(0), "java, c, UML");
    ASSERT_EQ(skills->getCategory(0), "software");
    ASSERT_EQ(skills->getSkills(1), "moto, auto");
    ASSERT_EQ(skills->getCategory(1), "licencia manejo");
}

TEST_F(TestProfile, testPictureWellCreated) {
    ASSERT_EQ(picture->getPicture(), "link");
}

TEST_F(TestProfile, testFriendsWellCreated) {
    ASSERT_EQ(friends->getNumberOfContacts(), 2);
    ASSERT_EQ(friends->getContactAt(0), "facundo.sanchez.galindo@gmail.com");
    ASSERT_EQ(friends->getContactAt(1), "smpiano@gmail.com");
}

TEST_F(TestProfile, testOwnRecommendationsWellCreated) {
    ASSERT_EQ(own_recommendations->getNumberOfContacts(), 2);
    ASSERT_EQ(own_recommendations->getContactAt(0), "facundo.sanchez.galindo@gmail.com");
    ASSERT_EQ(own_recommendations->getContactAt(1), "smpiano@gmail.com");
}

TEST_F(TestProfile, testOthersRecommendationsWellCreated) {
    ASSERT_EQ(others_recommendations->getNumberOfContacts(), 2);
    ASSERT_EQ(others_recommendations->getContactAt(0), "facundo.sanchez.galindo@gmail.com");
    ASSERT_EQ(others_recommendations->getContactAt(1), "smpiano@gmail.com");
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

TEST_F(TestProfile, testChangeGender) {
    personal->setGender("F");
    ASSERT_EQ(personal->getGender(), "F");
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

TEST_F(TestProfile, testChangeCity) {
    personal->setCity("CABA");
    ASSERT_EQ(personal->getCity(), "CABA");
}

TEST_F(TestProfile, testChangeSummary) {
    summary->setSummary("Nueva descripción");
    ASSERT_EQ(summary->getSummary(), "Nueva descripción");
}

TEST_F(TestProfile, testChangeCompany) {
    expertise->setCompany("Nueva compañía", 0);
    ASSERT_EQ(expertise->getCompany(0), "Nueva compañía");
}

TEST_F(TestProfile, testChangePosition) {
    expertise->setPosition("Nueva posición", 0);
    ASSERT_EQ(expertise->getPosition(0), "Nueva posición");
}

TEST_F(TestProfile, testChangeFrom) {
    expertise->setFrom("20/10/2000", 0);
    ASSERT_EQ(expertise->getFrom(0), "20/10/2000");
}

TEST_F(TestProfile, testChangeTo) {
    expertise->setTo("02/01/2015", 0);
    ASSERT_EQ(expertise->getTo(0), "02/01/2015");
}

TEST_F(TestProfile, testChangeExpertise) {
    expertise->setExpertise("Nueva experiencia", 0);
    ASSERT_EQ(expertise->getExpertise(0), "Nueva experiencia");
}

TEST_F(TestProfile, testChangeExpertiseCategory) {
    expertise->setCategory("Nueva categoría", 0);
    ASSERT_EQ(expertise->getCategory(0), "Nueva categoría");
}

TEST_F(TestProfile, testChangeSkills) {
    skills->setSkills("Java, PHP", 0);
    ASSERT_EQ(skills->getSkills(0), "Java, PHP");
}

TEST_F(TestProfile, testChangeSkillsCategory) {
    skills->setCategory("hardware", 0);
    ASSERT_EQ(skills->getCategory(0), "hardware");
}

TEST_F(TestProfile, testChangePicture) {
    picture->setPicture("nuevo_link");
    ASSERT_EQ(picture->getPicture(), "nuevo_link");
}

TEST_F(TestProfile, testAddFriend) {
    friends->addContact("hernan@gmail.com");
    ASSERT_EQ(friends->getNumberOfContacts(), 3);
    ASSERT_EQ(friends->getContactAt(0), "facundo.sanchez.galindo@gmail.com");
    ASSERT_EQ(friends->getContactAt(1), "hernan@gmail.com");
    ASSERT_EQ(friends->getContactAt(2), "smpiano@gmail.com");
}

TEST_F(TestProfile, testRemoveFriend) {
    friends->removeContact("smpiano@gmail.com");
    ASSERT_EQ(friends->getNumberOfContacts(), 1);
    ASSERT_EQ(friends->getContactAt(0), "facundo.sanchez.galindo@gmail.com");
}

TEST_F(TestProfile, testAddOwnRecommendation) {
    own_recommendations->addContact("hernan@gmail.com");
    ASSERT_EQ(own_recommendations->getNumberOfContacts(), 3);
    ASSERT_EQ(own_recommendations->getContactAt(0), "facundo.sanchez.galindo@gmail.com");
    ASSERT_EQ(own_recommendations->getContactAt(1), "hernan@gmail.com");
    ASSERT_EQ(own_recommendations->getContactAt(2), "smpiano@gmail.com");
}

TEST_F(TestProfile, testRemoveOwnRecommendation) {
    own_recommendations->removeContact("smpiano@gmail.com");
    ASSERT_EQ(own_recommendations->getNumberOfContacts(), 1);
    ASSERT_EQ(own_recommendations->getContactAt(0), "facundo.sanchez.galindo@gmail.com");
}

TEST_F(TestProfile, testAddOthersRecommendation) {
    others_recommendations->addContact("hernan@gmail.com");
    ASSERT_EQ(others_recommendations->getNumberOfContacts(), 3);
    ASSERT_EQ(others_recommendations->getContactAt(0), "facundo.sanchez.galindo@gmail.com");
    ASSERT_EQ(others_recommendations->getContactAt(1), "hernan@gmail.com");
    ASSERT_EQ(others_recommendations->getContactAt(2), "smpiano@gmail.com");
}

TEST_F(TestProfile, testRemoveOthersRecommendation) {
    others_recommendations->removeContact("smpiano@gmail.com");
    ASSERT_EQ(others_recommendations->getNumberOfContacts(), 1);
    ASSERT_EQ(others_recommendations->getContactAt(0), "facundo.sanchez.galindo@gmail.com");
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
    expertise->setCompany("Mercadolibre", 0);
    expertise->setPosition("Desarrollador", 0);
    expertise->setFrom("12/12/1999", 0);
    expertise->setTo("03/07/2005", 0);
    expertise->setExpertise("Desarrollo Java", 0);
    expertise->setCategory("Desarrollo", 0);
    expertise->updateJson(expertise_output);
    Expertise *expertise_modified = new Expertise();
    expertise_modified->getProfileInfo(expertise_output);
    ASSERT_EQ(expertise_modified->getCompany(0), "Mercadolibre");
    ASSERT_EQ(expertise_modified->getPosition(0), "Desarrollador");
    ASSERT_EQ(expertise_modified->getFrom(0), "12/12/1999");
    ASSERT_EQ(expertise_modified->getTo(0), "03/07/2005");
    ASSERT_EQ(expertise_modified->getExpertise(0), "Desarrollo Java");
    ASSERT_EQ(expertise_modified->getCategory(0), "Desarrollo");
    ASSERT_EQ(expertise_modified->getCompany(1), "Lololo");
    ASSERT_EQ(expertise_modified->getPosition(1), "Especialista técnico");
    ASSERT_EQ(expertise_modified->getFrom(1), "02/02/2012");
    ASSERT_EQ(expertise_modified->getTo(1), "02/02/2016");
    ASSERT_EQ(expertise_modified->getExpertise(1), "aaer");
    ASSERT_EQ(expertise_modified->getCategory(1), "hardware");
}

TEST_F(TestProfile, testUpdateJsonSkills) {
    skills->setSkills("Nueva_skill_1, Nueva_skill_2", 0);
    skills->updateJson(skills_output);
    Skills *skills_modified = new Skills();
    skills_modified->getProfileInfo(skills_output);
    ASSERT_EQ(skills_modified->getSkills(0), "Nueva_skill_1, Nueva_skill_2");
    ASSERT_EQ(skills_modified->getCategory(0), "software");
    ASSERT_EQ(skills_modified->getSkills(1), "moto, auto");
    ASSERT_EQ(skills_modified->getCategory(1), "licencia manejo");
}

TEST_F(TestProfile, testUpdateJsonPicture) {
    picture->setPicture("link_nueva_foto_de_perfil");
    picture->updateJson(picture_output);
    Picture *picture_modified = new Picture();
    picture_modified->getProfileInfo(picture_output);
    ASSERT_EQ(picture_modified->getPicture(), "link_nueva_foto_de_perfil");
}

TEST_F(TestProfile, testUpdateJsonFriends) {
    friends->addContact("hernan@gmail.com");
    friends->updateJson(friends_output);
    Friends *friends_modified = new Friends();
    friends_modified->getProfileInfo(friends_output);
    ASSERT_EQ(friends_modified->getNumberOfContacts(), 3);
    ASSERT_EQ(friends_modified->getContactAt(0), "facundo.sanchez.galindo@gmail.com");
    ASSERT_EQ(friends_modified->getContactAt(1), "hernan@gmail.com");
    ASSERT_EQ(friends_modified->getContactAt(2), "smpiano@gmail.com");
}

TEST_F(TestProfile, testUpdateJsonOwnRecommendations) {
    own_recommendations->addContact("hernan@gmail.com");
    own_recommendations->updateJson(own_recommendations_output);
    OwnRecommendations *own_recommendations_modified = new OwnRecommendations();
    own_recommendations_modified->getProfileInfo(own_recommendations_output);
    ASSERT_EQ(own_recommendations_modified->getNumberOfContacts(), 3);
    ASSERT_EQ(own_recommendations_modified->getContactAt(0), "facundo.sanchez.galindo@gmail.com");
    ASSERT_EQ(own_recommendations_modified->getContactAt(1), "hernan@gmail.com");
    ASSERT_EQ(own_recommendations_modified->getContactAt(2), "smpiano@gmail.com");
}

TEST_F(TestProfile, testUpdateJsonOthersRecommendations) {
    others_recommendations->addContact("hernan@gmail.com");
    others_recommendations->updateJson(others_recommendations_output);
    OthersRecommendations *others_recommendations_modified = new OthersRecommendations();
    others_recommendations_modified->getProfileInfo(others_recommendations_output);
    ASSERT_EQ(others_recommendations_modified->getNumberOfContacts(), 3);
    ASSERT_EQ(others_recommendations_modified->getContactAt(0), "facundo.sanchez.galindo@gmail.com");
    ASSERT_EQ(others_recommendations_modified->getContactAt(1), "hernan@gmail.com");
    ASSERT_EQ(others_recommendations_modified->getContactAt(2), "smpiano@gmail.com");
}
