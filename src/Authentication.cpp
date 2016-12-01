// "Copyright 2016 <Jobify>"

#include "Authentication.h"
#include <iostream>
#include <string>

/* Constant time to make tests consistent. */
#define TS_CONST    1475980545L

Authentication::Authentication() {}

std::string Authentication::encode(std::string email, std::string password, int numeroIncremental) {
    jwt_new(&this->myJWT);
    //  SHA 256 es seguro, no  tiene colisiones
    jwt_set_alg(this->myJWT, JWT_ALG_HS256, (const unsigned char *) SECRET, SECRET_LEN);
    jwt_add_grant(this->myJWT, "email", email.c_str());
    jwt_add_grant(this->myJWT, "password", password.c_str());
    jwt_add_grant_int(this->myJWT, "incremental_number", numeroIncremental);
    std::string token = jwt_encode_str(this->myJWT);
    jwt_free(this->myJWT);
    return token;
}

bool Authentication::decode(std::string token, LoginInformation *loginInformation, Credentials *credentials) {
    jwt_new(&this->myJWT);
    int ret = jwt_decode(&this->myJWT, token.c_str(), (const unsigned char *)SECRET, SECRET_LEN);
    bool rightDecode = ret == 0;
    if (rightDecode) {
        std::string email = jwt_get_grant(this->myJWT, "email");
        loginInformation->setEmail(email);
        std::string password = jwt_get_grant(this->myJWT, "password");
        loginInformation->setPassword(password);
        credentials->setToken(token);
        int incremental_number = jwt_get_grant_int(this->myJWT, "incremental_number");
        credentials->setIncrementalNumber(incremental_number);
    }
    jwt_free(this->myJWT);
    return rightDecode;
}

std::string Authentication::getEmailFromToken(std::string token) {
    jwt_new(&this->myJWT);
    int ret = jwt_decode(&this->myJWT, token.c_str(), (const unsigned char *)SECRET, SECRET_LEN);
    return jwt_get_grant(this->myJWT, "email");
}

Authentication::~Authentication() {}
