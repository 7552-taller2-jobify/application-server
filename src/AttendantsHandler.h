// "Copyright 2016 <Jobify>"

#ifndef ATTENDANTSHANDLER_H_
#define ATTENDANTSHANDLER_H_

#include <string>
#include <map>
#include "Attendant.h"

class AttendantsHandler {
 private:
    std::map<std::string, Attendant*> attendants;

 public:
    AttendantsHandler();
    ~AttendantsHandler();
    Attendant* find(std::string key_to_match);
};

#endif  // ATTENDANTSHANDLER_H_
