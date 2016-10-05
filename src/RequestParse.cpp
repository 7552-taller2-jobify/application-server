// "Copyright 2016 <Jobify>"

#include "RequestParse.h"

RequestParse::RequestParse() {
    // TODO Auto-generated constructor stub
}

Message* RequestParse::parseRequest(struct http_message* hm) {
    string uri;
    for (size_t i = 0; i < hm->uri.len; i++) {
        uri = uri + hm->uri.p[i];
    }

    Logger::getInstance().log(info, "The URI arrived was: " + uri);

    cout << "URI: " << uri << endl;

    string type = "";
    string who_send = "";

    istringstream issUri(uri);
    string word = "";

    for (unsigned int i = 0; getline(issUri, word, '/') ; i++) {
        if (i==1){
            type = this->getHTTPVerb(hm) + word;
        } else if (i==2) {
            who_send = word;
        }
    }

    Message* message = new Message;
    message->who_send = who_send;
    message->type = type;

    return message;

}

string RequestParse::getHTTPVerb(struct http_message* hm) {

    if (this->areEqual(&hm->method, &PUT_FUNCTION)){
        Logger::getInstance().log(info, "HTTP verb: PUT");
    	return "PUT";

    } else if (this->areEqual(&hm->method, &GET_FUNCTION)) {
        Logger::getInstance().log(info, "HTTP verb: GET");
        return "GET";

    } else if (this->areEqual(&hm->method, &POST_FUNCTION)) {
        Logger::getInstance().log(info, "HTTP verb: POST");
        return "POST";

    } else if (this->areEqual(&hm->method, &DELETE_FUNCTION)) {
        Logger::getInstance().log(info, "HTTP verb: DELETE");
        return "DELETE";

    } else {
        Logger::getInstance().log(error, "The HTTP verb not exists");
        return "ERROR";

    }

}

int RequestParse::areEqual(const struct mg_str *s1, const struct mg_str *s2) {
    return s1->len == s2->len && memcmp(s1->p, s2->p, s2->len) == 0;
}


RequestParse::~RequestParse() {
    // TODO Auto-generated destructor stub
}
