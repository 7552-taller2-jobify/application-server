#ifndef PROFILEHANDLER_H_
#define PROFILEHANDLER_H_

#include <regex.h>
#include <string>
#include <map>
#include "Profile.h"
#include "RegexMatcher.h"

using namespace std;

class ProfileHandler {
private:
	map<string, Profile*> profiles;

public:
	ProfileHandler();
	~ProfileHandler();
	Profile* find(string key_to_match);
};

#endif
