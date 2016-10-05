// "Copyright 2016 <Jobify>"

#include "Attendant.h"
#include <string>

Attendant::Attendant() {
    this->methods = new std::vector<std::string>();
}

Attendant::~Attendant() {}

void Attendant::attend(std::string operation) {}

bool Attendant::isMethodSupported(std::string a_method) {
    for (int i = 0; i < this->methods->size(); i++) {
        if (this->methods->at(i) == a_method) {
            return true;
        }
    }
    return false;
}

Login::Login() {
    methods->push_back("GET");
}

Login::~Login() {}

void Login::attend(std::string operation) {}



RecoveryPass::RecoveryPass() {
    // methods->push_back("GET");
}

RecoveryPass::~RecoveryPass() {}

void RecoveryPass::attend(std::string operation) {}



Contact::Contact() {
    // methods->push_back("GET");
}

Contact::~Contact() {}

void Contact::attend(std::string operation) {}



Accept::Accept() {
    // methods->push_back("GET");
}

Accept::~Accept() {}

void Accept::attend(std::string operation) {}



Reject::Reject() {
    // methods->push_back("GET");
}

Reject::~Reject() {}

void Reject::attend(std::string operation) {}



ProfilePersonal::ProfilePersonal() {
    // methods->push_back("GET");
}

ProfilePersonal::~ProfilePersonal() {}

void ProfilePersonal::attend(std::string operation) {}



ProfileSummary::ProfileSummary() {
    // methods->push_back("GET");
}

ProfileSummary::~ProfileSummary() {}

void ProfileSummary::attend(std::string operation) {}



ProfileExpertise::ProfileExpertise() {
    // methods->push_back("GET");
}

ProfileExpertise::~ProfileExpertise() {}

void ProfileExpertise::attend(std::string operation) {}



ProfileSkills::ProfileSkills() {
    // methods->push_back("GET");
}

ProfileSkills::~ProfileSkills() {}

void ProfileSkills::attend(std::string operation) {}



ProfilePhoto::ProfilePhoto() {
    // methods->push_back("GET");
}

ProfilePhoto::~ProfilePhoto() {}

void ProfilePhoto::attend(std::string operation) {}
