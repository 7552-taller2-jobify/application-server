// "Copyright 2016 <Jobify>"

#include "Attendant.h"
#include <string>
#include <map>

Attendant::Attendant() {}

Attendant::~Attendant() {}

Response* Attendant::attend(struct Message operation) {
    Response* response = NULL;
    if (this->isMethodSupported(operation.verb)) {
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
    this->functions["POST"] = post;
}

Login::~Login() {}

Response* Login::post(struct Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    LoginInformation *loginInformation = new LoginInformation();
    if (strcmp(operation.params.c_str(), "app=facebook") == 0) {
        // cargar datos de facebook
        loginInformation->setEmail("");
        loginInformation->setPassword("");
    } else {
        loginInformation->loadJson(operation.body.c_str());
    }

    Response* response = new Response();
    bool rigthCredential = dbAdministrator->rigthClient(loginInformation);
    if (rigthCredential) {
        response->setContent(dbAdministrator->getDataOfClient(loginInformation));
        response->setStatus(200);
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
    }

    return response;
}

Register::Register() {
    this->functions["POST"] = post;
}

Register::~Register() {}

Response* Register::post(struct Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    Personal *personal = new Personal();
    LoginInformation *loginInformation = new LoginInformation();

    if (strcmp(operation.params.c_str(), "app=facebook") == 0) {
        // cargar datos de facebook
        personal->setFirstName("");
        personal->setLastName("");
        personal->setGender("");
        personal->setBirthday("");
        personal->setPassword("");
        personal->setCity("");
        personal->setAddress("", "");
    } else {
         personal->loadJson(operation.body.c_str());
         loginInformation->loadJson(operation.body.c_str());
    }

    int success = dbAdministrator->addClient(personal, loginInformation, operation);
    std::ostringstream s;
    s << success;
    std::string success_parsed = s.str();

    Response* response = new Response();
    if (success == 0) {
        response->setContent("{\"registration\":\"OK\"}");
        response->setStatus(201);
    } else {
        if (success == 1) {
            response->setContent("{\"code\":" + success_parsed + ",\"message\":\"Client already exists.\"}");
            response->setStatus(500);
        } else {
            response->setContent("{\"code\":" + success_parsed + ",\"message\":\"There are empty fields.\"}");
            response->setStatus(500);
        }
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
    this->functions["PUT"] = put;
    this->functions["GET"] = get;
}

ProfilePersonal::~ProfilePersonal() {}

Response* ProfilePersonal::get(struct Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER); 

    Response* response = new Response();;
    if (dbAdministrator->rigthClient(email, token)) {
        response->setContent(dbAdministrator->getPersonal(email));
        response->setStatus(200);
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
    }

    return response;
}

Response* ProfilePersonal::put(struct Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    
    Personal *personal = new Personal();
    personal->loadJson(operation.body);
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER); 
    int success = dbAdministrator->uploadPersonal(email, token, personal);

    std::ostringstream s;
    s << success;
    std::string success_parsed = s.str();
    
    Response* response = new Response();
    if (success == 0) {
        response->setContent("");
        response->setStatus(200);
    } else if (success == 1) {
        response->setContent("{\"code\":" + success_parsed + ",\"message\":\"Don't upload.\"}");
        response->setStatus(500);
    }

    return response;
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
    this->functions["POST"] = post;
    this->functions["PUT"] = put;
    this->functions["GET"] = get;
}

ProfileSkills::~ProfileSkills() {}

Response* ProfileSkills::post(struct Message operation) {
    RequestParse *rp = new RequestParse();
    std::string mail = rp->extractEmail(operation.uri);
    Skills *skills = new Skills();
    skills->loadJson(operation.body);
    std::string category = skills->getCategory(0);
    std::cout<<category<<std::endl;
    Response *response = new Response();
    response->setStatus(201);
    response->setContent("");
    return response;
}

Response* ProfileSkills::put(struct Message operation) {

}

Response* ProfileSkills::get(struct Message operation) {

}



ProfilePhoto::ProfilePhoto() {
    // methods->push_back("GET");
}

ProfilePhoto::~ProfilePhoto() {}

Response* ProfilePhoto::get(struct Message operation) {
    std::cout << "Hola\n" << std::endl;
}
