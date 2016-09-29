#ifndef ATTENDANTSHANDLER_H_
#define ATTENDANTSHANDLER_H_

#include <string>
#include <map>
#include "Attendant.h"

using namespace std;

class AttendantsHandler {
private:
	map<string, Attendant*> attendants;

public:
	AttendantsHandler();
	~AttendantsHandler();
	Attendant* find(string key_to_match);
};

#endif
