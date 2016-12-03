// "Copyright 2016 <Jobify>"

#include <gtest/gtest.h>
#include "Attendant.h"
#include "DataBase.h"

class TestAttendant : public ::testing::Test {
 public:
    Login *login;
    RecoveryPass *recovery_pass;
    Register *reg;
    struct Message message;
    Response *response_register;
    Logout *logout;
    Contact *contact;
    Accept *accept;
    Reject *reject;
    ProfilePersonal *personal;
    ProfileSummary *summary;
    ProfileExpertise *expertise;
    ProfileSkills *skills;
    ProfilePhoto *picture;
    ProfileFriends *friends;
    Vote *vote;
    ProfileOwnRecommendations *own_recommendations;
    ProfileOthersRecommendations *others_recommendations;
    Search *search;

    void SetUp() {
        reg = new Register();
        message.verb = "POST";
        message.body = "{\"email\":\"test@yahoo.com\",\"password\":\"admin\",\"device_id\":\"123\",\"first_name\":\"test\",\"last_name\":\"T\",\"gender\":\"M\",\"birthday\":\"01/01/2000\",\"address\":{\"lat\":\"123456789\",\"lon\":\"12345678\"}}";
        message.params="";
        response_register = reg->post(message);

        login = new Login();
        logout = new Logout();
        contact = new Contact();
        accept = new Accept();
        reject = new Reject();
        personal = new ProfilePersonal();
        summary = new ProfileSummary();
        expertise = new ProfileExpertise();
        skills = new ProfileSkills();
        picture = new ProfilePhoto();
        friends = new ProfileFriends();
        vote = new Vote();
        own_recommendations = new ProfileOwnRecommendations();
        others_recommendations = new ProfileOthersRecommendations();
        search = new Search();
    }

    ~TestAttendant() {
        delete login;
        delete reg;
        delete response_register;
        delete logout;
        delete contact;
        delete accept;
        delete reject;
        delete personal;
        delete summary;
        delete expertise;
        delete skills;
        delete picture;
        delete friends;
        delete vote;
        delete own_recommendations;
        delete others_recommendations;
        delete search;
    }

    Response* loginTest() {
        message.verb = "POST";
        message.uri = "/users/login";
        message.body = "{\"email\":\"test@yahoo.com\",\"password\":\"admin\"}";
        message.params = "";
        return login->post(message);
    }

    Response* logoutTest() {
        message.verb = "DELETE";
        message.uri = "/users/test@yahoo.com/logout";
        message.body = ""; 
        message.params = "token:eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
        return logout->erase(message);
    }
};

/*TEST_F(TestAttendant, testPostRegisterDoneCorrectly) {
    ASSERT_EQ(response_register->getStatus(), 201);
    ASSERT_EQ(response_register->getContent(), "{\"registration\":\"OK\"}");
}

TEST_F(TestAttendant, testCannotRegisterTwice) {
    message.verb = "POST";
    message.body = "{\"email\":\"test@yahoo.com\",\"password\":\"admin\",\"device_id\":\"123\",\"first_name\":\"test\",\"last_name\":\"T\",\"gender\":\"M\",\"birthday\":\"01/01/2000\",\"address\":{\"lat\":\"123456789\",\"lon\":\"12345678\"}}";
    message.params="";
    Response *response = reg->post(message);
    ASSERT_EQ(response->getStatus(), 500);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(CLIENT_ALREADY_EXISTS) + ",\"message\":\"Client already exists.\"}");
    delete response;
}

TEST_F(TestAttendant, testCannotRegisterWithEmptyFields) {
    message.verb = "POST";
    message.body = "{\"email\":\"\",\"password\":\"\",\"device_id\":\"\",\"first_name\":\"\",\"last_name\":\"\",\"gender\":\"M\",\"birthday\":\"01/01/2000\",\"address\":{\"lat\":\"123456789\",\"lon\":\"12345678\"}}";
    message.params="";
    Response *response = reg->post(message);
    ASSERT_EQ(response->getStatus(), 500);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(EMPTY_FIELDS) + ",\"message\":\"There are empty fields.\"}");
    delete response;
}

TEST_F(TestAttendant, testGetRecoveryPasswordSuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/recovery_pass";
    message.body = "";
    message.params="";
    Response *response = recovery_pass->get(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "{\"password\":\"admin\"}");
    delete response;
}

TEST_F(TestAttendant, testGetRecoveryPasswordUnsuccessfully) {
    message.verb = "GET";
    message.uri = "/users/blabla@yahoo.com/recovery_pass";
    message.body = "";
    message.params="";
    Response *response = recovery_pass->get(message);
    ASSERT_EQ(response->getStatus(), 500);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(CLIENT_NOT_EXISTS) + ",\"message\":\"Client not exists.\"}");
    delete response;
}

TEST_F(TestAttendant, testPostLoginUnsuccessfully) {
    message.verb = "POST";
    message.uri = "/users/login";
    message.body = "{\"email\":\"blabla@yahoo.com\",\"password\":\"admin\"}";
    message.params = "";
    Response *response = login->post(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testPostContactUnsuccessfully) {
    message.verb = "POST";
    message.uri = "/users/test@yahoo.com/contact";
    message.body = "";
    message.params = "date=03-01-2010T12:34:00.000Z&email=contact@gmail.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = contact->post(message);
    ASSERT_EQ(response->getStatus(), 500);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(COULD_NOT_POST) + ",\"message\":\"Could not post.\"}");
    delete response;
}

TEST_F(TestAttendant, testPostContactSuccessfully) {
    Register *regist = new Register();
    message.verb = "POST";
    message.body = "{\"email\":\"contact@gmail.com\",\"password\":\"admin\",\"device_id\":\"321\",\"first_name\":\"cont\",\"last_name\":\"act\",\"gender\":\"M\",\"birthday\":\"01/01/2000\",\"address\":{\"lat\":\"123456789\",\"lon\":\"12345678\"}}";
    message.params = "";
    response_register = regist->post(message);
    delete regist;

    message.verb = "POST";
    message.uri = "/users/test@yahoo.com/contact";
    message.body = "";
    message.params = "date=03-01-2010T12:34:00.000Z&email=contact@gmail.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = contact->post(message);
    ASSERT_EQ(response->getStatus(), 201);
    ASSERT_EQ(response->getContent(), "");
    delete response;
}

TEST_F(TestAttendant, testGetContactUnsuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/contact";
    message.body = "";
    message.params = "token:eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6ImNvbnRhY3RAZ21haWwuY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.5a4NlJZMTFESSA3QWnX2Mwjt2vA2LFsB2wa6W2bc3zA";
    Response *response = contact->get(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testGetContactSuccessfully) {
    message.verb = "GET";
    message.uri = "/users/contact@gmail.com/contact";
    message.body = "";
    message.params = "token:eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6ImNvbnRhY3RAZ21haWwuY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.5a4NlJZMTFESSA3QWnX2Mwjt2vA2LFsB2wa6W2bc3zA";
    Response *response = contact->get(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "{\"solicitudes\":[{\"date\":\"03-01-2010T12:34:00.000Z\",\"email\":\"test@yahoo.com\"}]}");
    delete response;
}

TEST_F(TestAttendant, testPostAcceptUnsuccessfully) {
    message.verb = "POST";
    message.uri = "/users/test@yahoo.com/accept";
    message.body = "";
    message.params = "date=03-01-2010T12:34:00.000Z&email=contact@gmail.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = accept->post(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testPostAcceptContactSuccessfully) {
    message.verb = "POST";
    message.uri = "/users/contact@gmail.com/accept";
    message.body = "";
    message.params = "date=03-01-2010T12:34:00.000Z&email=test@yahoo.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6ImNvbnRhY3RAZ21haWwuY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.5a4NlJZMTFESSA3QWnX2Mwjt2vA2LFsB2wa6W2bc3zA";
    Response *response = accept->post(message);
    ASSERT_EQ(response->getStatus(), 201);
    ASSERT_EQ(response->getContent(), "");
    delete response;
}

TEST_F(TestAttendant, testPostAcceptNonExistentContactSuccessfully) {
    message.verb = "POST";
    message.uri = "/users/test@yahoo.com/accept";
    message.body = "";
    message.params = "date=03-01-2010T12:34:00.000Z&email=accept@gmail.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = accept->post(message);
    ASSERT_EQ(response->getStatus(), 500);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(NO_SOLICITUDE_SENT) + ",\"message\":\"User did not send solicitude.\"}");
    delete response;
}

TEST_F(TestAttendant, testDeleteRejectUnsuccessfully) {
    Register *regist = new Register();
    message.verb = "POST";
    message.body = "{\"email\":\"a@a.com\",\"password\":\"admin\",\"device_id\":\"222\",\"first_name\":\"a\",\"last_name\":\"a\",\"gender\":\"M\",\"birthday\":\"01/01/2000\",\"address\":{\"lat\":\"123456789\",\"lon\":\"12345678\"}}";
    message.params = "";
    response_register = regist->post(message);
    delete regist;

    message.verb = "POST";
    message.uri = "/users/test@yahoo.com/contact";
    message.body = "";
    message.params = "date=03-01-2010T12:34:00.000Z&email=a@a.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = contact->post(message);
    delete response;

    message.verb = "DELETE";
    message.uri = "/users/a@a.com/reject";
    message.body = "";
    message.params = "date=03-01-2010T12:34:00.000Z&email=test@yahoo.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    response = reject->erase(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testDeleteRejectSuccessfully) {
    message.verb = "POST";
    message.uri = "/users/test@yahoo.com/contact";
    message.body = "";
    message.params = "date=03-01-2010T12:34:00.000Z&email=a@a.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = contact->post(message);
    delete response;

    message.verb = "DELETE";
    message.uri = "/users/a@a.com/reject";
    message.body = "";
    message.params = "date=03-01-2010T12:34:00.000Z&email=test@yahoo.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6ImFAYS5jb20iLCJpbmNyZW1lbnRhbF9udW1iZXIiOjAsInBhc3N3b3JkIjoiYWRtaW4ifQ._q7591YbCv2c-Rm_6qQfEbHwCMD_qlAWNhqRWwPvxRM";
    response = reject->erase(message);
    ASSERT_EQ(response->getStatus(), 204);
    ASSERT_EQ(response->getContent(), "");
    delete response;
}

TEST_F(TestAttendant, testDeleteRejectNonExistentContactSuccessfully) {
    message.verb = "DELETE";
    message.uri = "/users/test@yahoo.com/reject";
    message.body = "";
    message.params = "date=03-01-2010T12:34:00.000Z&email=accept@gmail.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = reject->erase(message);
    ASSERT_EQ(response->getStatus(), 500);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(NO_SOLICITUDE_SENT) + ",\"message\":\"User did not send solicitude.\"}");
    delete response;
}

TEST_F(TestAttendant, testGetProfilePersonalUnsuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/personal";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = personal->get(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testGetProfilePersonalSuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/personal";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = personal->get(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "{\"first_name\":\"test\",\"last_name\":\"T\",\"email\":\"test@yahoo.com\",\"birthday\":\"01/01/2000\",\"address\":{\"lat\":\"123456789\", \"lon\":\"12345678\"},\"gender\":\"M\"}");
    delete response;
}

TEST_F(TestAttendant, testPutProfilePersonalUnsuccessfully) {
    message.verb = "PUT";
    message.uri = "/users/test@yahoo.com/profile/personal";
    message.body = "{\"first_name\":\"Donnal\",\"last_name\":\"Trump\",\"birthday\":\"23/05/1960\",\"gender\":\"M\",\"address\":{\"lat\":\"-11.9302\",\"lon\":\"-77.0846\"}}";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = personal->put(message);
    ASSERT_EQ(response->getStatus(), 500);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(COULD_NOT_PUT) + ",\"message\":\"Could not upload.\"}");
    delete response;
}

TEST_F(TestAttendant, testPutProfilePersonalSuccessfully) {
    message.verb = "PUT";
    message.uri = "/users/test@yahoo.com/profile/personal";
    message.body = "{\"first_name\":\"Donnal\",\"last_name\":\"Trump\",\"birthday\":\"23/05/1960\",\"gender\":\"M\",\"address\":{\"lat\":\"-11.9302\",\"lon\":\"-77.0846\"}}";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = personal->put(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "");
    delete response;
}

TEST_F(TestAttendant, testGetProfileSummaryUnsuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/summary";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = summary->get(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testGetProfileSummarySuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/summary";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = summary->get(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "{\"summary\":\"\"}");
    delete response;
}

TEST_F(TestAttendant, testPutProfileSummaryUnsuccessfully) {
    message.verb = "PUT";
    message.uri = "/users/test@yahoo.com/profile/summary";
    message.body = "{\"summary\": \"Future president of the USA\"}";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = summary->put(message);
    ASSERT_EQ(response->getStatus(), 500);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(COULD_NOT_PUT) + ",\"message\":\"Could not upload.\"}");
    delete response;
}

TEST_F(TestAttendant, testPutProfileSummarySuccessfully) {
    message.verb = "PUT";
    message.uri = "/users/test@yahoo.com/profile/summary";
    message.body = "{\"summary\": \"Future president of the USA\"}";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = summary->put(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "");
    delete response;
}

TEST_F(TestAttendant, testGetProfileExpertiseUnsuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/expertise";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = expertise->get(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testGetProfileExpertiseSuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/expertise";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = expertise->get(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "{\"expertises\":[]}");
    delete response;
}

TEST_F(TestAttendant, testPutProfileExpertiseUnsuccessfully) {
    message.verb = "PUT";
    message.uri = "/users/test@yahoo.com/profile/expertise";
    message.body = "{\"expertises\":[{\"company\":\"LalalaDOS\",\"position\":\"Lider tecnico\",\"from\":\"01/01/2010\",\"to\":\"26/09/2016\",\"expertise\":\"aaer\",\"category\":\"software\"},{\"company\":\"LololoDOS\",\"position\":\"Especialista tecnico\",\"from\":\"02/02/2012\",\"to\":\"02/02/2016\",\"expertise\":\"aaer\",\"category\":\"hardware\"}]}";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = expertise->put(message);
    ASSERT_EQ(response->getStatus(), 500);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(COULD_NOT_PUT) + ",\"message\":\"Could not upload.\"}");
    delete response;
}

TEST_F(TestAttendant, testPutProfileExpertiseSuccessfully) {
    message.verb = "PUT";
    message.uri = "/users/test@yahoo.com/profile/expertise";
    message.body = "{\"expertises\":[{\"company\":\"LalalaDOS\",\"position\":\"Lider tecnico\",\"from\":\"01/01/2010\",\"to\":\"26/09/2016\",\"expertise\":\"aaer\",\"category\":\"software\"},{\"company\":\"LololoDOS\",\"position\":\"Especialista tecnico\",\"from\":\"02/02/2012\",\"to\":\"02/02/2016\",\"expertise\":\"aaer\",\"category\":\"hardware\"}]}";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = expertise->put(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "");
    delete response;
}

TEST_F(TestAttendant, testGetProfileSkillsUnsuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/skills";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = skills->get(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testGetProfileSkillsSuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/skills";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = skills->get(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "{\"every_skill\":[]}");
    delete response;
}

TEST_F(TestAttendant, testPutProfileSkillsUnsuccessfully) {
    message.verb = "PUT";
    message.uri = "/users/test@yahoo.com/profile/skills";
    message.body = "{\"every_skill\":[{\"skills\":[\"java\",\"c\",\"UML\"],\"category\":\"software\"},{\"skills\":[\"moto\",\"auto\"],\"category\":\"licencia_manejo\"}]}";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = skills->put(message);
    ASSERT_EQ(response->getStatus(), 500);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(COULD_NOT_PUT) + ",\"message\":\"Could not upload.\"}");
    delete response;
}

TEST_F(TestAttendant, testPutProfileSkillsSuccessfully) {
    message.verb = "PUT";
    message.uri = "/users/test@yahoo.com/profile/skills";
    message.body = "{\"every_skill\":[{\"skills\":[\"java\",\"c\",\"UML\"],\"category\":\"software\"},{\"skills\":[\"moto\",\"auto\"],\"category\":\"licencia_manejo\"}]}";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = skills->put(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "");
    delete response;
}

TEST_F(TestAttendant, testGetProfilePictureUnsuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/picture";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = picture->get(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testGetProfilePictureSuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/picture";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = picture->get(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "{\"picture\":\"\"}");
    delete response;
}

TEST_F(TestAttendant, testPutProfilePictureUnsuccessfully) {
    message.verb = "PUT";
    message.uri = "/users/test@yahoo.com/profile/picture";
    message.body = "{\"picture\":\"asdf1234asdf\"}";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = picture->put(message);
    ASSERT_EQ(response->getStatus(), 500);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(COULD_NOT_PUT) + ",\"message\":\"Could not upload.\"}");
    delete response;
}

TEST_F(TestAttendant, testPutProfilePictureSuccessfully) {
    message.verb = "PUT";
    message.uri = "/users/test@yahoo.com/profile/picture";
    message.body = "{\"picture\":\"asdf1234asdf\"}";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = picture->put(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "");
    delete response;
}

TEST_F(TestAttendant, testGetProfileFriendsUnsuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/contacts";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = friends->get(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testGetProfileFriendsSuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/contacts";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = friends->get(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "{\"friends\":[{\"email\":\"contact@gmail.com\",\"first_name\":\"cont\",\"last_name\":\"act\",\"votes\":0,\"thumbnail\":\"\"}]}");
    delete response;
}

TEST_F(TestAttendant, testGetOwnRecommendationsUnsuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/own_recommendations";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = own_recommendations->get(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testGetOwnRecommendationsSuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/own_recommendations";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = own_recommendations->get(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "{\"own_recommendations\":[]}");
    delete response;
}

TEST_F(TestAttendant, testGetOthersRecommendationsUnsuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/others_recommendations";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = others_recommendations->get(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testGetOthersRecommendationsSuccessfully) {
    message.verb = "GET";
    message.uri = "/users/test@yahoo.com/profile/others_recommendations";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = others_recommendations->get(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "{\"others_recommendations\":[]}");
    delete response;
}

TEST_F(TestAttendant, testPostVoteUnsuccessfully) {
    message.verb = "POST";
    message.uri = "/users/test@yahoo.com/vote";
    message.body = "";
    message.params = "email=contact@gmail.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = vote->post(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testPostVoteSuccessfully) {
    message.verb = "POST";
    message.uri = "/users/test@yahoo.com/vote";
    message.body = "";
    message.params = "email=contact@gmail.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = vote->post(message);
    ASSERT_EQ(response->getStatus(), 201);
    ASSERT_EQ(response->getContent(), "");
    delete response;
}

TEST_F(TestAttendant, testDeleteVoteUnsuccessfully) {
    message.verb = "DELETE";
    message.uri = "/users/test@yahoo.com/vote";
    message.body = "";
    message.params = "email=contact@gmail.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = vote->post(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testDeleteVoteSuccessfully) {
    message.verb = "DELETE";
    message.uri = "/users/test@yahoo.com/vote";
    message.body = "";
    message.params = "email=contact@gmail.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = vote->post(message);
    ASSERT_EQ(response->getStatus(), 201);
    ASSERT_EQ(response->getContent(), "");
    delete response;
}

TEST_F(TestAttendant, testGetSearchUnsuccessfully) {
    message.verb = "GET";
    message.uri = "/users";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ";
    Response *response = search->get(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testGetSearchSuccessfully) {
    message.verb = "GET";
    message.uri = "/users";
    message.body = "";
    message.params = "token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4";
    Response *response = search->get(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "{\"paging\":{\"total\":3,\"offset\":0},\"results\":[{\"email\":\"test@yahoo.com\",\"first_name\":\"Donnal\",\"last_name\":\"Trump\",\"distance\":\"\",\"votes\":0,\"thumbnail\":\"asdf1234asdf\"},{\"email\":\"contact@gmail.com\",\"first_name\":\"cont\",\"last_name\":\"act\",\"distance\":\"\",\"votes\":1,\"thumbnail\":\"\"},{\"email\":\"a@a.com\",\"first_name\":\"a\",\"last_name\":\"a\",\"distance\":\"\",\"votes\":0,\"thumbnail\":\"\"}]}");
    delete response;
}

TEST_F(TestAttendant, testDeleteLogoutSuccessfully) {
    Response *response = this->loginTest();
    delete response;
    response = this->logoutTest();
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "");
    delete response;
}*/
