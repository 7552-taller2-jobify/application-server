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

std::string RequestParse::extractEmail(std::string uri) {
    return this->split(uri, "/")[1];
}

std::vector<std::string> RequestParse::split(std::string uri, std::string separator) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = uri.find(separator, prev);
        if (pos == std::string::npos) pos = uri.length();
        std::string token = uri.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + separator.length();
    }
    while (pos < uri.length() && prev < uri.length());
    return tokens;
}
