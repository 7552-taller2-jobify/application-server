// "Copyright 2016 <Jobify>"

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "mongoose.h"
#include <iostream>

static const struct mg_str PUT_FUNCTION = MG_STR("PUT");
static const struct mg_str GET_FUNCTION = MG_STR("GET");
static const struct mg_str POST_FUNCTION = MG_STR("POST");
static const struct mg_str DELETE_FUNCTION = MG_STR("DELETE");
static const char* SECRET = "AAAA";
static const int SECRET_LEN = 4;
static const int MAX_POPULAR_USERS = 10;

struct Message {
    std::string uri;
    std::string body;
    std::string verb;
    std::string params;
};

struct PopularUser {
    std::string email;
    int votes;
};

#endif // CONSTANTS_H_
