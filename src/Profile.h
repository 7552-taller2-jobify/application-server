#ifndef PROFILE_H_
#define PROFILE_H_

#include <string>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "Logger.h"

class Profile {
 protected:
    virtual std::string createJsonFile() = 0;
    virtual void getOwnInfo(rapidjson::Document &document) = 0;

 public:
    Profile();
    ~Profile() {}
    virtual void getProfileInfo(std::string json_file);
    void updateJson(std::string json_file);
};

class Personal: public Profile {
 private:
    std::string first_name;
    std::string last_name;
    std::string email;
    std::string birthday;
    std::string address[2];
    void getOwnInfo(rapidjson::Document &document);

 public:
    Personal() : Profile() {}
    ~Personal() {}
    std::string getFirstName();
    std::string getLastName();
    std::string getEmail();
    std::string getBirthday();
    std::string* getAddress();
    void setFirstName(std::string new_first_name);
    void setLastName(std::string new_last_name);
    void setEmail(std::string new_email);
    void setBirthday(std::string new_birthday);
    void setAddress(std::string new_lat, std::string new_lon);
    std::string createJsonFile();
};

class Summary: public Profile {
 private:
    std::string summary;
    void getOwnInfo(rapidjson::Document &document);

 public:
    Summary() : Profile() {}
    ~Summary() {}
    std::string getSummary();
    void setSummary(std::string new_summary);
    std::string createJsonFile();
};

class Expertise: public Profile {
 private:
    std::string company;
    std::string position;
    std::string from;
    std::string to;
    std::string expertise;
    void getOwnInfo(rapidjson::Document &document);

 public:
    Expertise() : Profile() {}
    ~Expertise() {}
    std::string getCompany();
    std::string getPosition();
    std::string getFrom();
    std::string getTo();
    std::string getExpertise();
    void setCompany(std::string new_company);
    void setPosition(std::string new_position);
    void setFrom(std::string new_from);
    void setTo(std::string new_to);
    void setExpertise(std::string new_expertise);
    std::string createJsonFile();
};

class Skills: public Profile {
 private:
    std::string skills;
    std::string parseSkills();
    void getOwnInfo(rapidjson::Document &document);

 public:
    Skills() : Profile() {}
    ~Skills() {}
    std::string getSkills();
    void setSkills(std::string new_skills);
    std::string createJsonFile();
};

class Picture: public Profile {
 private:
    std::string picture;
	void getOwnInfo(rapidjson::Document &document);

 public:
    Picture() : Profile() {}
    ~Picture() {}
    std::string getPicture();
    void setPicture(std::string new_picture);
    std::string createJsonFile();
};

#endif
