#ifndef REQUEST_H_
#define REQUEST_H_
#include "curl/curl.h"
#include <iostream>
#include <sstream>
#include "Response.h"

using namespace std;

class Request{
private:
		void* curl;
	

public:
		Request();
		std::string Execute(string url);
		Response* ExecutePost(string url, string body);
		~Request();
};
#endif
