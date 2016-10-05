// "Copyright 2016 <Jobify>"

#ifndef REQUESTADMINISTRATOR_H_
#define REQUESTADMINISTRATOR_H_

#include <iostream>
#include "mongoose/mongoose.h"
#include "RequestParse.h"
#include <string>
#include "Constants.h"

class RequestAdministrator {
private:
    struct mg_connection *c;
    int ev;
    struct http_message* hm;
    RequestParse* rp;	

public:
    RequestAdministrator(struct mg_connection *c, int ev, struct http_message* p);
    ~RequestAdministrator();
    void handle();
};

#endif  // REQUESTADMINISTRATOR_H_
