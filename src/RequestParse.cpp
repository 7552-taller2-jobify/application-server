// "Copyright 2016 <Jobify>"

#include "RequestParse.h"

RequestParse::RequestParse() {}

Message* RequestParse::parseRequest(struct http_message* hm) {
    std::string uri;
    for (size_t i = 0; i < hm->uri.len; i++) {
        uri = uri + hm->uri.p[i];
    }

    Logger::getInstance().log(info, "The URI arrived was: " + uri);

    std::string body = "";
    for (size_t i = 0; i < hm->body.len; i++) {
        body = body + hm->body.p[i];
    }

    std::string verb = "";
    for (size_t i = 0; i < hm->method.len; i++) {
        verb = verb + hm->method.p[i];
    }

    Message* message = new Message;
    message->uri = uri;
    message->body = body;
    message->verb = verb;
    message->params = "";

    for (size_t i = 0; i < hm->query_string.len; i++) {
        message->params += hm->query_string.p[i];
    }
    // message->params = hm->query_string;
    std::cout<<message->params<<std::endl;

    return message;
}

RequestParse::~RequestParse() {}
