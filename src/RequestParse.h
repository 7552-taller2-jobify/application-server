// "Copyright 2016 <Jobify>"

#ifndef REQUESTPARSE_H_
#define REQUESTPARSE_H_

#include "Constants.h"
#include "Logger.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class RequestParse {
 private:
    std::string getHTTPVerb(struct http_message* hm);
    int areEqual(const struct mg_str *s1, const struct mg_str *s2);

 public:
    RequestParse();

    Message* parseRequest(struct http_message* hm);

    virtual ~RequestParse();

    std::string extractEmail(std::string uri);

    std::vector<std::string> split(std::string uri, std::string separator);
};

#endif // REQUESTPARSE_H_
