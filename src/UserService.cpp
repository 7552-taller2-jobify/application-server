// "Copyright 2016 <Jobify>"

#include "UserService.h"
#include <string>

UserService::UserService() {}

std::string UserService::loginInformation(Personal *personal, Picture *picture, std::string token) {
    std::cout << "POR RETORNAR LOGIN INFORMATION" << std::endl;
    return ("{\"profile\":{\"first_name\":\"" + personal->getFirstName() + "\"," +
            "\"last_name\":\"" + personal->getLastName() + "\"," +
            "\"email\":\"" + personal->getEmail() + "\"," +
            "\"picture\":\"" + picture->getPicture() + "\"}," +
            "\"metadata\":{\"token\":\"" + token + "\"}}");
}

std::string UserService::getPersonal(Personal *personal) {
    return ("{\"first_name\":\"" + personal->getFirstName() + "\"," +
            "\"last_name\":\"" + personal->getLastName() + "\"," +
            "\"email\":\"" + personal->getEmail() + "\"," +
            "\"birthday\":\"" + personal->getBirthday() + "\"," +
            "\"address\":{\"lat\":\"" + personal->getLat() + "\", \"lon\":\"" + personal->getLon() + "\"}," +
            "\"gender\":\"" + personal->getGender() + "\"}");
}
