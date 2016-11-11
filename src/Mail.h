#ifndef MAIL_H_
#define MAIL_H_
#include "curl/curl.h"
#include <iostream>
#include <sstream>
#include "Response.h"

using namespace std;

class Mail{
private:
		void* curl;

public:
		Mail();
		Response* Send();
		~Mail();
};
#endif

