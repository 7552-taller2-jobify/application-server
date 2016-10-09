// "Copyright 2016 <Jobify>"

#include "DataBaseAdministrator.h"
#include <string>

DataBaseAdministrator::DataBaseAdministrator(){
}

DataBaseAdministrator::~DataBaseAdministrator(){
}

bool DataBaseAdministrator::existsClient(Profile *loginInformation){
	LoginInformation* li = (LoginInformation*) loginInformation;
	std::cout<<li->getEmail()<< std::endl;
	std::cout<<li->getPassword()<< std::endl;

	return (DataBase::getInstance().get(li->getEmail()) == li->getPassword());
}

std::string DataBaseAdministrator::getDataOfClient(Profile *loginInformation){
	LoginInformation* li = (LoginInformation*) loginInformation;
    return DataBase::getInstance().get("personal_" + li->getEmail());	
}

void DataBaseAdministrator::addClient(Profile *loginInformation, Profile* personal){
	LoginInformation* li = (LoginInformation*) loginInformation;
	DataBase::getInstance().put(li->getEmail(), li->getPassword());
	Personal* p = (Personal*) personal;
	DataBase::getInstance().put("personal_" + p->getEmail(), p->createJsonFile());
}
