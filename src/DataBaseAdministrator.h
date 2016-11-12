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

class DataBaseAdministrator {
    public:
        DataBaseAdministrator();
        ~DataBaseAdministrator();
        bool existsClient(std::string email);
        std::string getDataOfClient(LoginInformation *loginInformation);
        std::string getPersonal(std::string email);
        int addClient(Personal* personal, struct Message operation);

        int uploadPersonal(std::string email, Personal *personal);
};

#endif  // DATABASEADMINISTRATOR_H_
