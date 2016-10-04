// "Copyright 2016 <Jobify>"

#include "RegexMatcher.h"
#include <string>

RegexMatcher::RegexMatcher() {}

RegexMatcher::~RegexMatcher() {}

// Returns 0 if the pattern is incorrect.
// Returns 1 is there was match.
// Returns 2 if there was no match.
int RegexMatcher::match(const char *string_to_compare, const char *pattern) {
    int status;
    regex_t re;
    regmatch_t rm;
    if (regcomp(&re, pattern, REG_EXTENDED) != 0) {
        Logger::getInstance().log(warn, "Bad pattern: " + std::string(pattern));
        return 0;
    }
    status = regexec(&re, string_to_compare, 1, &rm, 0);
    regfree(&re);
    if (status != 0) {
        return 2;
    }
    return 1;
}
