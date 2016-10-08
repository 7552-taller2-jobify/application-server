// "Copyright 2016 <Jobify>"

#include "DataBaseAdministrator.h"
#include <string>

DataBaseAdministrator::DataBaseAdministrator(){
}

DataBaseAdministrator::~DataBaseAdministrator(){
}

bool DataBaseAdministrator::existsClient(LoginInformation *loginInformation){
	std::cout<<"exist client ???"<< std::endl; 
	std::cout<<loginInformation->getEmail()<< std::endl;
	std::cout<<loginInformation->getPassword()<< std::endl;

	return (DataBase::getInstance().get(loginInformation->getEmail()) == loginInformation->getPassword());
}

std::string DataBaseAdministrator::getDataOfClient(LoginInformation *loginInformation){
	std::cout<<"get data of client ???"<< std::endl;
    return DataBase::getInstance().get("personal_" + loginInformation->getEmail());	
}
