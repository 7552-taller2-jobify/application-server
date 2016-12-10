// "Copyright 2016 <Jobify>"

#include "RequestParse.h"
#include <string>
#include <vector>

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
    return message;
}

RequestParse::~RequestParse() {}

std::string RequestParse::extractEmail(std::string uri) {
    return this->split(uri, "/")[1];
}

std::vector<std::string> RequestParse::split(std::string uri, std::string separator) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = uri.find(separator, prev);
        if (pos == std::string::npos) pos = uri.length();
        std::string token = uri.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + separator.length();
    } while (pos < uri.length() && prev < uri.length());
    return tokens;
}

std::map<std::string, std::string>* RequestParse::parserParameters(std::string params) {
    std::map<std::string, std::string>* parameters = new std::map<std::string, std::string>();
    std::vector<std::string> parameters_vector = split(params, "&");
    for (int i = 0; i < parameters_vector.size(); i++) {
        std::string a_parameter = parameters_vector[i];
        std::vector<std::string> key_value = split(a_parameter, "=");
        bool isValue = key_value.size() > 1;
        std::string value = "";
        if (isValue) {
            value =  URLDecode(key_value[1]);
        }
        parameters->insert(std::pair<std::string, std::string>(key_value[0], value));
    } 
    return parameters;
}

std::string RequestParse::URLDecode(std::string text) {
    text = curl_unescape(text.c_str(), 0);
    RequestParse * rp = new RequestParse();
    std::vector<std::string> words = rp->split(text, "+");
    delete rp;
    std::string text_decoding = "";

    for (int i=0; i< words.size(); i++) {
        if (i == 0) {
            text_decoding = words[i];
        } else {
            text_decoding = text_decoding + " " + words[i];
        }
    }
    return text_decoding;
}
