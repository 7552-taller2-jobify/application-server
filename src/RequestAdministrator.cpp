// "Copyright 2016 <Jobify>"

#include "RequestAdministrator.h"

RequestAdministrator::RequestAdministrator(struct mg_connection *c, int ev, struct http_message* p){
    this->c = c;
    this->ev = ev;
    this->hm = p;
    this->rp = new RequestParse();
}

RequestAdministrator::~RequestAdministrator() {}

static void handle_hola(struct mg_connection *nc) {
    // Send headers
    mg_printf(nc, "%s",
            "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    mg_printf_http_chunk(nc, "{ \"HOLA TALLER 2  \"}");
    mg_send_http_chunk(nc, "", 0);  // Send empty chunk, the end of response
}

void RequestAdministrator::handle(){
    Message* msg = NULL;
    switch (this->ev) {
        case MG_EV_HTTP_REQUEST:

            msg = this->rp->parseRequest(this->hm);
            cout << " quien: " << msg->who_send << " type: " << msg->type << endl;
            // CHEQUEAR QUE OPERACION REALIZAR MEDIANTE MSG

            if (mg_vcmp(&this->hm->uri, "/api/hola") == 0) {
                handle_hola(this->c); /* Handle hola call */
            }
            else if (mg_vcmp(&this->hm->uri, "/printcontent") == 0) {
                char buf[100] = {0};
                memcpy(buf, this->hm->body.p,
                       sizeof(buf) - 1 < this->hm->body.len ? sizeof(buf) - 1 : this->hm->body.len);
                printf("%s\n", buf);
                mg_printf(this->c, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
                mg_send_http_chunk(this->c, "", 0); /* Send empty chunk, the end of response */
            }
            //delete msg;
            break;
        default:
            break;
    }
}
