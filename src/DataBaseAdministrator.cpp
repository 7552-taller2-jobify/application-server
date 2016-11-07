// "Copyright 2016 <Jobify>"

#include "DataBaseAdministrator.h"
#include <string>

DataBaseAdministrator::DataBaseAdministrator() {}

DataBaseAdministrator::~DataBaseAdministrator() {}

bool DataBaseAdministrator::existsClient(std::string email) {
	return (DataBase::getInstance().get("PERSONAL_" + email) != "");
}

std::string DataBaseAdministrator::getDataOfClient(LoginInformation *loginInformation) {

    std::string personal_parser =  DataBase::getInstance().get("PERSONAL_" + loginInformation->getEmail());
    std::cout<<"personal ok"<<std::endl;
    std::string picture_parser =  DataBase::getInstance().get("PICTURE_" + loginInformation->getEmail());	
    
    std::cout<<"picture ok"<<std::endl;
    std::cout<<picture_parser<<std::endl;
    Personal *personal = new Personal();
    personal->loadJson(personal_parser);
    
    Picture *picture = new Picture();
    if (strcmp(picture_parser.c_str(), "") != 0){
        picture->loadJson(picture_parser);
    } else {
        picture->setPicture("");
    }
    //Se crea token mediante JWT
    std::string token = "AAAA.BBBB.CCCC";

    UserService *userService = new UserService();
    std::string loginInformation_parser = userService->loginInformation(personal, picture, token);
    return loginInformation_parser;
}

// Returns 0 if success, 1 if email exists, 2 if there are empty fields
int DataBaseAdministrator::addClient(Personal *personal, struct Message operation) {

    if (this->existsClient(personal->getEmail())) {
        return 1;
    }

    if (strcmp(operation.params.c_str(), "app=facebook") == 0) {
	    //DataBase::getInstance().put("TOKEN_" + person->getEmail(), llamar función token);
	    DataBase::getInstance().put("PERSONAL_" + personal->getEmail(), personal->createJsonFile());
    } else {
 	    //DataBase::getInstance().put("TOKEN_" + person->getEmail(), llamar función token);
        if (personal->emptyFields()) {
            return 2;
        }
	    DataBase::getInstance().put("PERSONAL_" + personal->getEmail(), personal->createJsonFile());
    }
    return 0;
}
