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
private:
	Logger *logger;
	string name;
	string summary;
	string picture;
	string skills;
	string position[2];
	string job_experience;
	string parseSkills();
	string createJsonFileFromProfile();

public:
	Profile(string json_file, Logger *logger);
	~Profile();
	string getName();
	string getSummary();
	string getPicture();
	string getSkills();
	string* getPosition();
	string getJobExperience();
	void setName(string new_name);
	void setSummary(string new_summary);
	void setPicture(string new_picture);
	void setSkills(string new_skills);
	void setPosition(string new_lat, string new_lon);
	void setJobExperience(string new_job_experience);
	void updateJson(string json_file);
};

#endif
