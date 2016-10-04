// "Copyright 2016 <Jobify>"

#ifndef REQUESTADMINISTRATOR_H_
#define REQUESTADMINISTRATOR_H_

#include <iostream>
#include "mongoose/mongoose.h"

class RequestAdministrator {
 public:
    RequestAdministrator();
    ~RequestAdministrator();
    void handle(struct mg_connection *c, int ev, void *p);
};

#endif  // REQUESTADMINISTRATOR_H_
