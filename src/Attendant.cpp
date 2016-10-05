// "Copyright 2016 <Jobify>"

#include "Attendant.h"
#include <string>

Attendant::Attendant() {}

Attendant::~Attendant() {}

void Attendant::attend(struct Message operation) {
    if (isMethodSupported(operation.verb_http)) {
        this->functions[operation.verb_http](operation);
    } else {
        // Logger::log(error, "Does not exist the request " +
        //                    operation.verb_http + ".");
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
}

Login::~Login() {}

void Login::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}

void Login::hola() {}



RecoveryPass::RecoveryPass() {
    this->functions["GET"] = get;
}

RecoveryPass::~RecoveryPass() {}

void RecoveryPass::get(struct Message operation) {
    std::cout << "Chau\n" << std::endl;
}

void RecoveryPass::hola() {}



Contact::Contact() {
    // methods->push_back("GET");
}

Contact::~Contact() {}

void Contact::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}

void Contact::hola() {}



Accept::Accept() {
    // methods->push_back("GET");
}

Accept::~Accept() {}

void Accept::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}

void Accept::hola() {}



Reject::Reject() {
    // methods->push_back("GET");
}

Reject::~Reject() {}

void Reject::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}

void Reject::hola() {}



ProfilePersonal::ProfilePersonal() {
    // methods->push_back("GET");
}

ProfilePersonal::~ProfilePersonal() {}

void ProfilePersonal::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}

void ProfilePersonal::hola() {}



ProfileSummary::ProfileSummary() {
    // methods->push_back("GET");
}

ProfileSummary::~ProfileSummary() {}

void ProfileSummary::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}

void ProfileSummary::hola() {}



ProfileExpertise::ProfileExpertise() {
    // methods->push_back("GET");
}

ProfileExpertise::~ProfileExpertise() {}

void ProfileExpertise::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}

void ProfileExpertise::hola() {}



ProfileSkills::ProfileSkills() {
    // methods->push_back("GET");
}

ProfileSkills::~ProfileSkills() {}

void ProfileSkills::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}

void ProfileSkills::hola() {}



ProfilePhoto::ProfilePhoto() {
    // methods->push_back("GET");
}

ProfilePhoto::~ProfilePhoto() {}

void ProfilePhoto::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}

void ProfilePhoto::hola() {}
