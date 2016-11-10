// "Copyright 2016 <Jobify>"

#ifndef USER_SERVICE_H_
#define USER_SERVICE_H_

#include <string>
#include "Profile.h"

class UserService {
 public:
    UserService();
    ~UserService();
    std::string loginInformation(Personal *personal, Picture *picture, std::string token);
};

#endif  // USER_SERVICE_H_
