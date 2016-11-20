// "Copyright 2016 <Jobify>"
#include "Response.h"
#include <string>


Response::Response() {}

std::string Response::getContent() {
    return content;
}

int Response::getStatus() {
    return http_code;
}

void Response::setContent(std::string str) {
    content = str;
}

void Response::setStatus(int code) {
    http_code = code;
}

Response::~Response() {}
