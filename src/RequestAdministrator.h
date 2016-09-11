#ifndef REQUESTADMINISTRTOR_H_
#define REQUESTADMINISTRTOR_H_
#include <iostream>
#include "mongoose.h"

using namespace std;

class RequestAdministrator{
private:

public:
		RequestAdministrator();		
		~RequestAdministrator();
		void handle(struct mg_connection *c, int ev, void *p);


};
#endif
