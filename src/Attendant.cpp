// "Copyright 2016 <Jobify>"

#include "Attendant.h"

Attendant::Attendant() {
}

Attendant::~Attendant() {}

Response* Attendant::attend(struct Message operation) {
    Response* response = NULL;
    if (isMethodSupported(operation.verb)) {
        std::cout<<"verb  exist"<< std::endl;
         response = this->functions[operation.verb](operation);
    } else {
        Logger::getInstance().log(error, "Does not exist the request " + operation.verb + ".");
    }
    return response;
}

bool Attendant::isMethodSupported(std::string a_method) {
    for (std::map<std::string, function>::iterator it = this->functions.begin(); it != this->functions.end(); it++) {
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

Response* Login::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}

Response* Login::post(struct Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    LoginInformation *loginInformation = new LoginInformation();
    loginInformation->loadJson(operation.body.c_str());
    
    Response* response = NULL;

    if (dbAdministrator->existsClient(loginInformation)){
        // Falta contemplar mas chequeos         
        response = new Response();
        response->setContent(dbAdministrator->getDataOfClient(loginInformation));
        response->setStatus(200);
    }   

    return response;
}



RecoveryPass::RecoveryPass() {
    this->functions["GET"] = get;
}

RecoveryPass::~RecoveryPass() {}

Response* RecoveryPass::get(struct Message operation) {
    std::cout << "Chau\n" << std::endl;
}



Contact::Contact() {
    // methods->push_back("GET");
}

Contact::~Contact() {}

Response* Contact::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}



Accept::Accept() {
    // methods->push_back("GET");
}

Accept::~Accept() {}

Response* Accept::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}



Reject::Reject() {
    // methods->push_back("GET");
}

Reject::~Reject() {}

Response* Reject::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}



ProfilePersonal::ProfilePersonal() {
    // methods->push_back("GET");
}

ProfilePersonal::~ProfilePersonal() {}

Response* ProfilePersonal::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}



ProfileSummary::ProfileSummary() {
    // methods->push_back("GET");
}

ProfileSummary::~ProfileSummary() {}

Response* ProfileSummary::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}



ProfileExpertise::ProfileExpertise() {
    // methods->push_back("GET");
}

ProfileExpertise::~ProfileExpertise() {}

Response* ProfileExpertise::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}



ProfileSkills::ProfileSkills() {
    // methods->push_back("GET");
}

ProfileSkills::~ProfileSkills() {}

Response* ProfileSkills::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}



ProfilePhoto::ProfilePhoto() {
    // methods->push_back("GET");
}

ProfilePhoto::~ProfilePhoto() {}

Response* ProfilePhoto::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}
