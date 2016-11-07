// "Copyright 2016 <Jobify>"

#include "DataBaseAdministrator.h"
#include <string>

DataBaseAdministrator::DataBaseAdministrator() {}

DataBaseAdministrator::~DataBaseAdministrator() {}

bool DataBaseAdministrator::existsClient(std::string email) {
	return (DataBase::getInstance().get("PERSONAL_" + email) != "");
}

std::string DataBaseAdministrator::getDataOfClient(Profile *loginInformation) {
	LoginInformation* li = (LoginInformation*) loginInformation;
    return DataBase::getInstance().get("personal_" + li->getEmail());	
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
