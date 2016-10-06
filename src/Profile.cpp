// "Copyright 2016 <Jobify>"

#include "Profile.h"
#include <string>

Profile::Profile() {}

void Profile::updateJson(std::string json_file) {
    rapidjson::Document document;
    if (document.Parse(this->createJsonFile().c_str()).HasParseError()) {
        Logger::getInstance().log(error,
                        "Could not create JSON file from profile.");
    } else {
        FILE* file = fopen(json_file.c_str(), "w");
        char buffer[65536];
        rapidjson::FileWriteStream stream(file, buffer, sizeof(buffer));
        rapidjson::Writer<rapidjson::FileWriteStream> writer(stream);
        document.Accept(writer);
        fclose(file);
        Logger::getInstance().log(info,
                            "JSON file has been created successfully.");
    }
}

void Profile::getProfileInfo(std::string json_file) {
    FILE* file = fopen(json_file.c_str(), "r");
    char readBuffer[65536];
    rapidjson::FileReadStream stream(file, readBuffer, sizeof(readBuffer));
    fclose(file);
    rapidjson::Document document;
    if (document.ParseStream(stream).HasParseError()) {
        Logger::getInstance().log(error,
                            "Could not parse file " + json_file + ".");
    } else {
        this->getOwnInfo(document);
        Logger::getInstance().log(info, "File " + json_file +
                                    " has been parsed successfully.");
    }
}



void Personal::getOwnInfo(const rapidjson::Document &document) {
    this->id = document["id"].GetDouble();
    this->first_name = document["first_name"].GetString();
    this->last_name = document["last_name"].GetString();
    this->email = document["email"].GetString();
    this->birthday = document["birthday"].GetString();
    this->address[0] = document["address"]["lat"].GetString();
    this->address[1] = document["address"]["lon"].GetString();
}

double Personal::getId() {
    return this->id;
}

std::string Personal::getFirstName() {
    return this->first_name;
}

std::string Personal::getLastName() {
    return this->last_name;
}

std::string Personal::getEmail() {
    return this->email;
}

std::string Personal::getBirthday() {
    return this->birthday;
}

std::string* Personal::getAddress() {
    return this->address;
}

void Personal::setId(double new_id) {
    this->id = new_id;
}

void Personal::setFirstName(std::string new_name) {
    this->first_name = new_name;
}

void Personal::setLastName(std::string new_last_name) {
    this->last_name = new_last_name;
}

void Personal::setEmail(std::string new_email) {
    this->email = new_email;
}

void Personal::setBirthday(std::string new_birthday) {
    this->birthday = new_birthday;
}

void Personal::setAddress(std::string new_lat, std::string new_lon) {
    this->address[0] = new_lat.c_str();
    this->address[1] = new_lon.c_str();
}

std::string Personal::createJsonFile() {
    std::ostringstream oss;
    oss << this->id;
    std::string id_aux = oss.str();

    std::string id = "{\n\t\"id\": " + id_aux + ",\n",
    first_name = "\t\"first_name\": \"" +
                                this->first_name + "\",\n",
    last_name = "\t\"last_name\": \"" + this->last_name + "\",\n",
    email = "\t\"email\": \"" + this->email + "\",\n",
    birthday = "\t\"birthday\": \"" + this->birthday + "\",\n",
    address_1 = "\t\"address\": {\n\t\t\"lat\": \"" +
                                        this->address[0] + "\",\n",
    address_2 = "\t\t\"lon\": \"" + this->address[1] + "\"\n\t}\n}";
    return id + first_name + last_name + email + birthday +
                                    address_1 + address_2;
}



void Summary::getOwnInfo(const rapidjson::Document &document) {
    this->summary = document["summary"].GetString();
}

std::string Summary::getSummary() {
    return this->summary;
}

void Summary::setSummary(std::string new_summary) {
    this->summary = new_summary;
}

std::string Summary::createJsonFile() {
    return "{\n\t\"summary\": \"" + this->summary + "\"\n}";
}



void Expertise::getOwnInfo(const rapidjson::Document &document) {
    this->company = document["company"].GetString();
    this->position = document["position"].GetString();
    this->from = document["from"].GetString();
    this->to = document["to"].GetString();
    this->expertise = document["expertise"].GetString();
}

std::string Expertise::getCompany() {
    return this->company;
}

std::string Expertise::getPosition() {
    return this->position;
}

std::string Expertise::getFrom() {
    return this->from;
}

std::string Expertise::getTo() {
    return this->to;
}

std::string Expertise::getExpertise() {
    return this->expertise;
}

void Expertise::setCompany(std::string new_company) {
    this->company = new_company;
}

void Expertise::setPosition(std::string new_position) {
    this->position = new_position;
}

void Expertise::setFrom(std::string new_from) {
    this->from = new_from;
}

void Expertise::setTo(std::string new_to) {
    this->to = new_to;
}

void Expertise::setExpertise(std::string new_expertise) {
    this->expertise = new_expertise;
}

std::string Expertise::createJsonFile() {
    std::string company = "{\n\t\"company\": \"" + this->company + "\",\n",
    position = "\t\"position\": \"" + this->position + "\",\n",
    from = "\t\"from\": \"" + this->from + "\",\n",
    to = "\t\"to\": \"" + this->to + "\",\n",
    expertise = "\t\"expertise\": \"" + this->expertise + "\"\n}";
    return company + position + from + to + expertise;
}



void Skills::getOwnInfo(const rapidjson::Document &document) {
    for (rapidjson::SizeType i = 0; i < document["skills"].Size(); i++) {
        if (i != 0) {
            this->skills += ", ";
        }
        this->skills += document["skills"][i].GetString();
    }
}

std::string Skills::parseSkills() {
    std::string parsed_skills;
    std::stringstream ss;
    ss.str(this->skills);
    std::string item, aux;
    while (getline(ss, item, ',')) {
        item.erase(remove(item.begin(), item.end(), ' '), item.end());
        aux = "\"" + item + "\", ";
        parsed_skills += aux;
    }
    parsed_skills = parsed_skills.substr(0, parsed_skills.length() - 2);
    parsed_skills += "]\n}";
    return parsed_skills;
}

std::string Skills::getSkills() {
    return this->skills;
}

void Skills::setSkills(std::string new_skills) {
    this->skills = new_skills;
}

std::string Skills::createJsonFile() {
    return "{\n\t\"skills\": [" + this->parseSkills();
}



void Picture::getOwnInfo(const rapidjson::Document &document) {
    this->picture = document["picture"].GetString();
}

std::string Picture::getPicture() {
    return this->picture;
}

void Picture::setPicture(std::string new_picture) {
    this->picture = new_picture;
}

std::string Picture::createJsonFile() {
    return "{\n\t\"picture\": \"" + this->picture + "\"\n}";
}
