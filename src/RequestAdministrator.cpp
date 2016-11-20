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
    // Send headers
    mg_printf(nc, "%s",
            "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    mg_printf_http_chunk(nc, "{ \"HOLA TALLER 2\"}\n");
    mg_send_http_chunk(nc, "", 0);  // Send empty chunk, the end of response
}

void RequestAdministrator::handle() {
    Message* msg = NULL;
    Response* response = NULL;
    if (this->ev == MG_EV_HTTP_REQUEST) {
        msg = this->rp->parseRequest(this->hm);
        std::cout << "uri: " << msg->uri << "\nbody: " << msg->body << "\nverb: " << msg->verb  << std::endl;

        Attendant* attendant = this->attendantHandler->find(msg->uri);
        if (attendant != NULL) {
            response = attendant->attend(*msg);

            if (response != NULL) {
                std::cout << "body out : " << response->getContent() << std::endl;
                std::cout << "status out : " << response->getStatus() << std::endl;

                mg_printf(c, "HTTP/1.0 %i\r\n"
                             "Content-Length: %d\r\n"
                             "Content-Type: application/json\r\n\r\n%s",
                             response->getStatus(), static_cast<int> (response->getContent().size()),
                                                        response->getContent().c_str());
            }
        }
    }
}


/*
void RequestAdministrator::handle(struct mg_connection *c, int ev, void *p){
	struct http_message *hm = (struct http_message *) p;

	switch (ev) {
    	case MG_EV_HTTP_REQUEST:
		    if (mg_vcmp(&hm->uri, "/api/hola") == 0) {
		        handle_hola(c); // Handle hola call 
		    }
		    else if (mg_vcmp(&hm->uri, "/printcontent") == 0) {
		        char buf[100] = {0};
		        memcpy(buf, hm->body.p,
		               sizeof(buf) - 1 < hm->body.len ? sizeof(buf) - 1 : hm->body.len);
		        printf("%s\n", buf);
		        mg_printf(c, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
				mg_send_http_chunk(c, "", 0); // Send empty chunk, the end of response 
		    }
		    break;
		default:
			break;
	}
}  
*/
