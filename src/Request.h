#ifndef REQUEST_H_
#define REQUEST_H_

#include <iostream>
#include <sstream>
#include "curl/curl.h"
#include "Response.h"

using namespace std;

class Request {
 private:
    void* curl;

 public:
    Request();
    Response* Execute(string url);
    Response* ExecutePost(string url, string body);
    ~Request();
};
#endif // REQUEST_H_
