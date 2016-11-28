// "Copyright 2016 <Jobify>"

#include "Attendant.h"
#include <string>
#include <map>
#include <sstream>
#include <vector>

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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
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
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
        Logger::getInstance().log(warn, "The client " + email +" was not logged out.");
    }
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
    Response* response = new Response();
    if (success == 0) {
        response->setContent("{\"registration\":\"OK\"}");
        response->setStatus(201);
        Logger::getInstance().log(info, "The client " + loginInformation->getEmail() +" was register.");
    } else {
        if (success == 1) {
            response->setContent("{\"code\":" + std::string(CLIENT_ALREADY_EXISTS) + ",\"message\":\"Client already exists.\"}");
            response->setStatus(500);
        } else {
            response->setContent("{\"code\":" + std::string(EMPTY_FIELDS) + ",\"message\":\"There are empty fields.\"}");
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
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    Response *response = new Response();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    if (dbAdministrator->existsClient(email)){
        int result = dbAdministrator->resetPassword(email);
        if (result == 0) {
            std::string credentials_parser = DataBase::getInstance().get(email);
            Credentials *credentials = new Credentials();
            credentials->loadJson(credentials_parser);
            std::string token = credentials->getToken();
            Authentication *auth = new Authentication();
            LoginInformation *loginInformation = new LoginInformation();
            bool rightDecode = auth->decode(token, loginInformation, credentials);   
            std::string password = loginInformation->getPassword();
            delete loginInformation;
            delete credentials;
            delete auth;    
            response->setContent("{\"password\":\"" + password + "\"}");
            response->setStatus(200);
            Logger::getInstance().log(info, "The client " + loginInformation->getEmail() +" was register.");
            return response;
        }
    }
    response->setContent("{\"code\":" + std::string(CLIENT_NOT_EXISTS) + ",\"message\":\"Client not exists.\"}");
    response->setStatus(500);
    delete dbAdministrator;
    return response;
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
    Response* response = new Response();
    if (success == 0) {
        response->setContent("");
        response->setStatus(201);
    } else if (success == 1) {
        response->setContent("{\"code\":" + std::string(COULD_NOT_POST) + ",\"message\":\"Could not post.\"}");
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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
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
        if (success >= 0) {
            response->setContent("");
            response->setStatus(201);
        } else if (success == -1) {
            response->setContent("{\"code\":" + std::string(NO_SOLICITUDE_SENT) + ",\"message\":\"User did not send solicitude.\"}");
            response->setStatus(500);
        }
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
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
        if (success >= 0) {
            response->setContent("");
            response->setStatus(204);
        } else if (success == -1) {
            response->setContent("{\"code\":" + std::string(NO_SOLICITUDE_SENT) + ",\"message\":\"User did not send solicitude.\"}");
            response->setStatus(500);
        }
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
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
        response->setContent(dbAdministrator->getProfilePersonal(email));
        response->setStatus(200);
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
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
    Response* response = new Response();
    if (success == 0) {
        response->setContent("");
        response->setStatus(200);
    } else if (success == 1) {
        response->setContent("{\"code\":" + std::string(COULD_NOT_PUT) + ",\"message\":\"Could not upload.\"}");
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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
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
    Response* response = new Response();
    if (success == 0) {
        response->setContent("");
        response->setStatus(200);
    } else if (success == 1) {
        response->setContent("{\"code\":" + std::string(COULD_NOT_PUT) + ",\"message\":\"Could not upload.\"}");
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
    Response* response = new Response();
    if (success == 0) {
        response->setContent("");
        response->setStatus(200);
    } else if (success == 1) {
        response->setContent("{\"code\":" + std::string(COULD_NOT_PUT) + ",\"message\":\"Could not upload.\"}");
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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
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
    Response* response = new Response();
    if (success == 0) {
        response->setContent("");
        response->setStatus(200);
    } else if (success == 1) {
        response->setContent("{\"code\":" + std::string(COULD_NOT_PUT) + ",\"message\":\"Could not upload.\"}");
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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
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
    Response* response = new Response();
    if (success == 0) {
        response->setContent("");
        response->setStatus(200);
    } else if (success == 1) {
        response->setContent("{\"code\":" + std::string(COULD_NOT_PUT) + ",\"message\":\"Could not upload.\"}");
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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
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
std::cout<<"Vote POST\n\n\n\n";
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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}

Response* Vote::erase(Message operation) {
std::cout<<"Vote DELETE\n\n\n\n";
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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    } */
    delete dbAdministrator;
    return response;
}

Facebook::Facebook() {
    this->functions["GET"] = get;
    this->functions["POST"] = post;
}

Facebook::~Facebook() {}

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

Response* Facebook::get(struct Message operation) {
    std::vector<std::string> urlVector = split(operation.uri, '/');
    std::string token = urlVector[urlVector.size() -1];

    std::string urlFacebook = "https://graph.facebook.com/v2.2";
    std::string query = "/me?fields=id%2Cname%2Cemail%2Cabout%2Cbirthday%2Ceducation%2Cfirst_name&access_token=";

    Request* request = new Request();
    Response* facebookResponse = request->Execute(urlFacebook + query + token);
    Response* response = new Response();
        response->setContent(facebookResponse->getContent());
        response->setStatus(200);

    return response;
}

// prueba envio mail
Response* Facebook::post(struct Message operation) {
    Mail* mail = new Mail();

    Response* response = new Response();
        response->setContent(mail->Send()->getContent());
        response->setStatus(200);

    return response;
}

// Para probar el envio de mensajeria desde un cliente
Firebase::Firebase() {
    this->functions["POST"] = post;
}

Firebase::~Firebase() {}

Response* Firebase::post(struct Message operation) {
    // std::string toTokenMAti
    // "eZrExMhfu-o:APA91bGJwLtfev7GkgvEEA-bS1aTFSvyupR7ieVGgMo2IqrUFgPlt-pPQtihviEp4n-aXMYNwvnNZEg6O_xX55fhi3MOwjpHOZbeSeQgCudSifFn37t-tn1bTq2c5F9oBm21m6v95Rsc";
    // std::string toTokenFacu
    // "ciEaT_zMcQ8:APA91bEJxZCLBTgk1DKQJl0TxVIy-2BLmWWoEpJ7fo00nxjq13f9MxuNnDnQZZa8hqjdmz733wFoz4Vgaa4eqHgz8JwJWnKrBYC3e1YrGKeL-gRmyoEkxn8qJNZh4W9fL7_w-pB31bdi";
    // token2 mati
    // f0KndaMXQko:APA91bHP6ezwP07EuL67MzlJXVf19rsr4lI2J2CmcGrDXiXkQqL0g00sjtjJyYEwvpwaix9-FduRZbQ2FHQ-l9kKSC62kKyOZ-dYfmKmmrizGN1pOOONBkauVjyOjGvTFmxIXgsu3FTP
    std::vector<std::string> urlVector = split(operation.uri, '/');
    std::string toToken = urlVector[urlVector.size() -1];

    FirebaseService* firebaseService = new FirebaseService();
    Response* firebaseResponse = firebaseService->SendNotification(toToken, operation.body);

    Response* response = new Response();
    response->setContent(firebaseResponse->getContent());
    response->setStatus(200);

    return response;
}

//  Para probar el enlace con el shared
Category::Category() {
    this->functions["POST"] = post;
    this->functions["GET"] = get;
}

Category::~Category() {}

Response* Category::post(struct Message operation) {
    rapidjson::Document document;
    rapidjson::ParseResult parseRes = document.Parse(operation.body.c_str());

    SharedService* shared = new SharedService();
    return shared->createCategory(document["name"].GetString(), document["description"].GetString());
}

Response* Category::get(struct Message operation) {
    SharedService* shared = new SharedService();
    return shared->listCategories();
}

Skill::Skill() {
    this->functions["POST"] = post;
    this->functions["GET"] = get;
}

Skill::~Skill() {}

Response* Skill::post(struct Message operation) {
    rapidjson::Document document;
    rapidjson::ParseResult parseRes = document.Parse(operation.body.c_str());

    SharedService* shared = new SharedService();
    return shared->createSkill(document["name"].GetString(),
        document["description"].GetString(), document["category"].GetString());
}

Response* Skill::get(struct Message operation) {
    SharedService* shared = new SharedService();
    return shared->listSkills();
}

JobPosition::JobPosition() {
    this->functions["POST"] = post;
    this->functions["GET"] = get;
}

JobPosition::~JobPosition() {}

Response* JobPosition::post(struct Message operation) {
    rapidjson::Document document;
    rapidjson::ParseResult parseRes = document.Parse(operation.body.c_str());

    SharedService* shared = new SharedService();
    return shared->createJobPosition(document["name"].GetString(),
        document["description"].GetString(), document["category"].GetString());
}

Response* JobPosition::get(struct Message operation) {
    SharedService* shared = new SharedService();
    return shared->listJobPositions();
}

Search::Search() {
    this->functions["GET"] = get;
}

Search::~Search() {}

Response* Search::get(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    double lat, lon, distance;
    std::string token, position, lat_str, lon_str, distance_str;
    std::vector<std::string> *skills = new std::vector<std::string>();
    loadParameters(operation.params, &token, &lat_str, &lon_str, &distance_str, &position, skills);
    std::vector<std::string> *ids = dbAdministrator->getAllIds();   
    Response* response = new Response();    
    Authentication *auth = new Authentication();
    LoginInformation *loginInformation = new LoginInformation();
    Credentials *credentials = new Credentials();
    bool rightDecode = auth->decode(token, loginInformation, credentials);
    delete auth;
    delete credentials;
    if (!rightDecode) {
       response->setContent("{\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
       return response;
    }

    bool isEmptyPosition = std::strcmp(position.c_str(), "") == 0;    
    std::vector<std::string> *ids_match_position = NULL;
    if (!isEmptyPosition) {
        std::cout << "SEARCH BY POSITION" << std::endl;
        ids_match_position = searchByPosition(ids, position);
    }
    bool isEmptySkills = skills->size() == 0;
    std::vector<std::string> *ids_match_skills = NULL;
    if (!isEmptySkills) {
        std::cout << "SEARCH BY SKILLS" << std::endl;
        ids_match_skills = searchBySkills(ids, skills);
    }
    bool isEmpthyDistance = std::strcmp(distance_str.c_str(), "") == 0; 
    if (isEmpthyDistance) {
        distance_str = "10.0";  // Set distance by default 10.0 kms 
    }
    bool isEmpthyLat = std::strcmp(lat_str.c_str(), "") == 0; 
    bool isEmpthyLon = std::strcmp(lon_str.c_str(), "") == 0; 
    bool isEmthySomeLatLon = isEmpthyLat || isEmpthyLon;
    std::vector<std::string> *ids_match_distance = NULL;
    if (!isEmthySomeLatLon) {
        lat = atof(lat_str.c_str());
        lon = atof(lon_str.c_str());
        distance = atof(distance_str.c_str());
        std::cout << "SEARCH BY LAT AND LONG" << std::endl;
        ids_match_distance = searchByDistance(ids,  lat, lon, distance);
    }
    
    std::vector<std::string> *ids_match_2 = NULL;
    std::vector<std::string> *ids_match_1 = NULL;
    std::vector<std::string> *ids_match = NULL;

    if (!isEmptyPosition && !isEmptySkills && !isEmthySomeLatLon) {
        ids_match_2 = intersection(ids_match_position, ids_match_skills);
        ids_match_1 = intersection(ids_match_2, ids_match_distance);
        ids_match = ids_match_1;
    } else if (!isEmptyPosition && !isEmptySkills ) {
        ids_match = intersection(ids_match_position, ids_match_skills);
    } else if (!isEmptyPosition && !isEmthySomeLatLon) {
        ids_match = intersection(ids_match_position, ids_match_distance);
    } else if (!isEmptySkills && !isEmthySomeLatLon) {
        ids_match = intersection(ids_match_skills, ids_match_distance);
    } else if (!isEmptyPosition) {
        ids_match = ids_match_position;
    } else if (!isEmptySkills) {
        ids_match = ids_match_skills;
    } else if (!isEmthySomeLatLon) {
        ids_match = ids_match_distance;
    } else {
        // If al the fields are empthy returning all users.
        ids_match = ids;
    }   

    std::string message = "{\"ids\":[";    
    if (ids_match->size() > 0) {
        for (int i = 0; i < ids_match->size(); i++){
            std::string id = (*ids_match)[i];
            std::string personal_str = dbAdministrator->getPersonal(id);
            Personal *personal = new Personal();
            personal->loadJson(personal_str);
            std::string picture_str = dbAdministrator->getPicture(id);
            Picture *picture = new Picture();
            picture->loadJson(picture_str);
            message += "{\"email\":\"" + id + "\"" + ",\"first_name\":" + "\"" + personal->getFirstName() + "\"" + ",\"last_name\":" + "\"" + personal->getLastName() + "\"" + ",\"thumbnail\":" + "\"" + picture->getPicture() + "\"}";
            if (i != (ids_match->size() - 1)) {
                message += ",";
            }      
            delete personal;
            delete picture;
        }
    }
    message += "]}";
    delete dbAdministrator;
    response->setContent(message);
    response->setStatus(200);
    return response;
}

std::vector<std::string>* Search::intersection(std::vector<std::string>* ids_match_position, std::vector<std::string>* ids_match_skills){
    std::vector<std::string>* ids_match = new std::vector<std::string>();
    for (int i = 0; i < ids_match_position->size(); i++){
        std::string id_1 = (*ids_match_position)[i];
        for (int j = 0; j < ids_match_skills->size(); j++){
            std::string id_2 = (*ids_match_skills)[j];
            bool match = strcmp(id_1.c_str(), id_2.c_str()) == 0;
            if (match){
                ids_match->push_back(id_1);
            }   
        }    
    }    
    return ids_match;
}

std::vector<std::string>* Search::searchBySkills(std::vector<std::string>* ids, std::vector<std::string>* skills_to_match){
    std::vector<std::string>* ids_match_skills = new std::vector<std::string>();
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    for (int i = 0; i < ids->size(); i++){
        std::string id = (*ids)[i];
//        std::cout << "USER: " << id << std::endl;
        // CHEQUEAR SI ESTE USUARIO MACHEA CON SKILLS, SI -> AGREGAR
        bool isAdding = false;
        std::string skills_parse = dbAdministrator->getSkills(id);
        Skills *skills = new Skills();
        skills->loadJson(skills_parse);
        // Number of categories (category -> skills)
        int number_of_skills = skills->getNumberOfSkills();
        for (int j = 0; j < number_of_skills; j++){
            std::string skills_string = skills->getSkills(j);
            std::vector<std::string> skills_vector = rp->split(skills_string, ",");
//            std::cout << "SKILLS: " << skills_string << std::endl;
            int count = 0;
            // Skills for category
            for (int k = 0; k < skills_vector.size(); k++){
                std::string skill = skills_vector[k];
                for (int k = 0; k < skills_to_match->size(); k++){   
                    bool match_skill = strcmp(skill.c_str(), (*skills_to_match)[k].c_str()) == 0;
                    if (match_skill){
                        ++count;
                    }
                }
            }
            bool match_with_all_skills = count == skills_to_match->size();
            if (match_with_all_skills && !isAdding) {
                isAdding  = true;
                ids_match_skills->push_back(id);
                std::cout << "MATCHEA: " << id << std::endl;
            }       
        }
        delete skills;
    }
    delete dbAdministrator;
    delete rp;    
    return ids_match_skills;
}

std::vector<std::string>* Search::searchByPosition(std::vector<std::string>* ids, std::string position){
    std::vector<std::string>* ids_match_position = new std::vector<std::string>();
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    for (int i=0; i < ids->size(); i++){
        std::string id = (*ids)[i];
        // CHEQUEAR SI ESTE USUARIO MACHEA CON POSITION, SI -> AGREGAR
        std::string expertise_parse = dbAdministrator->getExpertise(id);
        Expertise *expertises = new Expertise();
        expertises->loadJson(expertise_parse);
        int number_of_expertises = expertises->getNumberOfExpertises();
        for (int j=0; j < number_of_expertises; j++){
            bool match_position = strcmp(expertises->getPosition(j).c_str(), position.c_str()) == 0;
            if (match_position){
                ids_match_position->push_back(id);
                std::cout << "MATCHEA: " << id << std::endl;
            }
        }
        delete expertises;
    }
    delete dbAdministrator;
    return ids_match_position;
}

std::vector<std::string>* Search::searchByDistance(std::vector<std::string>* ids, double lat_initial, double lon_initial, double distance) {
    std::vector<std::string>* ids_match_distance = new std::vector<std::string>();
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    for (int i=0; i < ids->size(); i++){
        std::string id = (*ids)[i];
//        std::cout << "USER: " << id << std::endl;
        std::string personal_parse = dbAdministrator->getPersonal(id);
        Personal *personal = new Personal();
        personal->loadJson(personal_parse);
        double lat_final = atof(personal->getLat().c_str());    
        double lon_final = atof(personal->getLon().c_str());
        
//        std::cout << "LAT: " << lat_final << std::endl;
//        std::cout << "LON: " << lon_final << std::endl;

        double distance_calculated = calculateDistance(lat_initial, lon_initial, lat_final, lon_final);
        bool rightDistance = 0 <= distance_calculated && distance_calculated <= distance;
        std::cout << "distance between ("<< lat_initial << ", " <<lon_initial << ") and (" << lat_final << ", " << lon_final << ") is : " << distance_calculated << std::endl;   
        if (rightDistance) {
            std::cout << "MATCHEA: " << id << std::endl;            
            ids_match_distance->push_back(id);
        }

        delete personal;
    }
    delete dbAdministrator;
    return ids_match_distance;
}

void Search::loadParameters(std::string params, std::string *token, std::string *lat_str, std::string *lon_str, std::string *distance_str, std::string *position, std::vector<std::string> *skills){
    int pos_token = params.find("&token=");
    int pos_lat = params.find("&lat=");
    int pos_lon = params.find("&lon=");
    int pos_distance = params.find("distance=");
    int pos_position = params.find("&position=");
    int pos_skills = params.find("&skills=");
    *distance_str = params.substr(pos_distance + 9, pos_lat - pos_distance - 9 );
    *lat_str = params.substr(pos_lat + 5, pos_lon - distance_str->length() - 14);
    *lon_str = params.substr(pos_lon + 5, pos_position - distance_str->length() - lat_str->length() - 19);
    *position = params.substr(pos_position + 10, pos_skills - distance_str->length() - lat_str->length() - lon_str->length() - 29);
    std::string skills_parse = params.substr(pos_skills + 8, pos_token - distance_str->length() - lat_str->length() - lon_str->length() - position->length()- 37);
    *token = params.substr(pos_token + 7);
  
    *position = URLDecode(*position);
    skills_parse = URLDecode(skills_parse);

    RequestParse * rp = new RequestParse();
    *skills = rp->split(skills_parse, ",");
    delete rp;

    std::cout << "token : " << *token << std::endl;
    std::cout << "position : " << *position << std::endl;
    std::cout << "lat : " << *lat_str << std::endl;
    std::cout << "lon : " << *lon_str << std::endl;
    std::cout << "distance : " << *distance_str << std::endl;
    for (int i = 0; i < skills->size(); i++){
        std::cout << "skill " << i << " : " << (*skills)[i] << std::endl;
    }
}

std::string Search::URLDecode(std::string text){
    text = curl_unescape(text.c_str(), 0);
    RequestParse * rp = new RequestParse();
    std::vector<std::string> words =  rp->split(text, "+");  
    delete rp;
    std::string text_decoding = "";
    
    for (int i=0; i< words.size(); i++) {
        if (i == 0){
            text_decoding = words[i];
        } else {
            text_decoding = text_decoding + " " + words[i];
        }
    }
    return text_decoding;      
}

double Search::toRad(double degree) {
    double rad =  degree/180 * pi;
    return rad;
}

bool Search::rightLat(double lat) {
    return 0 <= abs(lat) && abs(lat) <= 90;
}

bool Search::rightLong(double lon) {
    return 0 <= abs(lon) && abs(lon) <= 180;
}

double Search::calculateDistance(double lat1, double long1, double lat2, double long2) {
    double dist = -1;
    if ( rightLat(lat1) && rightLat(lat2) && rightLong(long1) && rightLong(long2)) {
        dist = sin(toRad(lat1)) * sin(toRad(lat2)) + cos(toRad(lat1)) * cos(toRad(lat2)) * cos(toRad(long1 - long2));
        dist = acos(dist);
    //        dist = (6371 * pi * dist) / 180;
        //got dist in radian, no need to change back to degree and convert to rad again.
        dist = earth_radio * dist;
    } else {
        std::cout << "Incorrect parameters" << std::endl;
    }
    return dist;
}
