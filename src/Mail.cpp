// "Copyright 2016 <Jobify>"
#include <string.h>
#include "Mail.h"

Mail::Mail() {
}

Response*  Mail::Send() {
    Response* response = new Response();
    response->setContent("{ok}");
    return response;
}

Mail::~Mail() {
    curl = NULL;
    curl_easy_cleanup(curl);
}
