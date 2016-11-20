// "Copyright 2016 <Jobify>"
#ifndef MAIL_H_
#define MAIL_H_
#include <iostream>
#include <sstream>
#include "Response.h"
#include "curl/curl.h"

class Mail{
 private:
    void* curl;

 public:
    Mail();
    Response* Send();
    ~Mail();
};
#endif  // MAIL_H_

