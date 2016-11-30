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

    void SetUp() {
        reg = new Register();
        message.verb = "POST";
        message.body = "{\"email\":\"test@yahoo.com\",\"password\":\"admin\",\"device_id\":\"123\",\"first_name\":\"test\",\"last_name\":\"T\",\"gender\":\"M\",\"birthday\":\"01/01/2000\",\"address\":{\"lat\":\"123456789\",\"lon\":\"12345678\"}}";
        message.params="";
        response_register = reg->post(message);
        login = new Login();
        logout = new Logout();
    }

    ~TestAttendant() {
        delete login;
        delete reg;
        delete response_register;
        delete logout;
    }

    Response* loginTest() {
        message.verb = "POST";
        message.uri = "/users/login";
        message.body = "{\"email\":\"test@yahoo.com\",\"password\":\"admin\"}";
        message.params="";
        return login->post(message);
    }

    Response* logoutTest() {
        message.verb = "DELETE";
        message.uri = "/users/test@yahoo.com/logout";
        message.body = "";
        message.params="{\"token\": \"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4\"}";
        return logout->erase(message);
    }
};

TEST_F(TestAttendant, testPostRegisterDoneCorrectly) {
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

/*TEST_F(TestAttendant, testPostLoginSuccessfully) {
    message.verb = "POST";
    message.uri = "/users/login";
    message.body = "{\"email\":\"test@yahoo.com\",\"password\":\"admin\"}";
    message.params="";
    Response *response = login->post(message);
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "{\"password\":\"admin\"}");
    delete response;
}*/

TEST_F(TestAttendant, testPostLoginUnsuccessfully) {
    message.verb = "POST";
    message.uri = "/users/login";
    message.body = "{\"email\":\"blabla@yahoo.com\",\"password\":\"admin\"}";
    message.params="";
    Response *response = login->post(message);
    ASSERT_EQ(response->getStatus(), 401);
    ASSERT_EQ(response->getContent(), "{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
    delete response;
}

TEST_F(TestAttendant, testDeleteLogoutSuccessfully) {
    /*Response *response = this->loginTest();
    delete response;*/this->loginTest();
    Response *response = this->logoutTest();
    ASSERT_EQ(response->getStatus(), 200);
    ASSERT_EQ(response->getContent(), "");
    delete response;
}

/*TEST_F(TestAttendant, testPostFacebookRegisterDoneCorrectly) {
    message.verb = "POST";
    message.body = "";
    message.params="app=facebook";
    response = reg->post(message);
    ASSERT_EQ(response->getStatus(), 201);
    ASSERT_EQ(response->getContent(), "{\"registration\":\"OK\"}");
}*/
