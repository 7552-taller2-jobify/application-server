// "Copyright 2016 <Jobify>"
#ifndef FIREBASESERVICE_H_
#define FIREBASESERVICE_H_
#include <string>
#include "Request.h"
#include "Response.h"

class FirebaseService{
 private:
 public:
    FirebaseService();
    Response* SendNotification(std::string toToken, std::string body);
    ~FirebaseService();
};
#endif  // FIREBASESERVICE_H_
