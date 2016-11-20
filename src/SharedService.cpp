// "Copyright 2016 <Jobify>"
#include "SharedService.h"
#include <string>

SharedService::SharedService() {
    sharedServerUrl = "https://jobify-7552-taller2.herokuapp.com";
}
Response* SharedService::createCategory(std::string name, std::string description) {
    Request* request = new Request();
    return request->ExecutePost(sharedServerUrl +
        "/categories", "{ \"name\": \"" + name +"\",\"description\": \"" + description + "\"}");
}
Response* SharedService::listCategories() {
    Request* request = new Request();
    return request->Execute(sharedServerUrl +"/categories");
}

Response* SharedService::createSkill(std::string name, std::string description, std::string category) {
    Request* request = new Request();
    return request->ExecutePost(sharedServerUrl +
        "/skills/categories/" + category, "{ \"name\": \"" + name + "\",\"description\": \"" + description + "\"}");
}
Response* SharedService::listSkills() {
    Request* request = new Request();
    return request->Execute(sharedServerUrl +"/skills");
}
Response* SharedService::createJobPosition(std::string name, std::string description, std::string category) {
    Request* request = new Request();
    return request->ExecutePost(sharedServerUrl +
        "/job_positions/categories/" + category,
        "{ \"name\": \"" + name + "\",\"description\": \"" + description + "\"}");
}
Response* SharedService::listJobPositions() {
    Request* request = new Request();
    return request->Execute(sharedServerUrl + "/job_positions");
}
SharedService::~SharedService() {
}
