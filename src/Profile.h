// "Copyright 2016 <Jobify>"

#ifndef PROFILE_H_
#define PROFILE_H_

#include <string>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <vector>
#include <functional>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "Logger.h"

class Profile {
 protected:
    virtual std::string createJsonFile() = 0;
    virtual void getOwnInfo(const rapidjson::Document &document) = 0;
    std::string getItemByIndex(std::string *items, int index);
    void setItem(std::string *items, int index, std::string new_item);

 public:
    Profile();
    ~Profile() {}
    virtual void getProfileInfo(std::string json_file);
    void updateJson(const std::string json_file);
    void loadJson(const std::string json);
};

class LoginInformation: public Profile {
 private:
    std::string email;
    std::string password;
    void getOwnInfo(const rapidjson::Document &document);

 public:
    LoginInformation() : Profile() {}
    ~LoginInformation() {}
    std::string getEmail();
    std::string getPassword();
    void setEmail(std::string new_email);
    void setPassword(std::string new_password);
    std::string createJsonFile();
};

class Personal: public Profile {
 private:
    double device_id;
    std::string first_name;
    std::string last_name;
    std::string email;
    std::string gender;
    std::string birthday;
    std::string address[2];
    std::string city;
    void getOwnInfo(const rapidjson::Document &document);

 public:
    Personal() : Profile() {}
    ~Personal() {}
    double getDeviceId();
    std::string getFirstName();
    std::string getLastName();
    std::string getEmail();
    std::string getGender();
    std::string getBirthday();
    std::string* getAddress();
    std::string getCity();
    void setDeviceId(double new_id);
    void setFirstName(std::string new_first_name);
    void setLastName(std::string new_last_name);
    void setEmail(std::string new_email);
    void setGender(std::string new_gender);
    void setBirthday(std::string new_birthday);
    void setAddress(std::string new_lat, std::string new_lon);
    void setCity(std::string new_city);
    std::string createJsonFile();
};

class Summary: public Profile {
 private:
    std::string summary;
    void getOwnInfo(const rapidjson::Document &document);

 public:
    Summary() : Profile() {}
    ~Summary() {}
    std::string getSummary();
    void setSummary(std::string new_summary);
    std::string createJsonFile();
};

class Expertise: public Profile {
 private:
    std::string *company;
    std::string *position;
    std::string *from;
    std::string *to;
    std::string *expertise;
    std::string *category;
    int number_of_expertises;

    void getOwnInfo(const rapidjson::Document &document);

 public:
    Expertise() : Profile() {}
    ~Expertise();
    std::string getCompany(int index);
    std::string getPosition(int index);
    std::string getFrom(int index);
    std::string getTo(int index);
    std::string getExpertise(int index);
    std::string getCategory(int index);
    void setCompany(std::string new_company, int index);
    void setPosition(std::string new_position, int index);
    void setFrom(std::string new_from, int index);
    void setTo(std::string new_to, int index);
    void setExpertise(std::string new_expertise, int index);
    void setCategory(std::string new_category, int index);
    std::string createJsonFile();
};

class Skills: public Profile {
 private:
    std::string *skills;
    std::string *category;
    std::string parseSkills(int index);
    int number_of_skills;
    void getOwnInfo(const rapidjson::Document &document);

 public:
    Skills() : Profile() {}
    ~Skills() {}
    std::string getSkills(int index);
    std::string getCategory(int index);
    void setSkills(std::string new_skills, int index);
    void setCategory(std::string new_skills, int index);
    std::string createJsonFile();
};

class Picture: public Profile {
 private:
    std::string picture;
    void getOwnInfo(const rapidjson::Document &document);

 public:
    Picture() : Profile() {}
    ~Picture() {}
    std::string getPicture();
    void setPicture(std::string new_picture);
    std::string createJsonFile();
};

class Contacts: public Profile {
 protected:
    std::string number_name;
    std::string contacts_name;
    std::vector<std::string> contacts;
    int search(std::string contact);
    void getOwnInfo(const rapidjson::Document &document);
    std::string createJsonFile();
    int getNumberOfContacts();
    void addContact(std::string contact_to_add);
    void removeContact(std::string contact_to_remove);
    void defineFields(const rapidjson::Document &document, std::string number_name, std::string contacts_name);

 public:
    Contacts() : Profile() {}
    virtual ~Contacts() = 0;
};

class Friends: public Contacts {
 public:
    Friends() : Contacts() {
        this->number_name = "number_of_friends";
        this->contacts_name = "friends";
    }
    ~Friends() {}
};

class OwnRecommendations: public Contacts {
 public:
    OwnRecommendations() : Contacts() {
        this->number_name = "number_of_own_recommendations";
        this->contacts_name = "own_recommendations";
    }
    ~OwnRecommendations() {}
};

class OthersRecommendations: public Contacts {
 public:
    OthersRecommendations() : Contacts() {
        this->number_name = "number_of_others_recommendations";
        this->contacts_name = "others_recommendations";
    }
    ~OthersRecommendations() {}
};

#endif  // PROFILE_H_
