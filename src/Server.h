#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include "mongoose.h"
#include "DataBase.h"
#include "RequestAdministrator.h"
#include "curl/curl.h"
#include "URLMapper.h"
#include "AttendantsHandler.h"

using namespace std;

class Server{
private:
	bool IsOnLine;
	DataBase *db;
	Logger *logger;
	URLMapper *url_mapper;
	AttendantsHandler *attendants_handler;

public:
	Server();
	~Server();
	friend void ev_handler(struct mg_connection *c, int ev, void *p);
	void start();
	void resolveRequest(string request);
};

#endif
