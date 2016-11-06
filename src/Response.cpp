#include "Response.h"
#include <string>

Response::Response() {}

std::string Response::getContent() {
	return content;
}

long Response::getStatus() {
	return http_code;
}

void Response::setContent(std::string str) {
	content = str;
}

void Response::setStatus(long code) {
	http_code = code;
}

Response::~Response() {}
