// "Copyright 2016 <Jobify>"
#ifndef REQUEST_H_
#define REQUEST_H_

#include <iostream>
#include <sstream>
#include <string>
#include "curl/curl.h"
#include "Response.h"

class Request{
 private:
    void* curl;
    struct curl_slist *slist1;
 public:
    Request();
    Response* Execute(std::string url);
    Response* ExecutePost(std::string url, std::string body);
    void SetContentJson();
    void SetAuthorization(std::string authorization);
    ~Request();
};
#endif  // REQUEST_H_
