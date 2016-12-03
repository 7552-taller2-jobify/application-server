// "Copyright 2016 <Jobify>"

#ifndef ATTENDANT_H_
#define ATTENDANT_H_

#include <curl/curl.h>
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>  /* atof */
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

const char INVALID_CREDENTIALS[] = "0";
const char CLIENT_ALREADY_EXISTS[] = "1";
const char EMPTY_FIELDS[] = "2";
const char COULD_NOT_POST[] = "3";
const char NO_SOLICITUDE_SENT[] = "4";
const char COULD_NOT_PUT[] = "5";
const char CLIENT_NOT_EXISTS[] = "6";
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
 private:
    static Response* putAndPost(Message operation, int status_ok, std::string error_code);

 public:
    ProfilePersonal();
    ~ProfilePersonal();
    static Response* get(Message operation);
    static Response* put(Message operation);
    static Response* post(Message operation);
};

class ProfileSummary: public Attendant {
 private:
    static Response* putAndPost(Message operation, int status_ok, std::string error_code);

 public:
    ProfileSummary();
    ~ProfileSummary();
    static Response* get(Message operation);
    static Response* put(Message operation);
    static Response* erase(Message operation);
    static Response* post(Message operation);
};

class ProfileExpertise: public Attendant {
 private:
    static Response* putAndPost(Message operation, int status_ok, std::string error_code);

 public:
    ProfileExpertise();
    ~ProfileExpertise();
    static Response* post(Message operation);
    static Response* put(Message operation);
    static Response* get(Message operation);
    static Response* erase(Message operation);
};

class ProfileSkills: public Attendant {
 private:
    static Response* putAndPost(Message operation, int status_ok, std::string error_code);

 public:
    ProfileSkills();
    ~ProfileSkills();
    static Response* post(Message operation);
    static Response* put(Message operation);
    static Response* get(Message operation);
    static Response* erase(Message operation);
};

class ProfilePhoto: public Attendant {
 private:
    static Response* putAndPost(Message operation, int status_ok, std::string error_code);

 public:
    ProfilePhoto();
    ~ProfilePhoto();
    static Response* get(Message operation);
    static Response* put(Message operation);
    static Response* erase(Message operation);
    static Response* post(Message operation);
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
    static void loadParameters(std::string params, std::string *token, std::string *lat, std::string *lon,
                               std::string *distance, std::string *position, std::string *limit_str, 
                               std::string *offset_str, std::vector<std::string> *skills);
    static std::string URLDecode(std::string text);
    static std::vector<std::string>* searchByPosition(std::vector<std::string>* ids, std::string position);
    static std::vector<std::string>* searchBySkills(std::vector<std::string>* ids, std::vector<std::string>* skills);
    static std::map<std::string, std::string>* searchByDistance(std::vector<std::string>* ids, double lat, double lon,
                                                      double distance);
    static std::vector<std::string>* intersection(std::vector<std::string>* ids_match_position,
                                                  std::vector<std::string>* ids_match_skills);
    static double calculateDistance(double lat1, double long1, double lat2, double long2);
    static bool rightLong(double lon);
    static bool rightLat(double lon);
    static double toRad(double degree);
    static std::vector<std::string>* getKeys(std::map<std::string, std::string>* m);
    static std::string generateMessage(std::vector<std::string>* ids_match, std::map<std::string, 
                                       std::string> *ids_match_distance_map, int offset, int limit);    
 public:
    Search();
    ~Search();
    static Response* get(Message operation);
};

#endif  // ATTENDANT_H_
