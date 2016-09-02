#ifndef SERVER_H_
#define SERVER_H_
#include <iostream>
#include "mongoose.h"


using namespace std;
class Server{
private:
	bool IsOnLine;
public:
		friend void ev_handler(struct mg_connection *c, int ev, void *p);
		Server();
		~Server();
		void start();


};
#endif
