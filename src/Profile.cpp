// "Copyright 2016 <Jobify>"

#include "Profile.h"
#include <string>
#include <iostream>

Profile::Profile() {}

void Profile::updateJson(const std::string json_file) {
    rapidjson::Document document;
    if (document.Parse(this->createJsonFile().c_str()).HasParseError()) {
        Logger::getInstance().log(error, "Could not create JSON file from profile.");
    } else {
        FILE* file = fopen(json_file.c_str(), "w");
        char buffer[65536];
        rapidjson::FileWriteStream stream(file, buffer, sizeof(buffer));
        rapidjson::Writer<rapidjson::FileWriteStream> writer(stream);
        document.Accept(writer);
        fclose(file);
        Logger::getInstance().log(info, "JSON file has been created successfully.");
    }
}

void Profile::loadJson(const std::string json) {
    rapidjson::Document document;
    rapidjson::ParseResult parseRes = document.Parse(json.c_str());
    this->getOwnInfo(document);
}

void Profile::getProfileInfo(std::string json_file) {
    FILE* file = fopen(json_file.c_str(), "r");
    char readBuffer[65536];
    rapidjson::FileReadStream stream(file, readBuffer, sizeof(readBuffer));
    fclose(file);
    rapidjson::Document document;
    if (document.ParseStream(stream).HasParseError()) {
        Logger::getInstance().log(error, "Could not parse file " + json_file + ".");
    } else {
        this->getOwnInfo(document);
        Logger::getInstance().log(info, "File " + json_file + " has been parsed successfully.");
    }
}

std::string Profile::getItemByIndex(std::string *items, int index) {
    if (index < items->length()) {
        return items[index];
    }
    return "";
}

void Profile::setItem(std::string *items, int index, std::string new_item) {
    if (index < items->length()) {
        items[index] = new_item;
    }
}



void LoginInformation::getOwnInfo(const rapidjson::Document &document) {
    this->email = document["email"].GetString();
    this->password = document["password"].GetString();
}

std::string LoginInformation::getEmail() {
    return this->email;
}

std::string LoginInformation::getPassword() {
    return this->password;
}

void LoginInformation::setEmail(std::string new_email) {
    this->email = new_email;
}

void LoginInformation::setPassword(std::string new_password) {
    this->password = new_password;
}

std::string LoginInformation::createJsonFile() {
    std::string email = "{\n\t\"email\": \"" + this->email + "\",\n",
    password = "\t\"password\": \"" + this->password + "\"\n}";
    return email + password;
}



void Personal::getOwnInfo(const rapidjson::Document &document) {
    this->first_name = document["first_name"].GetString();
    this->last_name = document["last_name"].GetString();
    this->email = document["email"].GetString();
    this->gender = document["gender"].GetString();
    this->birthday = document["birthday"].GetString();
    this->address[0] = document["address"]["lat"].GetString();
    this->address[1] = document["address"]["lon"].GetString();
    this->city = document["city"].GetString();
    this->device_id = document["device_id"].GetDouble();
}

double Personal::getDeviceId() {
    return this->device_id;
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

std::string Personal::getGender() {
    return this->gender;
}

std::string Personal::getBirthday() {
    return this->birthday;
}

std::string* Personal::getAddress() {
    return this->address;
}

std::string Personal::getCity() {
    return this->city;
}

void Personal::setDeviceId(double new_id) {
    this->device_id = new_id;
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

void Personal::setGender(std::string new_gender) {
    this->gender = new_gender;
}

void Personal::setBirthday(std::string new_birthday) {
    this->birthday = new_birthday;
}

void Personal::setAddress(std::string new_lat, std::string new_lon) {
    this->address[0] = new_lat.c_str();
    this->address[1] = new_lon.c_str();
}

void Personal::setCity(std::string new_city) {
    this->city = new_city;
}

std::string Personal::createJsonFile() {
    std::ostringstream oss;
    oss << this->device_id;
    std::string id_parsed = oss.str();

    std::string id = "{\n\t\"device_id\": " + id_parsed + ",\n",
    first_name = "\t\"first_name\": \"" + this->first_name + "\",\n",
    last_name = "\t\"last_name\": \"" + this->last_name + "\",\n",
    email = "\t\"email\": \"" + this->email + "\",\n",
    gender = "\t\"gender\": \"" + this->gender + "\",\n",
    birthday = "\t\"birthday\": \"" + this->birthday + "\",\n",
    address_1 = "\t\"address\": {\n\t\t\"lat\": \"" + this->address[0] + "\",\n",
    address_2 = "\t\t\"lon\": \"" + this->address[1] + "\"\n\t},\n",
    city = "\t\"city\": \"" + this->city + "\"\n}";
    return id + first_name + last_name + email + gender + birthday + address_1 + address_2 + city;
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
    this->number_of_expertises = document["expertises"].Size();
    this->company = new std::string[this->number_of_expertises];
    this->position = new std::string[this->number_of_expertises];
    this->from = new std::string[this->number_of_expertises];
    this->to = new std::string[this->number_of_expertises];
    this->expertise = new std::string[this->number_of_expertises];
    this->category = new std::string[this->number_of_expertises];
    for (rapidjson::SizeType i = 0; i < this->number_of_expertises; i++) {
        this->company[i] = document["expertises"][i]["company"].GetString();
        this->position[i] = document["expertises"][i]["position"].GetString();
        this->from[i] = document["expertises"][i]["from"].GetString();
        this->to[i] = document["expertises"][i]["to"].GetString();
        this->expertise[i] = document["expertises"][i]["expertise"].GetString();
        this->category[i] = document["expertises"][i]["category"].GetString();
    }
}

std::string Expertise::getCompany(int index) {
    return this->getItemByIndex(this->company, index);
}

std::string Expertise::getPosition(int index) {
    return this->getItemByIndex(this->position, index);
}

std::string Expertise::getFrom(int index) {
    return this->getItemByIndex(this->from, index);
}

std::string Expertise::getTo(int index) {
    return this->getItemByIndex(this->to, index);
}

std::string Expertise::getExpertise(int index) {
    return this->getItemByIndex(this->expertise, index);
}

std::string Expertise::getCategory(int index) {
    return this->getItemByIndex(this->category, index);
}

void Expertise::setCompany(std::string new_company, int index) {
    this->setItem(this->company, index, new_company);
}

void Expertise::setPosition(std::string new_position, int index) {
    this->setItem(this->position, index, new_position);
}

void Expertise::setFrom(std::string new_from, int index) {
    this->setItem(this->from, index, new_from);
}

void Expertise::setTo(std::string new_to, int index) {
    this->setItem(this->to, index, new_to);
}

void Expertise::setExpertise(std::string new_expertise, int index) {
    this->setItem(this->expertise, index, new_expertise);
}

void Expertise::setCategory(std::string new_category, int index) {
    this->setItem(this->category, index, new_category);
}

Expertise::~Expertise() {
    delete[] this->company;
    delete[] this->position;
    delete[] this->from;
    delete[] this->to;
    delete[] this->expertise;
    delete[] this->category;
}

std::string Expertise::createJsonFile() {
    std::string result = "{\n\t\"expertises\":\n\t[";
    int last = this->number_of_expertises - 1;
    for (int i = 0; i < this->number_of_expertises; i++) {
        result += "{\n\t\t\"company\": \"" + this->company[i] + "\",\n\t\t"
                + "\"position\": \"" + this->position[i] + "\",\n\t\t"
                + "\"from\": \"" + this->from[i] + "\",\n\t\t"
                + "\"to\": \"" + this->to[i] + "\",\n\t\t"
                + "\"expertise\": \"" + this->expertise[i] + "\",\n\t\t"
                + "\"category\": \"" + this->category[i] + "\"\n\t}";
        if (i != last) {
            result += ",\n\t";
        }
    }
    result += "]\n}";
    return result;
}



void Skills::getOwnInfo(const rapidjson::Document &document) {
    this->number_of_skills = document["every_skill"].Size();
    this->skills = new std::string[this->number_of_skills];
    this->category = new std::string[this->number_of_skills];
    for (rapidjson::SizeType i = 0; i < this->number_of_skills; i++) {
        for (rapidjson::SizeType j = 0; j < document["every_skill"][i]["skills"].Size(); j++) {
            if (j != 0) {
                this->skills[i] += ", ";
            }
            this->skills[i] += document["every_skill"][i]["skills"][j].GetString();
        }
        this->category[i] = document["every_skill"][i]["category"].GetString();
    }
}

std::string Skills::parseSkills(int index) {
    std::string parsed_skills;
    std::stringstream ss;
    ss.str(this->skills[index]);
    std::string item, aux;
    while (getline(ss, item, ',')) {
        item.erase(remove(item.begin(), item.end(), ' '), item.end());
        aux = "\"" + item + "\", ";
        parsed_skills += aux;
    }
    parsed_skills = parsed_skills.substr(0, parsed_skills.length() - 2);
    return parsed_skills;
}

std::string Skills::getSkills(int index) {
    return this->getItemByIndex(this->skills, index);
}

std::string Skills::getCategory(int index) {
    return this->getItemByIndex(this->category, index);
}

void Skills::setSkills(std::string new_skills, int index) {
    this->setItem(this->skills, index, new_skills);
}

void Skills::setCategory(std::string new_category, int index) {
    this->setItem(this->category, index, new_category);
}

std::string Skills::createJsonFile() {
    std::string result = "{\n\t\"every_skill\":\n\t[";
    int last = this->number_of_skills - 1;
    for (int i = 0; i < this->number_of_skills; i++) {
        result += "{\n\t\t\"skills\": [" + this->parseSkills(i) + "],\n\t\t"
                + "\"category\": \"" + this->category[i] + "\"\n\t}";
        if (i != last) {
            result += ",\n\t";
        }
    }
    result += "]\n}";
    return result;
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



void Contacts::getOwnInfo(const rapidjson::Document &document) {
    this->number_of_contacts = document["number_of_contacts"].GetInt();
}

int Contacts::getNumberOfContacts() {
    return this->number_of_contacts;
}

void Contacts::addContact(std::string contact_to_add) {
    this->contacts.push_back(contact_to_add);
    std::sort(this->contacts.begin(), this->contacts.end(), std::greater<std::string>());
    this->number_of_contacts++;
}

void Contacts::removeContact(std::string contact_to_remove) {
    bool found = false;
    int index = 0;
    while (!found) {
        std::string contact = this->contacts.at(index);
        if (std::strcmp(contact.c_str(), contact_to_remove.c_str())) {
            this->contacts.erase(this->contacts.begin() + index);
            continue;
        }
        index++;
    }
    this->number_of_contacts--;
}

std::string Contacts::createJsonFile() {
    std::ostringstream oss;
    oss << this->number_of_contacts;
    std::string contacts_parsed = oss.str();

    return "{\n\t\"number_of_contacts\": " + contacts_parsed + "\n}";
}
