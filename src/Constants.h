#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "mongoose.h"
#include <iostream>

using namespace std;

static const struct mg_str PUT_FUNCTION = MG_STR("PUT");
static const struct mg_str GET_FUNCTION = MG_STR("GET");
static const struct mg_str POST_FUNCTION = MG_STR("POST");
static const struct mg_str DELETE_FUNCTION = MG_STR("DELETE");

struct Message {
  string type;
  string who_send;

  string data_json;
  string verb_http;
};

#endif /* CONSTANTS_H_ */