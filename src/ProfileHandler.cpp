#include "ProfileHandler.h"

ProfileHandler::ProfileHandler() {
	profiles["personal_.*"] = new Personal();
	profiles["summary_.*"] = new Summary();
	profiles["expertise_.*"] = new Expertise();
	profiles["skills_.*"] = new Skills();
	profiles["picture_.*"] = new Picture();
}

ProfileHandler::~ProfileHandler() {}

Profile* ProfileHandler::find(string key_to_match) {
	RegexMatcher regex_matcher;
	for(map<string, Profile*>::iterator it = this->profiles.begin(); it != this->profiles.end(); it++) {
		if(regex_matcher.match(key_to_match.c_str(), it->first.c_str()) == 1) {
			return it->second;
		}
	}
	return NULL;
}