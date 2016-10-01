#include "Profile.h"

Profile::Profile(string json_file, Logger *logger) {
	this->logger = logger;
	FILE* file = fopen(json_file.c_str(), "r");
	char readBuffer[65536];
	FileReadStream stream(file, readBuffer, sizeof(readBuffer));
	fclose(file);
	Document document;
	if(document.ParseStream(stream).HasParseError()) {
		this->logger->log(error, "Could not parse file " + json_file + ".");
	} else {
		this->name = document["profile"]["name"].GetString();
		this->summary = document["profile"]["summary"].GetString();
		this->picture = document["profile"]["picture"].GetString();
		for(SizeType i = 0; i < document["profile"]["skills"].Size(); i++) {
			if(i != 0) {
				this->skills += ", ";
			}
			this->skills += document["profile"]["skills"][i].GetString();
		}
		this->position[0] = document["profile"]["position"]["lat"].GetString();
		this->position[1] = document["profile"]["position"]["lon"].GetString();
		this->job_experience = document["profile"]["job_experience"].GetString();
		this->logger->log(info, "File " + json_file + " has been parsed successfully.");
	}
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

string* Profile::getPosition() {
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
	this->position[0] = new_lat.c_str();
	this->position[1] = new_lon.c_str();
}

void Profile::setJobExperience(string new_job_experience) {
	this->job_experience = new_job_experience;
}

string Profile::parseSkills() {
	string parsed_skills;
	stringstream ss;
	ss.str(this->skills);
	string item, aux;
	while(getline(ss, item, ',')) {
		item.erase(remove(item.begin(), item.end(), ' '), item.end());
		aux = "\"" + item + "\", ";
        	parsed_skills += aux;
    	}
	parsed_skills = parsed_skills.substr(0, parsed_skills.length() - 2);
	parsed_skills += "],\n";
	return parsed_skills;
}

string Profile::createJsonFileFromProfile() {
	string name = "{\n\t\"profile\": {\n\t\t\"name\": \"" + this->name + "\",\n",
	summary = "\t\t\"summary\": \"" + this->summary + "\",\n",
	picture = "\t\t\"picture\": \"" + this->picture + "\",\n",
	skills = "\t\t\"skills\": [" + this->parseSkills(),
	position_1 = "\t\t\"position\": {\n\t\t\t\"lat\": \"" + this->position[0] + "\",\n",
	position_2 = "\t\t\t\"lon\": \"" + this->position[1] + "\"\n\t\t},\n",
	job_experience = "\t\t\"job_experience\": \"" + this->job_experience + "\"\n\t}\n}";
	return name + summary + picture + skills + position_1 + position_2 + job_experience;
}

void Profile::updateJson(string json_file) {
	Document document;
	if(document.Parse(this->createJsonFileFromProfile().c_str()).HasParseError()) {
		this->logger->log(error, "Could not create JSON file from profile.");
	} else {	
		FILE* file = fopen(json_file.c_str(), "w");
		char writeBuffer[65536];
		FileWriteStream stream(file, writeBuffer, sizeof(writeBuffer));
		Writer<FileWriteStream> writer(stream);
		document.Accept(writer);
		fclose(file);
		this->logger->log(info, "JSON file has been created successfully.");
	}
}
