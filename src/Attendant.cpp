// "Copyright 2016 <Jobify>"

#include "Attendant.h"
#include <string>
#include <map>

Attendant::Attendant() {}

Attendant::~Attendant() {}

Response* Attendant::attend(Message operation) {
    Response* response = NULL;
    if (strcmp(operation.verb.c_str(), "DELETE") == 0) {
        response = this->functions["ERASE"](operation);
    } else {
        if (this->isMethodSupported(operation.verb)) {
            response = this->functions[operation.verb](operation);
        } else {
            Logger::getInstance().log(error, "Does not exist the request " + operation.verb + ".");
        }
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

Response* Login::post(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    LoginInformation *loginInformation = new LoginInformation();
    bool hasLoginFacebook = strcmp(operation.params.c_str(), "app=facebook") == 0;
    if (hasLoginFacebook) {
        // cargar datos de facebook
        loginInformation->setEmail("");
        loginInformation->setPassword("");
    } else {
        loginInformation->loadJson(operation.body.c_str());
    }
    Response* response = new Response();
    std::string email = loginInformation->getEmail();
    bool rightCredential = dbAdministrator->rightClient(loginInformation);
    if (rightCredential) {
        response->setContent(dbAdministrator->getPersonalLogin(email));
        response->setStatus(200);
        Logger::getInstance().log(info, "The client " + email +" was logged.");
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
       Logger::getInstance().log(warn, "The client " + email +" was not logged.");
    }
    return response;
}

Logout::Logout() {
    this->functions["ERASE"] = erase;
}

Logout::~Logout() {}

Response* Logout::erase(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightClient = dbAdministrator->rightClient(email, token);
    if (rightClient) {
        std::cout << "right client" << std::endl;
        Authentication *auth = new Authentication();
        LoginInformation *loginInformation = new LoginInformation();
        Credentials *credentials = new Credentials();
        bool rightDecode = auth->decode(token, loginInformation, credentials);
        if (rightDecode) {
            std::cout << "right decode" << std::endl;
            credentials->increaseIncrementalNumber(1);
            std::string email = loginInformation->getEmail();
            std::string password = loginInformation->getPassword();
            int incremental_number = credentials->getIncrementalNumber();
            std::string new_token = auth->encode(email, password, incremental_number);
            credentials->setToken(new_token);
            std::string new_credentials_parser = credentials->createJsonFile();
            DataBase::getInstance().erase(email);
            DataBase::getInstance().put(email, new_credentials_parser);
            response->setContent("");
            response->setStatus(200);
            return response;
            Logger::getInstance().log(info, "The client " + loginInformation->getEmail() +" was logged out.");
        }
    }
    response->setContent("{\"message\":\"Invalid credentials.\"}");
    response->setStatus(401);
    Logger::getInstance().log(warn, "The client " + email +" was not logged out.");
    return response;
}

Register::Register() {
    this->functions["POST"] = post;
}

Register::~Register() {}

Response* Register::post(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    Personal *personal = new Personal();
    LoginInformation *loginInformation = new LoginInformation();
    bool hasLoginFacebook = strcmp(operation.params.c_str(), "app=facebook") == 0;
    if (hasLoginFacebook) {
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
        Logger::getInstance().log(info, "The client " + loginInformation->getEmail() +" was register.");
    } else {
        if (success == 1) {
            response->setContent("{\"code\":" + success_parsed + ",\"message\":\"Client already exists.\"}");
            response->setStatus(500);
        } else {
            response->setContent("{\"code\":" + success_parsed + ",\"message\":\"There are empty fields.\"}");
            response->setStatus(500);
        }
        Logger::getInstance().log(warn, "The client " + loginInformation->getEmail() +" was not register.");
    }
    return response;
}




RecoveryPass::RecoveryPass() {
    this->functions["GET"] = get;
}

RecoveryPass::~RecoveryPass() {}

Response* RecoveryPass::get(Message operation) {
    std::cout << "Chau\n" << std::endl;
}



Contact::Contact() {
    this->functions["POST"] = post;
    this->functions["GET"] = get;
}

Contact::~Contact() {}

Response* Contact::post(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    int pos_date = operation.params.find("date=");
    int pos_email = operation.params.find("&email=");
    int pos_token = operation.params.find("&token=");
    std::string date = operation.params.substr(pos_date + 5, pos_email - 5);
    std::string contact_email = operation.params.substr(pos_email + 7, pos_token - 12 - date.length());
    std::string token = operation.params.substr(pos_token + 7);
    CURL *curl = curl_easy_init();
    int number[3];
    date = curl_easy_unescape(curl, date.c_str(), date.length(), number);
    contact_email = curl_easy_unescape(curl, contact_email.c_str(), contact_email.length(), number);
     Solicitude solicitude;
    solicitude.date = date;
    solicitude.email = contact_email;
    int success = dbAdministrator->addSolicitude(email, token, solicitude);
    delete dbAdministrator;
    std::ostringstream s;
    s << success;
    std::string success_parsed = s.str();
    Response* response = new Response();
    if (success == 0) {
        response->setContent("");
        response->setStatus(201);
    } else if (success == 1) {
        response->setContent("{\"code\":" + success_parsed + ",\"message\":\"Don't posted.\"}");
        response->setStatus(500);
    }
    return response;
}

Response* Contact::get(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        response->setContent(dbAdministrator->getSolicitudes(email));
        response->setStatus(200);
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}



Accept::Accept() {
    this->functions["POST"] = post;
}

Accept::~Accept() {}

Response* Accept::post(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    int pos_date = operation.params.find("date=");
    int pos_email = operation.params.find("&email=");
    int pos_token = operation.params.find("&token=");
    std::string date = operation.params.substr(pos_date + 5, pos_email - 5);
    std::string contact_email = operation.params.substr(pos_email + 7, pos_token - 12 - date.length());
    std::string token = operation.params.substr(pos_token + 7);
    CURL *curl = curl_easy_init();
    int number[3];
    date = curl_easy_unescape(curl, date.c_str(), date.length(), number);
    contact_email = curl_easy_unescape(curl, contact_email.c_str(), contact_email.length(), number);
     Solicitude solicitude;
    solicitude.date = date;
    solicitude.email = contact_email;
    bool rightCredential = dbAdministrator->rightClient(email, token);
    Response* response = new Response();
    if (rightCredential) {
        int success = dbAdministrator->addFriend(email, solicitude);
        delete dbAdministrator;
        std::ostringstream s;
        s << success;
        std::string success_parsed = s.str();
        if (success >= 0) {
            response->setContent("");
            response->setStatus(201);
        } else if (success == -1) {
            response->setContent("{\"code\":" + success_parsed + ",\"message\":\"User did not send solicitude.\"}");
            response->setStatus(500);
        }
    } else {
        response->setContent("{\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    return response;
}



Reject::Reject() {
    this->functions["ERASE"] = erase;
}

Reject::~Reject() {}

Response* Reject::erase(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    int pos_date = operation.params.find("date=");
    int pos_email = operation.params.find("&email=");
    int pos_token = operation.params.find("&token=");
    std::string date = operation.params.substr(pos_date + 5, pos_email - 5);
    std::string contact_email = operation.params.substr(pos_email + 7, pos_token - 12 - date.length());
    std::string token = operation.params.substr(pos_token + 7);
    CURL *curl = curl_easy_init();
    int number[3];
    date = curl_easy_unescape(curl, date.c_str(), date.length(), number);
    contact_email = curl_easy_unescape(curl, contact_email.c_str(), contact_email.length(), number);
     Solicitude solicitude;
    solicitude.date = date;
    solicitude.email = contact_email;
    bool rightCredential = dbAdministrator->rightClient(email, token);
    Response* response = new Response();
    if (rightCredential) {
        int success = dbAdministrator->removeSolicitude(email, solicitude);
        delete dbAdministrator;
        std::ostringstream s;
        s << success;
        std::string success_parsed = s.str();
        if (success >= 0) {
            response->setContent("");
            response->setStatus(204);
        } else if (success == -1) {
            response->setContent("{\"code\":" + success_parsed + ",\"message\":\"User did not send solicitude.\"}");
            response->setStatus(500);
        }
    } else {
        response->setContent("{\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    return response;
}


ProfilePersonal::ProfilePersonal() {
    this->functions["PUT"] = put;
    this->functions["GET"] = get;
}

ProfilePersonal::~ProfilePersonal() {}

Response* ProfilePersonal::get(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        response->setContent(dbAdministrator->getPersonal(email));
        response->setStatus(200);
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
    }
    return response;
}

Response* ProfilePersonal::put(Message operation) {
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
    this->functions["PUT"] = put;
    this->functions["GET"] = get;
}

ProfileSummary::~ProfileSummary() {}

Response* ProfileSummary::get(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        response->setContent(dbAdministrator->getSummary(email));
        response->setStatus(200);
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
    }
    return response;
}

Response* ProfileSummary::put(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    Summary *summary = new Summary();
    summary->loadJson(operation.body);
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    int success = dbAdministrator->uploadSummary(email, token, summary);
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



ProfileExpertise::ProfileExpertise() {
    this->functions["PUT"] = put;
    this->functions["GET"] = get;
}

ProfileExpertise::~ProfileExpertise() {}

Response* ProfileExpertise::put(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    Expertise *expertise = new Expertise();
    expertise->loadJson(operation.body);
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    int success = dbAdministrator->uploadExpertise(email, token, expertise);
    delete dbAdministrator;
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

Response* ProfileExpertise::get(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        response->setContent(dbAdministrator->getExpertise(email));
        response->setStatus(200);
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}



ProfileSkills::ProfileSkills() {
    this->functions["POST"] = post;
    this->functions["PUT"] = put;
    this->functions["GET"] = get;
}

ProfileSkills::~ProfileSkills() {}

Response* ProfileSkills::post(Message operation) {
    RequestParse *rp = new RequestParse();
    std::string mail = rp->extractEmail(operation.uri);
    delete rp;
    Skills *skills = new Skills();
    skills->loadJson(operation.body);
    std::string category = skills->getCategory(0);
    std::cout << category << std::endl;
    delete skills;
    Response *response = new Response();
    response->setStatus(201);
    response->setContent("");
    return response;
}

Response* ProfileSkills::put(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    Skills *skills = new Skills();
    skills->loadJson(operation.body);
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    int success = dbAdministrator->uploadSkills(email, token, skills);
    delete dbAdministrator;
    delete skills;
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

Response* ProfileSkills::get(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        response->setContent(dbAdministrator->getSkills(email));
        response->setStatus(200);
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}



ProfilePhoto::ProfilePhoto() {
    this->functions["PUT"] = put;
    this->functions["GET"] = get;
}

ProfilePhoto::~ProfilePhoto() {}

Response* ProfilePhoto::get(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        response->setContent(dbAdministrator->getPicture(email));
        response->setStatus(200);
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}

Response* ProfilePhoto::put(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();    
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    Picture *picture = new Picture();
    picture->loadJson(operation.body);
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    int success = dbAdministrator->uploadPicture(email, token, picture);
    delete dbAdministrator;
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



ProfileFriends::ProfileFriends() {
    this->functions["GET"] = get;
}

ProfileFriends::~ProfileFriends() {}

Response* ProfileFriends::get(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        response->setContent(dbAdministrator->getFriends(email));
        response->setStatus(200);
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}



Vote::Vote() {
    this->functions["POST"] = post;
    this->functions["ERASE"] = erase;
}

Vote::~Vote() {}

Response* Vote::post(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    int pos_email = operation.params.find("email=");
    int pos_token = operation.params.find("&token=");
    std::string email_to_vote = operation.params.substr(pos_email + 6, pos_token - 6);
    std::string token = operation.params.substr(pos_token + 7);
    CURL *curl = curl_easy_init();
    int number[3];
    email_to_vote = curl_easy_unescape(curl, email_to_vote.c_str(), email_to_vote.length(), number);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        dbAdministrator->vote(email, email_to_vote);
        response->setContent("");
        response->setStatus(201);
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}

Response* Vote::erase(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    int pos_email = operation.params.find("email=");
    int pos_token = operation.params.find("&token=");
    std::string email_to_unvote = operation.params.substr(pos_email + 6, pos_token - 6);
    std::string token = operation.params.substr(pos_token + 7);
    CURL *curl = curl_easy_init();
    int number[3];
    email_to_unvote = curl_easy_unescape(curl, email_to_unvote.c_str(), email_to_unvote.length(), number);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        dbAdministrator->unvote(email, email_to_unvote);
        response->setContent("");
        response->setStatus(204);
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}



ProfileOwnRecommendations::ProfileOwnRecommendations() {
    this->functions["GET"] = get;
}

ProfileOwnRecommendations::~ProfileOwnRecommendations() {}

Response* ProfileOwnRecommendations::get(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        response->setContent(dbAdministrator->getOwnRecommendations(email));
        response->setStatus(200);
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}



ProfileOthersRecommendations::ProfileOthersRecommendations() {
    this->functions["GET"] = get;
}

ProfileOthersRecommendations::~ProfileOthersRecommendations() {}

Response* ProfileOthersRecommendations::get(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        response->setContent(dbAdministrator->getOthersRecommendations(email));
        response->setStatus(200);
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}



MostPopularUsers::MostPopularUsers() {
    this->functions["GET"] = get;
}

MostPopularUsers::~MostPopularUsers() {}

Response* MostPopularUsers::get(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
        response->setContent(dbAdministrator->getMostPopularUsers());
        response->setStatus(200);
    /* bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        response->setContent(dbAdministrator->getMostPopularUsers());
        response->setStatus(200);
    } else {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
    } */
    delete dbAdministrator;
    return response;
}
