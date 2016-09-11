#include "Server.h"
#include "leveldb/db.h"

using namespace std;

static const char *s_http_port = "8000";

Server::Server(){
	this->IsOnLine = false;
	this->db = new DataBase(PATH_DB);
}
static void handle_hola(struct mg_connection *nc){

/* Send headers */
  mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

  mg_printf_http_chunk(nc, "{ \"HOLA TALLER 2  \"}");
  mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

void ev_handler(struct mg_connection *c,int ev, void *ev_data){
	struct http_message *hm = (struct http_message *) ev_data;

	  switch (ev) {
	    case MG_EV_HTTP_REQUEST:
	      if (mg_vcmp(&hm->uri, "/api/hola") == 0) {
	        handle_hola(c); /* Handle hola call */
	      }
	      else if (mg_vcmp(&hm->uri, "/printcontent") == 0) {
	        char buf[100] = {0};
	        memcpy(buf, hm->body.p,
	               sizeof(buf) - 1 < hm->body.len ? sizeof(buf) - 1 : hm->body.len);
	        printf("%s\n", buf);
	        mg_printf(c, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
			mg_send_http_chunk(c, "", 0); /* Send empty chunk, the end of response */
	      }
	      break;
	    default:
	      break;
	  }
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
	struct mg_mgr mgr;
	struct mg_connection *nc;
	
	mg_mgr_init(&mgr,this);
	nc = mg_bind(&mgr, s_http_port, ev_handler);

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
