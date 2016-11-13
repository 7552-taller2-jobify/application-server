/*
 *  Authentication.h
 *
 *  Created on: Sep 17, 2016
 *      Author: hernan-fiuba
 */

#ifndef AUTHENTICATION_H_
#define AUTHENTICATION_H_

#include <stdio.h>
#include <string>
#include "jwt.h"  // Include JWT API definitions
#include "Constants.h"
 
class Authentication {
 private:
    jwt* myJWT;
 public:
    Authentication();
    std::string encode(std::string email, std::string password);
    bool isDecode(std::string token);
    virtual ~Authentication();
};

#endif /* AUTHENTICATION_H_ */
