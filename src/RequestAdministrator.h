#ifndef REQUESTADMINISTRTOR_H_
#define REQUESTADMINISTRTOR_H_

#include <iostream>
#include "mongoose.h"

class RequestAdministrator{
 public:
    RequestAdministrator();		
    ~RequestAdministrator();
    void handle(struct mg_connection *c, int ev, void *p);
};

#endif
