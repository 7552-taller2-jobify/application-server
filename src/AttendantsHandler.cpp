// "Copyright 2016 <Jobify>"

#include "AttendantsHandler.h"
#include <string>
#include <map>

AttendantsHandler::AttendantsHandler() {
    attendants["Login"] = new Login();
    attendants["Logout"] = new Logout();
    attendants["Register"] = new Register();
    attendants["RecoveryPass"] = new RecoveryPass();
    attendants["Contact"] = new Contact();
    attendants["Accept"] = new Accept();
    attendants["Reject"] = new Reject();
    attendants["ProfilePersonal"] = new ProfilePersonal();
    attendants["ProfileSummary"] = new ProfileSummary();
    attendants["ProfileExpertise"] = new ProfileExpertise();
    attendants["ProfileSkills"] = new ProfileSkills();
    attendants["ProfilePhoto"] = new ProfilePhoto();
    attendants["Facebook"] = new Facebook();
    attendants["Firebase"] = new Firebase();
    attendants["Category"] = new Category();
    attendants["Skill"] = new Skill();
    attendants["JobPositions"] = new JobPosition();
    attendants["ProfileFriends"] = new ProfileFriends();
    attendants["Vote"] = new Vote();
    attendants["ProfileOwnRecommendations"] = new ProfileOwnRecommendations();
    attendants["ProfileOthersRecommendations"] = new ProfileOthersRecommendations();
    attendants["MostPopularUsers"] = new MostPopularUsers();
    attendants["Search"] = new Search();
    this->url_mapper = new URLMapper();
}

AttendantsHandler::~AttendantsHandler() {
    delete this->url_mapper;
}

Attendant* AttendantsHandler::find(std::string uri) {
    std::string key_to_match = url_mapper->find(uri);

    for (std::map<std::string, Attendant*>::iterator it = attendants.begin(); it != attendants.end(); it++) {
        if (key_to_match == it->first) {
            return it->second;
        }
    }
    return NULL;
}
