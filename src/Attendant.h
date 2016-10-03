#ifndef ATTENDANT_H_
#define ATTENDANT_H_

#include <string>

using namespace std;

class Attendant {
public:
	Attendant();
	~Attendant();
	virtual void attend(string operation);
};


class Login: public Attendant {
public:
	Login();
	~Login();
	void attend(string operation);
};

class RecoveryPass: public Attendant {
public:
	void attend(string operation);
};

class Contact: public Attendant {
public:
	void attend(string operation);
};

class Accept: public Attendant {
public:
	void attend(string operation);
};

class Reject: public Attendant {
public:
	void attend(string operation);
};

class ProfilePersonal: public Attendant {
public:
	void attend(string operation);
};

class ProfileSummary: public Attendant {
public:
	void attend(string operation);
};

class ProfileExpertise: public Attendant {
public:
	void attend(string operation);
};

class ProfileSkills: public Attendant {
public:
	void attend(string operation);
};

class ProfilePhoto: public Attendant {
public:
	void attend(string operation);
};

#endif
