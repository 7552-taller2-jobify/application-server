// "Copyright 2016 <Jobify>"

#ifndef DATABASEADMINISTRATOR_H_
#define DATABASEADMINISTRATOR_H_

#include <string>
#include <iostream>
#include <vector>
#include "DataBase.h"
#include "Profile.h"
#include "Logger.h"
#include "Constants.h"
#include "UserService.h"
#include "Authentication.h"

class DataBaseAdministrator {
 private:
    Authentication *auth;
    std::vector<struct PopularUser> searchRange(const std::string start, const std::string end);

 public:
    DataBaseAdministrator();
    ~DataBaseAdministrator();
    bool existsClient(std::string email);
    bool rightClient(std::string email, std::string token);
    bool rightClient(LoginInformation *li);
    std::string getPersonalLogin(std::string email);
    std::string getPersonal(std::string email);
    std::string getProfilePersonal(std::string email);
    std::string getSummary(std::string email);
    std::string getExpertise(std::string email);
    std::string getSkills(std::string email);
    std::string getPicture(std::string email);
    std::string getSolicitudes(std::string email);
    int addClient(Personal* personal, LoginInformation *loginInformation, struct Message operation);
    int createPersonal(std::string email, std::string token, Personal *upload_personal);
    int uploadPersonal(std::string email, std::string token, Personal *personal);
    int createSummary(std::string email, std::string token, Summary *summary);
    int uploadSummary(std::string email, std::string token, Summary *summary);
    int createExpertise(std::string email, std::string token, Expertise *upload_expertise);
    int uploadExpertise(std::string email, std::string token, Expertise *expertise);
    int createSkills(std::string email, std::string token, Skills *skills);
    int uploadSkills(std::string email, std::string token, Skills *skills);
    int createPicture(std::string email, std::string token, Picture *picture);
    int uploadPicture(std::string email, std::string token, Picture *picture);
    int addSolicitude(std::string email, std::string token, struct Solicitude new_solicitude);
    int addFriend(std::string email, struct Solicitude solicitude_to_delete);
    int removeSolicitude(std::string email, struct Solicitude solicitude_to_delete);
    std::string getFriends(std::string email);
    std::string getOwnRecommendations(std::string email);
    std::string getOthersRecommendations(std::string email);
    void vote(std::string email, std::string email_to_vote);
    void unvote(std::string email, std::string email_to_unvote);
    std::string getMostPopularUsers();
    std::vector<std::string>* getAllIds();
    int resetPassword(std::string email);
};

#endif  // DATABASEADMINISTRATOR_H_
