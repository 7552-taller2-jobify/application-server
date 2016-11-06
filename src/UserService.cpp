// "Copyright 2016 <Jobify>"

#include "UserService.h"

UserService::UserService() {}

std::string UserService::loginInformation(Personal *personal, Picture *picture, std::string token) {
    return ("{\"profile\":{\"first_name\":\"" + personal->getFirstName() + "\"," +
            "\"last_name\":\"" + personal->getLastName() + "\"," +
            "\"email\":\"" + personal->getEmail() + "\"," +
            "\"picture\":\"" + picture->getPicture() + "\"}," +
            "\"metadata\":{\"token\":\"" + token + "\"}}");
}

/*std::string UserService::loginInformation(Personal *personal, Picture *picture, str::string token) {
    return ("{\"first_name\": \"" + personal->getFirstName() + "\"," +
            "\"last_name\": \"" + personal->getLastName() + "\"," +
            "\"email\": \"" + personal->getEmail() + "\"," +
            "\"picture\": \"" + picture->getPicture() + "\"}");
}*/
