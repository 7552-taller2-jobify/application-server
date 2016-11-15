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

bool DataBaseAdministrator::rigthClient(LoginInformation *loginInformation) {
    std::string email = loginInformation->getEmail();
    std::string password = loginInformation->getPassword();     
    bool existsClient = this->existsClient(email);
    if (existsClient) {
        std::string credentials_parser = DataBase::getInstance().get(email);
        Credentials *credentials = new Credentials();
        credentials->loadJson(credentials_parser);
        std::string token_calculated = this->auth->encode(email, password, credentials->getIncrementalNumber());
        bool rigthCredentials = strcmp(credentials->getToken().c_str(), token_calculated.c_str()) == 0;
        if (rigthCredentials) {
            return true;
        }
    }
    return false;
}

bool DataBaseAdministrator::rigthClient(std::string email, std::string token) {     
    bool existsClient = this->existsClient(email);
    if (existsClient) {
        std::cout << "exists client" << std::endl;        
        std::string credentials_parser = DataBase::getInstance().get(email);
        Credentials *credentials = new Credentials();
        credentials->loadJson(credentials_parser);
        std::string token_stored = credentials->getToken();
        bool rigthCredentials = strcmp(token_stored.c_str(), token.c_str()) == 0;
        if (rigthCredentials) { 
            return true;
        }
    }
    std::cout << "not exists client" << std::endl;
    return false;
}

std::string DataBaseAdministrator::getPersonalLogin(std::string email) {
    std::string personal_parser =  DataBase::getInstance().get("PERSONAL_" + email);
    std::string picture_parser =  DataBase::getInstance().get("PICTURE_" + email);
    Personal *personal = new Personal();
    personal->loadJson(personal_parser);
    Picture *picture = new Picture();
    bool hasPicture = strcmp(picture_parser.c_str(), "") != 0;
    if (hasPicture) {
        picture->loadJson(picture_parser);
    } else {
        picture->setPicture("");
    }
    std::string credentials_parser =  DataBase::getInstance().get(email);
    Credentials *credentials = new Credentials();
    credentials->loadJson(credentials_parser);
    std::string token = credentials->getToken();
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
    if (personal->emptyFields()) {
        return 2;
    }
    bool hasLoginFacebook = strcmp(operation.params.c_str(), "app=facebook") == 0;
    if (hasLoginFacebook) {
        DataBase::getInstance().put("PERSONAL_" + email, personal->createJsonFile());
    } else {
        int incremental_number = 0;
        std::string token = this->auth->encode(email, password, incremental_number);
        Credentials *credentials = new Credentials();
        credentials->setToken(token);
        credentials->setIncrementalNumber(incremental_number);
        std::string credentials_parser = credentials->createJsonFile();
        DataBase::getInstance().put(email, credentials_parser);
        DataBase::getInstance().put("PERSONAL_" + email, personal->createJsonFile());
    }
    return 0;
}

// Returns 0 if success, 1 if credential invalid,
int DataBaseAdministrator::uploadPersonal(std::string email, std::string token, Personal *upload_personal) {
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

std::string DataBaseAdministrator::getPersonal(std::string email) {
    std::string personal_parser =  DataBase::getInstance().get("PERSONAL_" + email);
    Personal *personal = new Personal();
    personal->loadJson(personal_parser);
    UserService *userService = new UserService();
    std::string personal_parser_modify = userService->getPersonal(personal);
    return personal_parser_modify;
}

// Returns 0 if success, 1 if credential invalid,
int DataBaseAdministrator::uploadSummary(std::string email, std::string token, Summary *upload_summary) {
    bool rigthCredential = this->rigthClient(email, token);
    if (rigthCredential) {
        DataBase::getInstance().erase("SUMMARY_" + email);
        DataBase::getInstance().put("SUMMARY_" + email, upload_summary->createJsonFile());
        return 0;
    }
    return 1;
}

std::string DataBaseAdministrator::getSummary(std::string email){
    std::string summary_parser = DataBase::getInstance().get("SUMMARY_" + email);
    return summary_parser;
}

// Returns 0 if success, 1 if credential invalid,
int DataBaseAdministrator::uploadExpertise(std::string email, std::string token, Expertise *upload_expertise) {
    bool rigthCredential = this->rigthClient(email, token);
    if (rigthCredential) {
        DataBase::getInstance().erase("EXPERTISE_" + email);
        DataBase::getInstance().put("EXPERTISE_" + email, upload_expertise->createJsonFile());
        return 0;
    }
    return 1;
}

std::string DataBaseAdministrator::getExpertise(std::string email) {
    return DataBase::getInstance().get("EXPERTISE_" + email);
}

// Returns 0 if success, 1 if credential invalid,
int DataBaseAdministrator::uploadSkills(std::string email, std::string token, Skills *upload_skills) {
    bool rigthCredential = this->rigthClient(email, token);
    if (rigthCredential) {
        DataBase::getInstance().erase("SKILLS_" + email);
        DataBase::getInstance().put("SKILLS_" + email, upload_skills->createJsonFile());
        return 0;
    }
    return 1;
}

std::string DataBaseAdministrator::getSkills(std::string email) {
    return DataBase::getInstance().get("SKILLS_" + email);
}

// Returns 0 if success, 1 if credential invalid,
int DataBaseAdministrator::uploadPicture(std::string email, std::string token, Picture *upload_picture) {
    bool rigthCredential = this->rigthClient(email, token);
    if (rigthCredential) {
        DataBase::getInstance().erase("PICTURE_" + email);
        DataBase::getInstance().put("PICTURE_" + email, upload_picture->createJsonFile());
        return 0;
    }
    return 1;
}

std::string DataBaseAdministrator::getPicture(std::string email) {
    return DataBase::getInstance().get("PICTURE_" + email);
}

// Returns 0 if success, 1 if credential invalid,
int DataBaseAdministrator::addSolicitude(std::string email, std::string token, struct Solicitude new_solicitude) {
    bool rigthCredential = this->rigthClient(email, token);
    if (rigthCredential) {
        Solicitudes *solicitudes = new Solicitudes();
        if (DataBase::getInstance().get("SOLICITUDES_" + email) == "") {
            DataBase::getInstance().put("SOLICITUDES_" + email, "{\"solicitudes\":[]}");
        }
        solicitudes->loadJson(DataBase::getInstance().get("SOLICITUDES_" + email));
        DataBase::getInstance().erase("SOLICITUDES_" + email);
        solicitudes->addSolicitude(new_solicitude);
        DataBase::getInstance().put("SOLICITUDES_" + email, solicitudes->createJsonFile());
        delete solicitudes;
        return 0;
    }
    return 1;
}

std::string DataBaseAdministrator::getSolicitudes(std::string email) {
    return DataBase::getInstance().get("SOLICITUDES_" + email);
}
