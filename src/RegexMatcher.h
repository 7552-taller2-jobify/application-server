// "Copyright 2016 <Jobify>"

#ifndef REGEXMATCHER_H_
#define REGEXMATCHER_H_

#include <regex.h>
#include <string>
#include "Logger.h"

class RegexMatcher {
 public:
    RegexMatcher();
    ~RegexMatcher();
    int match(const char *string_to_compare, const char *pattern);
};

#endif  // REGEXMATCHER_H_
