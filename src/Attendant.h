// "Copyright 2016 <Jobify>"

#ifndef ATTENDANT_H_
#define ATTENDANT_H_

#include <string>

class Attendant {
 public:
    Attendant();
    ~Attendant();
    virtual void attend(std::string operation);
};


class Login: public Attendant {
 public:
    Login();
    ~Login();
    void attend(std::string operation);
};

class RecoveryPass: public Attendant {
 public:
    void attend(std::string operation);
};

class Contact: public Attendant {
 public:
    void attend(std::string operation);
};

class Accept: public Attendant {
 public:
    void attend(std::string operation);
};

class Reject: public Attendant {
 public:
    void attend(std::string operation);
};

class ProfilePersonal: public Attendant {
 public:
    void attend(std::string operation);
};

class ProfileSummary: public Attendant {
 public:
    void attend(std::string operation);
};

class ProfileExpertise: public Attendant {
 public:
    void attend(std::string operation);
};

class ProfileSkills: public Attendant {
 public:
    void attend(std::string operation);
};

class ProfilePhoto: public Attendant {
 public:
    void attend(std::string operation);
};

#endif  // ATTENDANT_H_
