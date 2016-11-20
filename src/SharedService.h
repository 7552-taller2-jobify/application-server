// "Copyright 2016 <Jobify>"
#ifndef SHAREDSERVICE_H_
#define SHAREDSERVICE_H_
#include "Request.h"
#include "Response.h"
#include <string>


class SharedService{
 private:
    std::string sharedServerUrl;
 public:
    SharedService();
    Response* createCategory(std::string name, std::string descripcion);
    Response* listCategories();
    Response* createSkill(std::string name, std::string descripcion, std::string category);
    Response* listSkills();
    Response* createJobPosition(std::string name, std::string descripcion, std::string category);
    Response* listJobPositions();
    ~SharedService();
};
#endif  // SHAREDSERVICE_H_
