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
    bool rigthClient(std::string email, std::string token);     
    bool rigthClient(LoginInformation *li);
    std::string getPersonalLogin(std::string email);
    std::string getPersonal(std::string email);
    std::string getSummary(std::string email);
    std::string getExpertise(std::string email);
    std::string getSkills(std::string email);
    std::string getPicture(std::string email);
    std::string getSolicitudes(std::string email);
    int addClient(Personal* personal, LoginInformation *loginInformation, struct Message operation);
    int uploadPersonal(std::string email, std::string token, Personal *personal);
    int uploadSummary(std::string email, std::string token, Summary *summary);
    int uploadExpertise(std::string email, std::string token, Expertise *expertise);
    int uploadSkills(std::string email, std::string token, Skills *skills);
    int uploadPicture(std::string email, std::string token, Picture *picture);
    int addSolicitude(std::string email, std::string token, struct Solicitude new_solicitude);
};

#endif  // DATABASEADMINISTRATOR_H_
