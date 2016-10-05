// "Copyright 2016 <Jobify>"

#ifndef ATTENDANT_H_
#define ATTENDANT_H_

#include <string>
#include <vector>

class Attendant {
 protected:
    std::vector<std::string> *methods;
    bool isMethodSupported(std::string a_method);

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
    RecoveryPass();
    ~RecoveryPass();
    void attend(std::string operation);
};

class Contact: public Attendant {
 public:
    Contact();
    ~Contact();
    void attend(std::string operation);
};

class Accept: public Attendant {
 public:
    Accept();
    ~Accept();
    void attend(std::string operation);
};

class Reject: public Attendant {
 public:
    Reject();
    ~Reject();
    void attend(std::string operation);
};

class ProfilePersonal: public Attendant {
 public:
    ProfilePersonal();
    ~ProfilePersonal();
    void attend(std::string operation);
};

class ProfileSummary: public Attendant {
 public:
    ProfileSummary();
    ~ProfileSummary();
    void attend(std::string operation);
};

class ProfileExpertise: public Attendant {
 public:
    ProfileExpertise();
    ~ProfileExpertise();
    void attend(std::string operation);
};

class ProfileSkills: public Attendant {
 public:
    ProfileSkills();
    ~ProfileSkills();
    void attend(std::string operation);
};

class ProfilePhoto: public Attendant {
 public:
    ProfilePhoto();
    ~ProfilePhoto();
    void attend(std::string operation);
};

#endif  // ATTENDANT_H_
