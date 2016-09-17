#include "Profile.h"

Profile::Profile(string json_file) {
	FILE* file = fopen(json_file.c_str(), "r");
	char readBuffer[65536];
	FileReadStream stream(file, readBuffer, sizeof(readBuffer));
	fclose(file);
	Document document;
	document.ParseStream(stream);

	this->name = document["profile"]["name"].GetString();
	this->summary = document["profile"]["summary"].GetString();
	this->picture = document["profile"]["picture"].GetString();
	for(SizeType i = 0; i < document["profile"]["skills"].Size(); i++) {
		if(i != 0) {
			this->skills += ", ";
		}
        	this->skills += document["profile"]["skills"][i].GetString();
	}
	this->position[0] = document["profile"]["position"]["lat"].GetDouble();
	this->position[1] = document["profile"]["position"]["lon"].GetDouble();
	this->job_experience = document["profile"]["job_experience"].GetString();
}

Profile::~Profile() {}

string Profile::getName() {
	return this->name;
}

string Profile::getSummary() {
	return this->summary;
}

string Profile::getPicture() {
	return this->picture;
}

string Profile::getSkills() {
	return this->skills;
}

double* Profile::getPosition() {
	return this->position;
}

string Profile::getJobExperience() {
	return this->job_experience;
}

void Profile::setName(string new_name) {
	this->name = new_name;
}

void Profile::setSummary(string new_summary) {
	this->summary = new_summary;
}

void Profile::setPicture(string new_picture) {
	this->picture = new_picture;
}

void Profile::setSkills(string new_skills) {
	this->skills = new_skills;
}

void Profile::setPosition(string new_lat, string new_lon) {
	this->position[0] = atof(new_lat.c_str());
	this->position[1] = atof(new_lon.c_str());
}

void Profile::setJobExperience(string new_job_experience) {
	this->job_experience = new_job_experience;
}
