// "Copyright 2016 <Jobify>"

#ifndef DATABASEADMINISTRATOR_H_
#define DATABASEADMINISTRATOR_H_

#include <string>
#include <iostream>
#include "DataBase.h"
#include "Profile.h"
#include "Logger.h"
#include "Constants.h"
#include "UserService.h"
#include "Authentication.h"

class DataBaseAdministrator {
 private:
    Authentication *auth;
 public:
    DataBaseAdministrator();
    ~DataBaseAdministrator();
    bool existsClient(std::string email);
    bool rigthClient(LoginInformation *li);
    std::string getDataOfClient(LoginInformation *loginInformation);
    int addClient(Personal* personal, LoginInformation *loginInformation, struct Message operation);
};

#endif  // DATABASEADMINISTRATOR_H_
