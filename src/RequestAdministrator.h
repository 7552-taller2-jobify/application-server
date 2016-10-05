#ifndef REQUESTADMINISTRTOR_H_
#define REQUESTADMINISTRTOR_H_
#include <iostream>
#include "mongoose.h"
#include "ProfileHandler.h"
#include "Profile.h"
#include "RequestParse.h"
#include <string>
#include "Constants.h"

using namespace std;

class RequestAdministrator{
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
#endif
