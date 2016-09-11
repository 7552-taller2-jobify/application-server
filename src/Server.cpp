#include "Server.h"

using namespace std;

static const char *s_http_port = "8000";

Server::Server(){
	this->IsOnLine = false;
	this->db = new DataBase(PATH_DB);
}

void ev_handler(struct mg_connection *c, int ev, void *p){
	
	RequestAdministrator* requestAdministrator = new RequestAdministrator();
	requestAdministrator->handle(c,ev,p);
}

void Server::start(){
///PRUEBA LEVELDB
	 /*leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, "./db", &db);

    if (false == status.ok())
    {
        cerr << "Unable to open/create test database './db'" << endl;
        cerr << status.ToString() << endl;
	
	}
	leveldb::WriteOptions writeOptions;
	db->Put(writeOptions, "1", "HOLA");*/

	//this->db->put("2", "chau");


///
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
