// "Copyright 2016 <Jobify>"

#include "RequestAdministrator.h"

RequestAdministrator::RequestAdministrator(struct mg_connection *c, int ev, struct http_message* p) {
    this->c = c;
    this->ev = ev;
    this->hm = p;
    this->rp = new RequestParse();
    this->attendantHandler = new AttendantsHandler();
}

RequestAdministrator::~RequestAdministrator() {}

static void handle_hola(struct mg_connection *nc) {
    mg_printf(nc, "%s",
            "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    mg_printf_http_chunk(nc, "{ \"HOLA TALLER 2\"}\n");
    mg_send_http_chunk(nc, "", 0);
}

void RequestAdministrator::handle() {
    Message* msg = NULL;
    Response* response = NULL;
    if (this->ev == MG_EV_HTTP_REQUEST) {
        msg = this->rp->parseRequest(this->hm);
        std::cout << "uri: " << msg->uri << "\nbody: " << msg->body << "\nverb: " << msg->verb  << "\n params: " << msg->params << std::endl;
        Attendant* attendant = this->attendantHandler->find(msg->uri);
        if (attendant != NULL) {
            response = attendant->attend(*msg);
        } else {
std::cout<<"METODO NO SOPORTADO"<<std::endl;
            Logger::getInstance().log(error, "Does not exist the request " + msg->verb + ".");
            response = new Response();
            response->setStatus(404);

        }
        if (response != NULL) {
            std::cout << "\nbody out : " << response->getContent() << std::endl;
            std::cout << "status out : " << response->getStatus() << std::endl;
            mg_printf(c, "HTTP/1.0 %i\r\n"
                         "Content-Length: %d\r\n"
                         "Content-Type: application/json\r\n\r\n%s",
                         response->getStatus(), static_cast<int> (response->getContent().size()),
                                                    response->getContent().c_str());
        }
    }
}
