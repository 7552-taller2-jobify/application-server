#include "Server.h"

using namespace std;

static const char *s_http_port = "8000";

Server::Server(){
	this->IsOnLine = false;
	this->dataBase = new DataBase();
}

void ev_handler(struct mg_connection *c, int ev, void *p){
	
	RequestAdministrator* requestAdministrator = new RequestAdministrator();
	requestAdministrator->handle(c,ev,p);
}
	



void Server::start(){
	//DATABASE
	this->dataBase->open();
	this->dataBase->putHola();

	this->IsOnLine = true;
	
	struct mg_mgr mgr; //Mongoose event manager
	struct mg_connection *nc;//Callback function (event handler) prototype,
	
	mg_mgr_init(&mgr,this);//Initialize Mongoose manager
	//nc = mg_bind(&mgr, s_http_port, ev_handler);//Create listening connection.
	nc = mg_bind(&mgr, s_http_port, ev_handler);//Create listening connection.
	if(nc == NULL){
		this->IsOnLine = false;
		cout << "ERROR at Server->start()" <<endl;
		return;
	}
	mg_set_protocol_http_websocket(nc);
	
	cout<<"Starting server on port:"<< s_http_port <<endl;
	while(this->IsOnLine){
		mg_mgr_poll(&mgr,1000);
	}
	mg_mgr_free(&mgr);
}

Server::~Server(){
	
}
