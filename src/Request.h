#ifndef REQUEST_H_
#define REQUEST_H_

#include <iostream>
#include <sstream>
#include "curl/curl.h"
#include "Response.h"

using namespace std;

class Request{
private:
		void* curl;
		struct curl_slist *slist1;
	

public:
		Request();
		Response* Execute(string url);
		Response* ExecutePost(string url, string body);
		void SetContentJson();
		void SetAuthorization(std::string authorization);
		~Request();
};
#endif // REQUEST_H_
