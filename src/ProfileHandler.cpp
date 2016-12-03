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
    profiles["FRIENDS_.*"] = new Friends();
    profiles["OWN_RECOMMENDATIONS_.*"] = new OwnRecommendations();
    profiles["OTHERS_RECOMMENDATIONS_.*"] = new OthersRecommendations();
    profiles["SOLICITUDES_.*"] = new Solicitudes();
    profiles[".*@.*"] = new LoginInformation();
    this->beginings.push_back("PERSONAL_");
    this->beginings.push_back("SUMMARY_");
    this->beginings.push_back("EXPERTISE_");
    this->beginings.push_back("SKILLS_");
    this->beginings.push_back("PICTURE_");
    this->beginings.push_back("FRIENDS_");
    this->beginings.push_back("OWN_RECOMMENDATIONS_");
    this->beginings.push_back("OTHERS_RECOMMENDATIONS_");
    this->beginings.push_back("SOLICITUDES_");
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
    for (int i = 0; i < beginings.size(); i++) {
        int length;
        if (strlen(beginings.at(i).c_str()) > strlen(key_to_match.c_str())) {
            length = strlen(key_to_match.c_str());
        } else {
            length = strlen(beginings.at(i).c_str());
        }
        if (strncmp(key_to_match.c_str(), beginings.at(i).c_str(), length - 1) == 0) {
            return true;
        }
    }
    return false;
}
