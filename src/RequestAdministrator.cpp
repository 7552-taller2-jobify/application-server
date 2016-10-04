#include "RequestAdministrator.h"

RequestAdministrator::RequestAdministrator() {}

RequestAdministrator::~RequestAdministrator() {}

static void handle_hola(struct mg_connection *nc){
/* Send headers */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    mg_printf_http_chunk(nc, "{ \"HOLA TALLER 2  \"}");
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

void RequestAdministrator::handle(struct mg_connection *c, int ev, void *p){
    struct http_message *hm = (struct http_message *) p;
    switch (ev) {
        case MG_EV_HTTP_REQUEST:
            if (mg_vcmp(&hm->uri, "/api/hola") == 0) {
                handle_hola(c); /* Handle hola call */
            } else if (mg_vcmp(&hm->uri, "/printcontent") == 0) {
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
