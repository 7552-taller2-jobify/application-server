#include "Profile.h"

Profile::Profile() {
}

void Profile::updateJson(string json_file) {
	Document aux_document;
	if(aux_document.Parse(this->createJsonFile().c_str()).HasParseError()) {
		Logger::getInstance().log(error, "Could not create JSON file from profile.");
	} else {	
		FILE* file = fopen(json_file.c_str(), "w");
		char writeBuffer[65536];
		FileWriteStream stream(file, writeBuffer, sizeof(writeBuffer));
		Writer<FileWriteStream> writer(stream);
		aux_document.Accept(writer);
		fclose(file);
		Logger::getInstance().log(info, "JSON file has been created successfully.");
	}
}

void Profile::getProfileInfo(string json_file) {
	FILE* file = fopen(json_file.c_str(), "r");
	char readBuffer[65536];
	FileReadStream stream(file, readBuffer, sizeof(readBuffer));
	fclose(file);
Document aux_document;
	if(aux_document.ParseStream(stream).HasParseError()) {
		Logger::getInstance().log(error, "Could not parse file " + json_file + ".");
	} else {
		this->getOwnInfo(aux_document);
		Logger::getInstance().log(info, "File " + json_file + " has been parsed successfully.");
	}
}



void Personal::getOwnInfo(Document &document) {
	this->first_name = document["first_name"].GetString();
	this->last_name = document["last_name"].GetString();
	this->email = document["email"].GetString();
	this->birthday = document["birthday"].GetString();
	this->address[0] = document["address"]["lat"].GetString();
	this->address[1] = document["address"]["lon"].GetString();
}

string Personal::getFirstName() {
	return this->first_name;
}

string Personal::getLastName() {
	return this->last_name;
}

string Personal::getEmail() {
	return this->email;
}

string Personal::getBirthday() {
	return this->birthday;
}

string* Personal::getAddress() {
	return this->address;
}

void Personal::setFirstName(string new_name) {
	this->first_name = new_name;
}

void Personal::setLastName(string new_last_name) {
	this->last_name = new_last_name;
}

void Personal::setEmail(string new_email) {
	this->email = new_email;
}

void Personal::setBirthday(string new_birthday) {
	this->birthday = new_birthday;
}

void Personal::setAddress(string new_lat, string new_lon) {
	this->address[0] = new_lat.c_str();
	this->address[1] = new_lon.c_str();
}

string Personal::createJsonFile() {
	string first_name = "{\n\t\"profile\": {\n\t\t\"first_name\": \"" + this->first_name + "\",\n",
	last_name = "\t\t\"last_name\": \"" + this->last_name + "\",\n",
	email = "\t\t\"email\": \"" + this->email + "\",\n",
	birthday = "\t\t\"birthday\": \"" + this->birthday + "\",\n",
	address_1 = "\t\t\"address\": {\n\t\t\t\"lat\": \"" + this->address[0] + "\",\n",
	address_2 = "\t\t\t\"lon\": \"" + this->address[1] + "\"\n\t\t}\n}";
	return first_name + last_name + email + birthday + address_1 + address_2;
}



void Summary::getOwnInfo(Document &document) {
	this->summary = document["summary"].GetString();
}

string Summary::getSummary() {
	return this->summary;
}

void Summary::setSummary(string new_summary) {
	this->summary = new_summary;
}

string Summary::createJsonFile() {
	return "{\n\t\"profile\": {\n\t\t\"summary\": \"" + this->summary + "\"\n}"; 
}



void Expertise::getOwnInfo(Document &document) {
	this->company = document["company"].GetString();
	this->position = document["position"].GetString();
	this->from = document["from"].GetString();
	this->to = document["to"].GetString();
	this->expertise = document["expertise"].GetString();
}

string Expertise::getCompany() {
	return this->company;
}

string Expertise::getPosition() {
	return this->position;
}

string Expertise::getFrom() {
	return this->from;
}

string Expertise::getTo() {
	return this->to;
}

string Expertise::getExpertise() {
	return this->expertise;
}

void Expertise::setCompany(string new_company) {
	this->company = new_company;
}

void Expertise::setPosition(string new_position) {
	this->position = new_position;
}

void Expertise::setFrom(string new_from) {
	this->from = new_from;
}

void Expertise::setTo(string new_to) {
	this->to = new_to;
}

void Expertise::setExpertise(string new_expertise) {
	this->expertise = new_expertise;
}

string Expertise::createJsonFile() {
	string company = "{\n\t\"profile\": {\n\t\t\"company\": \"" + this->company + "\",\n",
	position = "\t\t\"position\": \"" + this->position + "\",\n",
	from = "\t\t\"from\": \"" + this->from + "\",\n",
	to = "\t\t\"to\": \"" + this->to + "\",\n",
	expertise = "\t\t\"expertise\": \"" + this->expertise + "\"\n\t\t}\n}";
	return company + position + from + to + expertise;
}



void Skills::getOwnInfo(Document &document) {
	for(SizeType i = 0; i < document["skills"].Size(); i++) {
		if(i != 0) {
			this->skills += ", ";
		}
		this->skills += document["skills"][i].GetString();
	}
}

string Skills::parseSkills() {
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
	parsed_skills += "]\n}";
	return parsed_skills;
}

string Skills::getSkills() {
	return this->skills;
}

void Skills::setSkills(string new_skills) {
	this->skills = new_skills;
}

string Skills::createJsonFile() {
	return "{\n\t\"profile\": {\n\t\t\"skills\": [" + this->parseSkills();
}



void Picture::getOwnInfo(Document &document) {
	this->picture = document["picture"].GetString();
}

string Picture::getPicture() {
	return this->picture;
}

void Picture::setPicture(string new_picture) {
	this->picture = new_picture;
}

string Picture::createJsonFile() {
	return "{\n\t\"profile\": {\n\t\t\"picture\": \"" + this->picture + "\"\n}"; 
}
