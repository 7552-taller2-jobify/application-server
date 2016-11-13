// "Copyright 2016 <Jobify>"

#include "DataBaseAdministrator.h"
#include <string>

DataBaseAdministrator::DataBaseAdministrator() {
    this->auth = new Authentication();
}

DataBaseAdministrator::~DataBaseAdministrator() {
    delete this->auth;
}

bool DataBaseAdministrator::existsClient(std::string email) {
    return (DataBase::getInstance().get(email) != "");
}

bool DataBaseAdministrator::rigthClient(LoginInformation *loginInformation){
    std::string email = loginInformation->getEmail();
    std::string password = loginInformation->getPassword();     
    bool existsClient = this->existsClient(email);
    if (existsClient) {
        std::string token_stored = DataBase::getInstance().get(email);
        std::string token_calculated = this->auth->encode(email, password);
        bool rigthCredentials = strcmp(token_stored.c_str(), token_calculated.c_str()) == 0;
        if (rigthCredentials) {
            return true;
        }
    }
    return false;
}

bool DataBaseAdministrator::rigthClient(std::string email, std::string token){     
    bool existsClient = this->existsClient(email);
    if (existsClient) {
        std::string token_stored = DataBase::getInstance().get(email);
        bool rigthCredentials = strcmp(token_stored.c_str(), token.c_str()) == 0;
        if (rigthCredentials) {
            return true;
        }
    }
    return false;
}

std::string DataBaseAdministrator::getDataOfClient(LoginInformation *loginInformation) {
    std::string email = loginInformation->getEmail();
    std::string password = loginInformation->getPassword();

    std::string personal_parser =  DataBase::getInstance().get("PERSONAL_" + email);
    std::cout << "personal ok" << std::endl;
    std::string picture_parser =  DataBase::getInstance().get("PICTURE_" + email);

    std::cout << "picture ok" << std::endl;
    std::cout << picture_parser << std::endl;
    Personal *personal = new Personal();
    personal->loadJson(personal_parser);

    Picture *picture = new Picture();
    if (strcmp(picture_parser.c_str(), "") != 0) {
        picture->loadJson(picture_parser);
    } else {
        picture->setPicture("");
    }
    std::string token = DataBase::getInstance().get(email);
    UserService *userService = new UserService();
    std::string loginInformation_parser = userService->loginInformation(personal, picture, token);
    return loginInformation_parser;
}

// Returns 0 if success, 1 if email exists, 2 if there are empty fields
int DataBaseAdministrator::addClient(Personal *personal, LoginInformation *loginInformation, struct Message operation) {
    std::string email = loginInformation->getEmail();
    std::string password = loginInformation->getPassword();

    if (this->existsClient(email)) {
        return 1;
    }

    if (strcmp(operation.params.c_str(), "app=facebook") == 0) {
        DataBase::getInstance().put("PERSONAL_" + email, personal->createJsonFile());
    } else if (personal->emptyFields()) {
        return 2;
    }
    std::string token = this->auth->encode(email, password);
    DataBase::getInstance().put(email, token);
    DataBase::getInstance().put("PERSONAL_" + email, personal->createJsonFile());
    return 0;
}

// Returns 0 if success, 1 if credential invalid,
int DataBaseAdministrator::uploadPersonal(std::string email, std::string token, Personal *upload_personal){
    bool rigthCredential = this->rigthClient(email, token);
    if (rigthCredential) {
        std::string actual_personal_parser =  DataBase::getInstance().get("PERSONAL_" + email);
        Personal *actual_personal = new Personal();
        actual_personal->loadJson(actual_personal_parser);
        
        actual_personal->setFirstName(upload_personal->getFirstName());
        actual_personal->setLastName(upload_personal->getLastName());
        actual_personal->setGender(upload_personal->getGender());
        actual_personal->setBirthday(upload_personal->getBirthday());
        actual_personal->setCity(upload_personal->getCity());
        actual_personal->setAddress(upload_personal->getAddress()[0], upload_personal->getAddress()[1]);
        
        DataBase::getInstance().erase("PERSONAL_" + email);
        DataBase::getInstance().put("PERSONAL_" + email, actual_personal->createJsonFile());
        return 0;
    }
    return 1;
}

std::string DataBaseAdministrator::getPersonal(std::string email){
    std::string personal_parser =  DataBase::getInstance().get("PERSONAL_" + email);
    Personal *personal = new Personal();
    personal->loadJson(personal_parser);

    UserService *userService = new UserService();
    return userService->getPersonal(personal);
}
