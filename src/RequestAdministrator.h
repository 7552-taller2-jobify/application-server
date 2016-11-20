// "Copyright 2016 <Jobify>"

#ifndef REQUESTADMINISTRATOR_H_
#define REQUESTADMINISTRATOR_H_

#include <iostream>
#include <string>
#include "mongoose/mongoose.h"
#include "RequestParse.h"
#include "AttendantsHandler.h"
#include "Attendant.h"
#include "Constants.h"

class RequestAdministrator {
 private:
    struct mg_connection *c;
    int ev;
    struct http_message* hm;
    RequestParse* rp;
    AttendantsHandler* attendantHandler;

 public:
    RequestAdministrator(struct mg_connection *c, int ev, struct http_message* p);
    ~RequestAdministrator();
    void handle();
};

#endif  // REQUESTADMINISTRATOR_H_
