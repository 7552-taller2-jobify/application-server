// "Copyright 2016 <Jobify>"

#include "ProfileHandler.h"
#include <string>
#include <map>

ProfileHandler::ProfileHandler() {
    profiles["PERSONAL_.*"] = new Personal();
    profiles["SUMMARY_.*"] = new Summary();
    profiles["EXPERTISE_.*"] = new Expertise();
    profiles["SKILLS_.*"] = new Skills();
    profiles["PICTURE_.*"] = new Picture();
    profiles[".*@.*"] = new LoginInformation();
    beginings[0] = "PERSONAL_";
    beginings[1] = "SUMMARY_";
    beginings[2] = "EXPERTISE_";
    beginings[3] = "SKILLS_";
    beginings[4] = "PICTURE_";
}

ProfileHandler::~ProfileHandler() {}

Profile* ProfileHandler::find(std::string key_to_match) {
    RegexMatcher regex_matcher;
    for (std::map<std::string, Profile*>::iterator it = this->profiles.begin(); it != this->profiles.end(); it++) {
        if (this->checkBegining(key_to_match) &&  (!this->checkBegining(it->first))) {
            continue;
        }
        if (regex_matcher.match(key_to_match.c_str(), it->first.c_str()) == 1) {
            return it->second;
        }
    }
    return NULL;
}

bool ProfileHandler::checkBegining(std::string key_to_match) {
    for (int i = 0; i < ((sizeof(beginings)/sizeof(*beginings)) - 1); i++) {
        int length;
        if (strlen(beginings[i].c_str()) > strlen(key_to_match.c_str())) {
            length = strlen(key_to_match.c_str());
        } else {
            length = strlen(beginings[i].c_str());
        }
        if (strncmp(key_to_match.c_str(), beginings[i].c_str(), length - 1) == 0) {
            return true;
        }
    }
    return false;
}
