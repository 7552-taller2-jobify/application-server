// "Copyright 2016 <Jobify>"

#ifndef DATABASEADMINISTRATOR_H_
#define DATABASEADMINISTRATOR_H_

#include <string>
#include <iostream>
#include "DataBase.h"
#include "Profile.h"
#include "Logger.h"
#include "Constants.h"

class DataBaseAdministrator {
    public:
        DataBaseAdministrator();
        ~DataBaseAdministrator();
        bool existsClient(std::string email);
        std::string getDataOfClient(Profile *loginInformation);
        int addClient(Personal* personal, struct Message operation);
};

#endif  // DATABASEADMINISTRATOR_H_
