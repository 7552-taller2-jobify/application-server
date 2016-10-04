// "Copyright 2016 <Jobify>"

#include "AttendantsHandler.h"
#include <string>
#include <map>

AttendantsHandler::AttendantsHandler() {
    attendants["Login"] = new Login();
    attendants["RecoveryPass"] = new RecoveryPass();
    attendants["Contact"] = new Contact();
    attendants["Accept"] = new Accept();
    attendants["Reject"] = new Reject();
    attendants["ProfilePersonal"] = new ProfilePersonal();
    attendants["ProfileSummary"] = new ProfileSummary();
    attendants["ProfileExpertise"] = new ProfileExpertise();
    attendants["ProfileSkills"] = new ProfileSkills();
    attendants["ProfilePhoto"] = new ProfilePhoto();
}

AttendantsHandler::~AttendantsHandler() {}

Attendant* AttendantsHandler::find(std::string key_to_match) {
    for (std::map<std::string, Attendant*>::iterator it =
                this->attendants.begin(); it != this->attendants.end(); it++) {
        if (key_to_match == it->first) {
            return it->second;
        }
    }
    return NULL;
}
