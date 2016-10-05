// "Copyright 2016 <Jobify>"

#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <string>
#include "mongoose/mongoose.h"
#include "DataBase.h"
#include "RequestAdministrator.h"
#include "curl/curl.h"
#include "URLMapper.h"
#include "AttendantsHandler.h"

class Server{
 private:
    bool IsOnLine;
    URLMapper *url_mapper;
    AttendantsHandler *attendants_handler;

 public:
    Server();
    ~Server();
    friend void ev_handler(struct mg_connection *c, int ev, void *p);
    void start();
    void resolveRequest(std::string request);
};

#endif  // SERVER_H_
