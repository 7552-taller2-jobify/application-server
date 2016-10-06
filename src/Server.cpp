// "Copyright 2016 <Jobify>"

#include "Server.h"
#include <string>

static const char s_http_port[] = "8000";

Server::Server() {
    this->IsOnLine = false;
    this->db = new DataBase(PATH_DB);
    this->url_mapper = new URLMapper();
    this->attendants_handler = new AttendantsHandler();
}

void ev_handler(struct mg_connection *c, int ev, void *p) {
    RequestAdministrator* requestAdministrator = new RequestAdministrator();
    requestAdministrator->handle(c, ev, p);
}

void Server::start() {
    Request* request = new Request();
// Poner IP de LAN de su compu
    string response = request->ExecutePost("http://192.168.182.136:5000/categories","{ 'name': 'sport','description': 'Categoría relacionada con los deportes'}");
    std::cout<< response << std::endl;
    string responseTest = request->Execute("http://192.168.182.136:5000/job_positions/test");
	std::cout<< responseTest << std::endl;
	string responseGet = request->Execute("http://192.168.182.136:5000/categories");
	std::cout<< responseGet << std::endl;
    CURL *curl = curl_easy_init();
    this->IsOnLine = true;
    struct mg_mgr mgr;  // Mongoose event manager
    struct mg_connection *nc;  // Callback function (event handler) prototype
    mg_mgr_init(&mgr, this);  // Initialize Mongoose manager
    // nc = mg_bind(&mgr, s_http_port, ev_handler);
    // Create listening connection
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
    delete this->db;
    delete this->url_mapper;
    delete this->attendants_handler;
}

void Server::resolveRequest(std::string request) {
    std::string class_id = this->url_mapper->find(request);
    Attendant *attendant = this->attendants_handler->find(class_id);
    attendant->attend(request);
    /*TODO en lugar de string request, podría pasarse un struct con todos
    los datos necesarios.
    Habría que ver bien cómo resolver el llamado a la función*/
}
