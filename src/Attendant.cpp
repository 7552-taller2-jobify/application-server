// "Copyright 2016 <Jobify>"

#include "Attendant.h"
#include <string>

Attendant::Attendant() {}

Attendant::~Attendant() {}

void Attendant::attend(struct Message operation) {
    if (isMethodSupported(operation.verb)) {
        std::cout<<"verb  exist"<< std::endl;
        this->functions[operation.verb](operation);
    } else {
        Logger::getInstance().log(error, "Does not exist the request " +
                                        operation.verb + ".");
    }
}

bool Attendant::isMethodSupported(std::string a_method) {
    for (std::map<std::string, function>::iterator it = this->functions.begin();
                                                it != this->functions.end(); it++) {
        if (it->first == a_method) {
            return true;
        }
    }
    return false;
}



Login::Login() {
    this->functions["GET"] = get;
    this->functions["POST"] = post;
}

Login::~Login() {}

void Login::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}

void Login::post(struct Message operation) {
    //operation.body;
}



RecoveryPass::RecoveryPass() {
    this->functions["GET"] = get;
}

RecoveryPass::~RecoveryPass() {}

void RecoveryPass::get(struct Message operation) {
    std::cout << "Chau\n" << std::endl;
}



Contact::Contact() {
    // methods->push_back("GET");
}

Contact::~Contact() {}

void Contact::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}



Accept::Accept() {
    // methods->push_back("GET");
}

Accept::~Accept() {}

void Accept::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}



Reject::Reject() {
    // methods->push_back("GET");
}

Reject::~Reject() {}

void Reject::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}



ProfilePersonal::ProfilePersonal() {
    // methods->push_back("GET");
}

ProfilePersonal::~ProfilePersonal() {}

void ProfilePersonal::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}



ProfileSummary::ProfileSummary() {
    // methods->push_back("GET");
}

ProfileSummary::~ProfileSummary() {}

void ProfileSummary::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}



ProfileExpertise::ProfileExpertise() {
    // methods->push_back("GET");
}

ProfileExpertise::~ProfileExpertise() {}

void ProfileExpertise::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}



ProfileSkills::ProfileSkills() {
    // methods->push_back("GET");
}

ProfileSkills::~ProfileSkills() {}

void ProfileSkills::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}



ProfilePhoto::ProfilePhoto() {
    // methods->push_back("GET");
}

ProfilePhoto::~ProfilePhoto() {}

void ProfilePhoto::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}
