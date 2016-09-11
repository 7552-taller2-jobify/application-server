#ifndef SERVER_H_
#define SERVER_H_
#include <iostream>
#include "mongoose.h"
#include "DataBase.h"
#include "RequestAdministrator.h"


using namespace std;
class Server{
private:
	bool IsOnLine;
	DataBase* dataBase;	

public:
		
		Server();
		~Server();
		friend void ev_handler(struct mg_connection *c, int ev, void *p);
		void start();


};
#endif
