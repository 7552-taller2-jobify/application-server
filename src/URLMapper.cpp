// "Copyright 2016 <Jobify>"

#include "URLMapper.h"
#include <string>
#include <map>

URLMapper::URLMapper() {
    if (!file.is_open()) {
        file.open(URL_FILE_PATH);
    }
    std::string line;
    while (getline(file, line)) {
        std::stringstream ss_line(line);
        std::string parameters[2];
        ss_line >> parameters[0] >> parameters[1];
        urls[parameters[0]] = parameters[1];
    }
    file.close();
}

URLMapper::~URLMapper() {}

std::string URLMapper::find(std::string key) {
    RegexMatcher regex_matcher;
    for (std::map<std::string, std::string>::iterator it = this->urls.begin();
                                                it != this->urls.end(); it++) {
        if (regex_matcher.match(key.c_str(), it->first.c_str()) == 1) {
            return it->second;
        }
    }
    return "";
}
