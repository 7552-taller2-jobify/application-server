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

using namespace rapidjson;
using namespace std;

class Profile {
protected:
	virtual string createJsonFile() = 0;
	virtual void getOwnInfo(Document &document) = 0;

public:
	Profile();
	~Profile() {};
	virtual void getProfileInfo(string json_file);
	void updateJson(string json_file);
};

class Personal: public Profile {
private:
	string first_name;
	string last_name;
	string email;
	string birthday;
	string address[2];

	void getOwnInfo(Document &document);

public:
	Personal() : Profile() {};
	~Personal() {};
	string getFirstName();
	string getLastName();
	string getEmail();
	string getBirthday();
	string* getAddress();
	void setFirstName(string new_first_name);
	void setLastName(string new_last_name);
	void setEmail(string new_email);
	void setBirthday(string new_birthday);
	void setAddress(string new_lat, string new_lon);
	string createJsonFile();
};

class Summary: public Profile {
private:
	string summary;

	void getOwnInfo(Document &document);

public:
	Summary() : Profile() {};
	~Summary() {};
	string getSummary();
	void setSummary(string new_summary);
	string createJsonFile();
};

class Expertise: public Profile {
private:
	string company;
	string position;
	string from;
	string to;
	string expertise;

	void getOwnInfo(Document &document);

public:
	Expertise() : Profile() {};
	~Expertise() {};
	string getCompany();
	string getPosition();
	string getFrom();
	string getTo();
	string getExpertise();
	void setCompany(string new_company);
	void setPosition(string new_position);
	void setFrom(string new_from);
	void setTo(string new_to);
	void setExpertise(string new_expertise);
	string createJsonFile();
};

class Skills: public Profile {
private:
	string skills;
	string parseSkills();

	void getOwnInfo(Document &document);

public:
	Skills() : Profile() {};
	~Skills() {};
	string getSkills();
	void setSkills(string new_skills);
	string createJsonFile();
};

class Picture: public Profile {
private:
	string picture;

	void getOwnInfo(Document &document);

public:
	Picture() : Profile() {};
	~Picture() {};
	string getPicture();
	void setPicture(string new_picture);
	string createJsonFile();
};

#endif
