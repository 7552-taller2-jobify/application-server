// "Copyright 2016 <Jobify>"

#include "Server.h"
#include "Response.h"
#include <string>

static const char s_http_port[] = "8000";

Server::Server() {
    this->IsOnLine = false;
    this->url_mapper = new URLMapper();
    this->attendants_handler = new AttendantsHandler();
}


void ev_handler(struct mg_connection *c, int ev, void *p) {
    RequestAdministrator* requestAdministrator = new RequestAdministrator(c, ev, (struct http_message *) p);
    requestAdministrator->handle();
}

void Server::start() {
    this->IsOnLine = true;
    struct mg_mgr mgr;
    struct mg_connection *nc;
    mg_mgr_init(&mgr, this);
    nc = mg_bind(&mgr, s_http_port, ev_handler);
    if (nc == NULL) {
        this->IsOnLine = false;
        std::cout << "ERROR at Server->start()" << std::endl;
        return;
    }
    mg_set_protocol_http_websocket(nc);
    std::cout << "Starting server on port:" << s_http_port << std::endl;
    while (this->IsOnLine) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
}

Server::~Server() {
    delete this->url_mapper;
    delete this->attendants_handler;
}

void Server::resolveRequest(std::string request) {
    std::string class_id = this->url_mapper->find(request);
}
