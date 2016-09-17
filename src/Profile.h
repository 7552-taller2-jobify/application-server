#ifndef PROFILE_H_
#define PROFILE_H_

#include <string>
#include <cstdio>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;
using namespace std;

class Profile {
private:
	string name;
	string summary;
	string picture;
	string skills;
	double position[2];
	string job_experience;

public:
	Profile(string json_file);
	~Profile();
	string getName();
	string getSummary();
	string getPicture();
	string getSkills();
	double* getPosition();
	string getJobExperience();
	void setName(string new_name);
	void setSummary(string new_summary);
	void setPicture(string new_picture);
	void setSkills(string new_skills);
	void setPosition(string new_lat, string new_lon);
	void setJobExperience(string new_job_experience);
};

#endif
