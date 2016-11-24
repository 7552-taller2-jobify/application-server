// "Copyright 2016 <Jobify>"

#ifndef ATTENDANT_H_
#define ATTENDANT_H_

#include <map>
#include <string>
#include <curl/curl.h>
#include <iostream>  // TODO borrar
#include "Constants.h"
#include "Logger.h"
#include "Profile.h"
#include "Response.h"
#include "DataBaseAdministrator.h"
#include "DataBase.h"
#include "Request.h"
#include "Mail.h"
#include "FirebaseService.h"
#include "SharedService.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "RequestParse.h"
#include "Authentication.h"
#include <vector>

typedef Response* (*function) (Message operation);

class Attendant {
 private:
    bool isMethodSupported(std::string a_method);
 protected:
    std::map<std::string, function> functions;
 public:
    Attendant();
    virtual ~Attendant();
    Response* attend(Message operation);
    static Response* get(Message operation);
};


class Login: public Attendant {
 public:
    Login();
    ~Login();
    static Response* get(Message operation);
    static Response* post(Message operation);
};

class Logout: public Attendant {
 public:
    Logout();
    ~Logout();
    static Response* get(Message operation);
    static Response* erase(Message operation);
};

class Register: public Attendant {
 public:
    Register();
    ~Register();
    static Response* get(Message operation);
    static Response* post(Message operation);
};

class RecoveryPass: public Attendant {
 public:
    RecoveryPass();
    ~RecoveryPass();
    static Response* get(Message operation);
};

class Contact: public Attendant {
 public:
    Contact();
    ~Contact();
    static Response* post(Message operation);
    static Response* get(Message operation);
};

class Accept: public Attendant {
 public:
    Accept();
    ~Accept();
    static Response* post(Message operation);
};

class Reject: public Attendant {
 public:
    Reject();
    ~Reject();
    static Response* erase(Message operation);
};

class ProfilePersonal: public Attendant {
 public:
    ProfilePersonal();
    ~ProfilePersonal();
    static Response* get(Message operation);
    static Response* put(Message operation);
};

class ProfileSummary: public Attendant {
 public:
    ProfileSummary();
    ~ProfileSummary();
    static Response* get(Message operation);
    static Response* put(Message operation);
};

class ProfileExpertise: public Attendant {
 public:
    ProfileExpertise();
    ~ProfileExpertise();
    static Response* put(Message operation);
    static Response* get(Message operation);
};

class ProfileSkills: public Attendant {
 public:
    ProfileSkills();
    ~ProfileSkills();
    static Response* post(Message operation);
    static Response* put(Message operation);
    static Response* get(Message operation);
};

class ProfilePhoto: public Attendant {
 public:
    ProfilePhoto();
    ~ProfilePhoto();
    static Response* get(Message operation);
    static Response* put(Message operation);
};

class ProfileFriends: public Attendant {
 public:
    ProfileFriends();
    ~ProfileFriends();
    static Response* get(Message operation);
};

class Vote: public Attendant {
 public:
    Vote();
    ~Vote();
    static Response* post(Message operation);
    static Response* erase(Message operation);
};

class ProfileOwnRecommendations: public Attendant {
 public:
    ProfileOwnRecommendations();
    ~ProfileOwnRecommendations();
    static Response* get(Message operation);
};

class ProfileOthersRecommendations: public Attendant {
 public:
    ProfileOthersRecommendations();
    ~ProfileOthersRecommendations();
    static Response* get(Message operation);
};

class MostPopularUsers: public Attendant {
 public:
    MostPopularUsers();
    ~MostPopularUsers();
    static Response* get(Message operation);
};

class Facebook: public Attendant {
 public:
    Facebook();
    ~Facebook();
    static Response* get(struct Message operation);
    static Response* post(struct Message operation);
};
class Firebase: public Attendant {
 public:
    Firebase();
    ~Firebase();
    static Response* post(struct Message operation);
};
class Category: public Attendant {
 public:
    Category();
    ~Category();
    static Response* post(struct Message operation);
    static Response* get(struct Message operation);
};
class Skill: public Attendant {
 public:
    Skill();
    ~Skill();
    static Response* post(struct Message operation);
    static Response* get(struct Message operation);
};

class JobPosition: public Attendant {
 public:
    JobPosition();
    ~JobPosition();
    static Response* post(struct Message operation);
    static Response* get(struct Message operation);
};

class Search: public Attendant {
 private:
    static void loadParameters(std::string params, std::string *token, double *lat, double *lon, double *distance, std::string *position, std::vector<std::string> *skills);
    static std::string URLDecode(std::string text);
    static std::vector<std::string>* searchByPosition(std::vector<std::string>* ids, std::string position);
    static std::vector<std::string>* searchBySkills(std::vector<std::string>* ids, std::vector<std::string>* skills);
    static std::vector<std::string>* intersection(std::vector<std::string>* ids_match_position, std::vector<std::string>* ids_match_skills);
 public:
    Search();
    ~Search();
    static Response* get(Message operation);
};

#endif  // ATTENDANT_H_
