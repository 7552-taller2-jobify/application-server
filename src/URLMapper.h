// "Copyright 2016 <Jobify>"

#ifndef URLMAPPER_H_
#define URLMAPPER_H_

#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include "RegexMatcher.h"

const char URL_FILE_PATH[] = "files/url.txt";

class URLMapper {
 private:
    std::map<std::string, std::string> urls;
    std::ifstream file;

 public:
    URLMapper();
    ~URLMapper();
    std::string find(std::string key);
};

#endif  // URLMAPPER_H_
