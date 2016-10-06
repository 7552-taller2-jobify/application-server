// "Copyright 2016 <Jobify>"

#ifndef ATTENDANT_H_
#define ATTENDANT_H_

#include <string>
#include <map>
#include <iostream> //TODO borrar
#include "Constants.h"
#include "Logger.h"

typedef void (*function)(struct Message operation);

class Attendant {
 protected:
    std::map<std::string, function> functions;
    bool isMethodSupported(std::string a_method);

 public:
    Attendant();
    virtual ~Attendant();
    void attend(struct Message operation);
    static void get(struct Message operation);
};


class Login: public Attendant {
 public:
    Login();
    ~Login();
    static void get(struct Message operation);
    static void post(struct Message operation);
};

class RecoveryPass: public Attendant {
 public:
    RecoveryPass();
    ~RecoveryPass();
    static void get(struct Message operation);
};

class Contact: public Attendant {
 public:
    Contact();
    ~Contact();
    static void get(struct Message operation);
};

class Accept: public Attendant {
 public:
    Accept();
    ~Accept();
    static void get(struct Message operation);
};

class Reject: public Attendant {
 public:
    Reject();
    ~Reject();
    static void get(struct Message operation);
};

class ProfilePersonal: public Attendant {
 public:
    ProfilePersonal();
    ~ProfilePersonal();
    static void get(struct Message operation);
};

class ProfileSummary: public Attendant {
 public:
    ProfileSummary();
    ~ProfileSummary();
    static void get(struct Message operation);
};

class ProfileExpertise: public Attendant {
 public:
    ProfileExpertise();
    ~ProfileExpertise();
    static void get(struct Message operation);
};

class ProfileSkills: public Attendant {
 public:
    ProfileSkills();
    ~ProfileSkills();
    static void get(struct Message operation);
};

class ProfilePhoto: public Attendant {
 public:
    ProfilePhoto();
    ~ProfilePhoto();
    static void get(struct Message operation);
};

#endif  // ATTENDANT_H_
