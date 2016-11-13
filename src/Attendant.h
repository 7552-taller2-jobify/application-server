// "Copyright 2016 <Jobify>"

#ifndef ATTENDANT_H_
#define ATTENDANT_H_

#include <map>
#include <iostream> //TODO borrar
#include "Constants.h"
#include "Logger.h"
#include "Profile.h"
#include "Response.h"
#include "DataBaseAdministrator.h"
#include "DataBase.h"
#include "RequestParse.h"
#include "Authentication.h"

typedef Response* (*function)(struct Message operation);

class Attendant {
 protected:
    std::map<std::string, function> functions;
    bool isMethodSupported(std::string a_method);
 public:
    Attendant();
    virtual ~Attendant();
    Response* attend(struct Message operation);
    static Response* get(struct Message operation);
};


class Login: public Attendant {
 public:
    Login();
    ~Login();
    static Response* get(struct Message operation);
    static Response* post(struct Message operation);
};

class Logout: public Attendant {
 public:
    Logout();
    ~Logout();
    static Response* get(struct Message operation);
    static Response* put(struct Message operation);
};

class Register: public Attendant {
 public:
    Register();
    ~Register();
    static Response* get(struct Message operation);
    static Response* post(struct Message operation);
};

class RecoveryPass: public Attendant {
 public:
    RecoveryPass();
    ~RecoveryPass();
    static Response* get(struct Message operation);
};

class Contact: public Attendant {
 public:
    Contact();
    ~Contact();
    static Response* get(struct Message operation);
};

class Accept: public Attendant {
 public:
    Accept();
    ~Accept();
    static Response* get(struct Message operation);
};

class Reject: public Attendant {
 public:
    Reject();
    ~Reject();
    static Response* get(struct Message operation);
};

class ProfilePersonal: public Attendant {
 public:
    ProfilePersonal();
    ~ProfilePersonal();
    static Response* get(struct Message operation);
    static Response* put(struct Message operation);
};

class ProfileSummary: public Attendant {
 public:
    ProfileSummary();
    ~ProfileSummary();
    static Response* get(struct Message operation);
    static Response* put(struct Message operation);
};

class ProfileExpertise: public Attendant {
 public:
    ProfileExpertise();
    ~ProfileExpertise();
    static Response* get(struct Message operation);
};

class ProfileSkills: public Attendant {
 public:
    ProfileSkills();
    ~ProfileSkills();
    static Response* post(struct Message operation);
    static Response* put(struct Message operation);
    static Response* get(struct Message operation);
};

class ProfilePhoto: public Attendant {
 public:
    ProfilePhoto();
    ~ProfilePhoto();
    static Response* get(struct Message operation);
};

#endif  // ATTENDANT_H_
