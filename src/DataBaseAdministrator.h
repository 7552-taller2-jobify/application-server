// "Copyright 2016 <Jobify>"

#ifndef DATABASEADMINISTRATOR_H_
#define DATABASEADMINISTRATOR_H_

#include <string>
#include <iostream>
#include "DataBase.h"
#include "Profile.h"
#include "Logger.h"

class DataBaseAdministrator {
    public:
        DataBaseAdministrator();
        ~DataBaseAdministrator();
        bool existsClient(Profile *loginInformation);
        std::string getDataOfClient(Profile *loginInformation);
        void addClient(Profile *loginInformation, Profile* personal);	


};

#endif  // DATABASEADMINISTRATOR_H_
