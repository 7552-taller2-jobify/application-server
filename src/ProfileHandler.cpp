#include "ProfileHandler.h"

ProfileHandler::ProfileHandler() {
	profiles["^personal_(.*)"] = new Personal();
	profiles["^summary_(.*)"] = new Summary();
	profiles["^expertise_(.*)"] = new Expertise();
	profiles["^skills_(.*)"] = new Skills();
	profiles["^picture_(.*)"] = new Picture();
}

ProfileHandler::~ProfileHandler() {}

Profile* ProfileHandler::find(string key_to_match) {
	for(map<string, Profile*>::iterator it = this->profiles.begin(); it != this->profiles.end(); it++) {
		if (key_to_match == it->first) {
			return it->second;
		}
	}
	return NULL;
}
