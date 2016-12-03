// "Copyright 2016 <Jobify>"
#ifndef AUTHENTICATION_H_
#define AUTHENTICATION_H_

#include <stdio.h>
#include <string>
#include "jwt.h"  // Include JWT API definitions
#include "Constants.h"
#include "Profile.h"
 
class Authentication {
 private:
    jwt* myJWT;
 public:
    Authentication();
    std::string encode(std::string email, std::string password, int numeroIncremental);
    bool decode(std::string token, LoginInformation *loginInformation, Credentials *credentials);
    virtual ~Authentication();
    std::string getEmailFromToken(std::string token);
};

#endif  /* AUTHENTICATION_H_ */
