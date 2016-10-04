#ifndef PROFILEHANDLER_H_
#define PROFILEHANDLER_H_

#include <regex.h>
#include <string>
#include <map>
#include "Profile.h"
#include "RegexMatcher.h"

class ProfileHandler {
 private:
    std::map<std::string, Profile*> profiles;

 public:
    ProfileHandler();
    ~ProfileHandler();
    Profile* find(std::string key_to_match);
};

#endif
