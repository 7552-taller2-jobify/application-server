// "Copyright 2016 <Jobify>"

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
    std::vector<std::string> beginings;
    bool checkBegining(std::string key_to_match);

 public:
    ProfileHandler();
    ~ProfileHandler();
    Profile* find(std::string key_to_match);
};

#endif  // PROFILEHANDLER_H_
