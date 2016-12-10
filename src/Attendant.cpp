// "Copyright 2016 <Jobify>"

#include "Attendant.h"
#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <utility>

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
    //bool hasLoginFacebook = strcmp(operation.params.c_str(), "app=facebook") == 0;
    /*if (hasLoginFacebook) {
        loginInformation->setEmail("");
        loginInformation->setPassword("");
    } else {*/
        loginInformation->loadJson(operation.body.c_str());
    //}
    Response* response = new Response();
    std::string email = loginInformation->getEmail();
    bool rightCredential = dbAdministrator->rightClient(loginInformation);
    if (rightCredential) {
        response->setContent(dbAdministrator->getPersonalLogin(email));
        response->setStatus(200);
        Logger::getInstance().log(info, "The client " + email +" was logged.");
    } else {
        if (dbAdministrator->checkPasswordIsNull(email)) {
            response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                                ",\"message\":\"Client has Facebook login.\"}");
            response->setStatus(400);
            Logger::getInstance().log(warn, "The client " + email + " was not logged.");
        } else {
            response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                                ",\"message\":\"Invalid credentials.\"}");
            response->setStatus(401);
            Logger::getInstance().log(warn, "The client " + email + " was not logged.");
        }
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
        Authentication *auth = new Authentication();
        LoginInformation *loginInformation = new LoginInformation();
        Credentials *credentials = new Credentials();
        bool rightDecode = auth->decode(token, loginInformation, credentials);
        if (rightDecode) {
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
            Logger::getInstance().log(info, "The client " + loginInformation->getEmail() + " was logged out.");
            return response;
        }
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                                    ",\"message\":\"Invalid credentials.\"}");
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
    /*if (hasLoginFacebook) {
        personal->setFirstName("");
        personal->setLastName("");
        personal->setGender("");
        personal->setBirthday("");
        personal->setPassword("");
        personal->setAddress("", "");
    } else {*/
        personal->loadJson(operation.body.c_str());
        loginInformation->loadJson(operation.body.c_str());
    //}
        Picture *picture = new Picture();
        picture->loadJson(operation.body.c_str());


    int success = dbAdministrator->addClient(personal, picture, loginInformation, operation);
    Response* response = new Response();
    if (success == 0) {
        response->setContent("{\"registration\":\"OK\"}");
        response->setStatus(201);
        Logger::getInstance().log(info, "The client " + loginInformation->getEmail() +" was register.");
    } else {
        if (success == 1) {
            if (hasLoginFacebook) { 
                response->setContent("{\"registration\":\"OK\"}");
                response->setStatus(201);
                Logger::getInstance().log(info, "The client " + loginInformation->getEmail() +" was register.");
            } else {
                response->setContent("{\"code\":" + std::string(CLIENT_ALREADY_EXISTS) + ",\"message\":\"Client already exists.\"}");
                response->setStatus(500);
            }
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
        if (!dbAdministrator->checkPasswordIsNull(email)) {
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
                Logger::getInstance().log(info, "The client " + loginInformation->getEmail() + 
                                                                            " has regenerated his password.");
                //return response;
            }
        } else {
            response->setContent("{\"code\":" + std::string(CLIENT_WITH_FACEBOOK) + ",\"message\":\"Client has Facebook login.\"}");
            response->setStatus(500);
            //return response;
        }
    } else {
        response->setContent("{\"code\":" + std::string(CLIENT_NOT_EXISTS) + ",\"message\":\"Client not exists.\"}");
        response->setStatus(500);
    }
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
 std::cout << "ANTES PARAMETER" << std::endl;    
    std::map<std::string, std::string>* parameters = rp->parserParameters(operation.params);
 std::cout << "DESPUES PARAMETER" << std::endl;    
    std::string date, contact_email, token;
    std::cout << "DATE: " << (*parameters)["date"] << std::endl;
    std::cout << "EMAIL: " << (*parameters)["email"] << std::endl;
    std::cout << "TOKEN: " << (*parameters)["token"] << std::endl;



    bool existDate = parameters->find("date") != parameters->end();
    if (existDate) {
        date = (*parameters)["date"];
    }
    bool exist_email = parameters->find("email") != parameters->end();
    if (exist_email) {
        contact_email = (*parameters)["email"];
    }
    bool existToken = parameters->find("token") != parameters->end();
    if (existToken) {
        token = (*parameters)["token"];
    }
    delete rp;

    std::cout << "DATE: " << date << std::endl;
    std::cout << "EMAIL: " << contact_email << std::endl;
    std::cout << "TOKEN: " << token << std::endl;
/*
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
    */

    Solicitude solicitude;
    solicitude.date = date;
    solicitude.email = email;
    int success = dbAdministrator->addSolicitude(contact_email, token, solicitude);
    delete dbAdministrator;
    Response* response = new Response();
    if (success == 0) {
        response->setContent("{\"message\":\"La solicitud de amistad ha sido enviada a " + contact_email + " con exito\"}" );
        response->setStatus(201);
        FirebaseService *firebase = new FirebaseService();
        firebase->SendNotification(token, "Solicitud de amistad", contact_email + "quiere contactarte");
        delete firebase;
    } else if (success == 1) {
        response->setContent("{\"code\":" + std::string(COULD_NOT_POST) + ",\"message\":\"Could not post.\"}");
        response->setStatus(500);
    } else if (success == 2) {
        response->setContent("{\"code\":" + std::string(COULD_NOT_POST) + ",\"message\":\"User does not exists.\"}");
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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                            ",\"message\":\"Invalid credentials.\"}");
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
 std::cout << "ANTES PARAMETER" << std::endl;    
    std::map<std::string, std::string>* parameters = rp->parserParameters(operation.params);
 std::cout << "DESPUES PARAMETER" << std::endl;    
    std::string contact_email, date, token;
    std::cout << "EMAIL: " << (*parameters)["email"] << std::endl;
    std::cout << "TOKEN: " << (*parameters)["token"] << std::endl;

    bool exist_date = parameters->find("date") != parameters->end();
    if (exist_date) {
        date = (*parameters)["date"];
    }
    bool exist_email = parameters->find("email") != parameters->end();
    if (exist_email) {
        contact_email = (*parameters)["email"];
    }
    bool existToken = parameters->find("token") != parameters->end();
    if (existToken) {
        token = (*parameters)["token"];
    }
    delete rp;


/*
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
*/
    Solicitude solicitude;
    solicitude.date = date;
    solicitude.email = contact_email;
    bool rightCredential = dbAdministrator->rightClient(email, token);
    Response* response = new Response();
    if (rightCredential) {
        int success = dbAdministrator->addFriend(email, solicitude);
        if (success >= 0) {
            response->setContent("{\"message\":\"La solicitud de amistad de " + contact_email + " ha sido aceptada con exito\"}" );
            response->setStatus(201);
            FirebaseService *firebase = new FirebaseService();
            std::string contact_token = dbAdministrator->getToken(contact_email);
            firebase->SendNotification(contact_token, "Aceptación de amistad", email + "te ha aceptado como amigo");
            delete firebase;
        } else if (success == -1) {
            response->setContent("{\"code\":" + std::string(NO_SOLICITUDE_SENT) +
                                ",\"message\":\"User did not send solicitude.\"}");
            response->setStatus(500);
        }
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                            ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    delete dbAdministrator;
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
 std::cout << "ANTES PARAMETER" << std::endl;    
    std::map<std::string, std::string>* parameters = rp->parserParameters(operation.params);
 std::cout << "DESPUES PARAMETER" << std::endl;    
    std::string contact_email, date, token;
    std::cout << "EMAIL: " << (*parameters)["email"] << std::endl;
    std::cout << "TOKEN: " << (*parameters)["token"] << std::endl;

    bool exist_date = parameters->find("date") != parameters->end();
    if (exist_date) {
        date = (*parameters)["date"];
    }
    bool exist_email = parameters->find("email") != parameters->end();
    if (exist_email) {
        contact_email = (*parameters)["email"];
    }
    bool existToken = parameters->find("token") != parameters->end();
    if (existToken) {
        token = (*parameters)["token"];
    }
    delete rp;


/*
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
*/
     Solicitude solicitude;
    solicitude.date = date;
    solicitude.email = contact_email;
    bool rightCredential = dbAdministrator->rightClient(email, token);
    Response* response = new Response();
    if (rightCredential) {
        int success = dbAdministrator->removeSolicitude(email, solicitude);
        delete dbAdministrator;
        if (success >= 0) {
            response->setContent("{\"message\":\"La solicitud de amistad de " + contact_email + " ha sido rechazada con exito\"}" );
            response->setStatus(204);
        } else if (success == -1) {
            response->setContent("{\"code\":" + std::string(NO_SOLICITUDE_SENT) +
                                ",\"message\":\"User did not send solicitude.\"}");
            response->setStatus(500);
        }
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                            ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    return response;
}


ProfilePersonal::ProfilePersonal() {
    this->functions["POST"] = post;
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
    if (dbAdministrator->getEmailFromToken(token) == email) {
        bool rightCredentials = dbAdministrator->rightClient(email, token);
        if (rightCredentials) {
            response->setContent(dbAdministrator->getProfilePersonal(email));
            response->setStatus(200);
        } else {
            response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                ",\"message\":\"Invalid credentials.\"}");
            response->setStatus(401);
        }
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}

Response* ProfilePersonal::putAndPost(Message operation, int status_ok, std::string error_code) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    Personal *personal = new Personal();
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    int success;
    if (operation.verb == "POST") {
        success = dbAdministrator->createPersonal(email, token, personal);
    } else {
        personal->loadJson(operation.body);
        success = dbAdministrator->uploadPersonal(email, token, personal);
    }
    Response* response = new Response();
    if (success == 0) {
        response->setContent("");
        response->setStatus(status_ok);
    } else if (success == 1) {
        response->setContent("{\"code\":" + error_code + ",\"message\":\"Could not upload.\"}");
        response->setStatus(500);
    }
    return response;
}

Response* ProfilePersonal::put(Message operation) {
    return putAndPost(operation, 200, std::string(COULD_NOT_PUT));
}

Response* ProfilePersonal::post(Message operation) {
    return putAndPost(operation, 201, std::string(COULD_NOT_POST));
}


ProfileSummary::ProfileSummary() {
    this->functions["POST"] = post;
    this->functions["PUT"] = put;
    this->functions["GET"] = get;
    this->functions["ERASE"] = erase;
}

ProfileSummary::~ProfileSummary() {}

Response* ProfileSummary::erase(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        dbAdministrator->deleteSummary(email);
        response->setContent("");
        response->setStatus(204);
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                            ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}

Response* ProfileSummary::get(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (dbAdministrator->getEmailFromToken(token) == email) {
        if (rightCredentials) {
            response->setContent(dbAdministrator->getSummary(email));
            response->setStatus(200);
            Logger::getInstance().log(info, "Get summary of " + email + " is OK");
        } else {
            response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                ",\"message\":\"Invalid credentials.\"}");
            response->setStatus(401);
            Logger::getInstance().log(warn, "Get summary of " + email + " is not OK for credentials");
        }
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}

Response* ProfileSummary::putAndPost(Message operation, int status_ok, std::string error_code) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    Summary *summary = new Summary();
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);

    int success;
    if (operation.verb == "POST") {
        success = dbAdministrator->createSummary(email, token, summary);
    } else {
        summary->loadJson(operation.body);
        success = dbAdministrator->uploadSummary(email, token, summary);
    }

    Response* response = new Response();
    if (success == 0) {
        response->setContent("");
        response->setStatus(status_ok);
    } else if (success == 1) {
        response->setContent("{\"code\":" + error_code + ",\"message\":\"Could not upload.\"}");
        response->setStatus(500);
    }
    return response;
}

Response* ProfileSummary::put(Message operation) {
    return putAndPost(operation, 200, std::string(COULD_NOT_PUT));
}

Response* ProfileSummary::post(Message operation) {
    return putAndPost(operation, 201, std::string(COULD_NOT_POST));
}



ProfileExpertise::ProfileExpertise() {
    this->functions["PUT"] = put;
    this->functions["GET"] = get;
    this->functions["POST"] = post;
    this->functions["ERASE"] = erase;
}

ProfileExpertise::~ProfileExpertise() {}

Response* ProfileExpertise::erase(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        dbAdministrator->deleteExpertises(email);
        response->setContent("");
        response->setStatus(204);
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                            ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}

Response* ProfileExpertise::putAndPost(Message operation, int status_ok, std::string error_code) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    Expertise *expertise = new Expertise();
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    int success;
    if (operation.verb == "POST") {
        success = dbAdministrator->createExpertise(email, token, expertise);
    } else {
        expertise->loadJson(operation.body);
        success = dbAdministrator->uploadExpertise(email, token, expertise);
    }
    delete dbAdministrator;
    Response* response = new Response();
    if (success == 0) {
        response->setContent("");
        response->setStatus(status_ok);
    } else if (success == 1) {
        response->setContent("{\"code\":" + error_code + ",\"message\":\"Could not upload.\"}");
        response->setStatus(500);
    }
    return response;
}

Response* ProfileExpertise::put(Message operation) {
    return putAndPost(operation, 200, std::string(COULD_NOT_PUT));
}

Response* ProfileExpertise::post(Message operation) {
    return putAndPost(operation, 201, std::string(COULD_NOT_POST));
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
    if (dbAdministrator->getEmailFromToken(token) == email) {
        if (rightCredentials) {
            response->setContent(dbAdministrator->getExpertise(email));
            response->setStatus(200);
        } else {
            response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                ",\"message\":\"Invalid credentials.\"}");
            response->setStatus(401);
        }
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}



ProfileSkills::ProfileSkills() {
    this->functions["POST"] = post;
    this->functions["PUT"] = put;
    this->functions["GET"] = get;
    this->functions["ERASE"] = erase;
}

ProfileSkills::~ProfileSkills() {}

Response* ProfileSkills::erase(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        dbAdministrator->deleteSkills(email);
        response->setContent("");
        response->setStatus(204);
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                            ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}

Response* ProfileSkills::putAndPost(Message operation, int status_ok, std::string error_code) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    Skills *skills = new Skills();
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);

    int success;
    if (operation.verb == "POST") {
        success = dbAdministrator->createSkills(email, token, skills);
    } else {
        skills->loadJson(operation.body);
        success = dbAdministrator->uploadSkills(email, token, skills);
    }

    delete dbAdministrator;
    delete skills;
    Response* response = new Response();
    if (success == 0) {
        response->setContent("");
        response->setStatus(status_ok);
    } else if (success == 1) {
        response->setContent("{\"code\":" + error_code + ",\"message\":\"Could not upload.\"}");
        response->setStatus(500);
    }
    return response;
}

Response* ProfileSkills::post(Message operation) {
    return putAndPost(operation, 201, std::string(COULD_NOT_POST));
}

Response* ProfileSkills::put(Message operation) {
    return putAndPost(operation, 200, std::string(COULD_NOT_PUT));
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
    if (dbAdministrator->getEmailFromToken(token) == email) {
        if (rightCredentials) {
            response->setContent(dbAdministrator->getSkills(email));
            response->setStatus(200);
        } else {
            response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                ",\"message\":\"Invalid credentials.\"}");
            response->setStatus(401);
        }
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}



ProfilePhoto::ProfilePhoto() {
    this->functions["POST"] = post;
    this->functions["PUT"] = put;
    this->functions["GET"] = get;
    this->functions["ERASE"] = erase;
}

ProfilePhoto::~ProfilePhoto() {}

Response* ProfilePhoto::erase(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        dbAdministrator->deletePicture(email);
        response->setContent("");
        response->setStatus(204);
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                            ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}

Response* ProfilePhoto::get(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (dbAdministrator->getEmailFromToken(token) == email) {
        if (rightCredentials) {
            response->setContent(dbAdministrator->getPicture(email));
            response->setStatus(200);
        } else {
            response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                ",\"message\":\"Invalid credentials.\"}");
            response->setStatus(401);
        }
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}

Response* ProfilePhoto::putAndPost(Message operation, int status_ok, std::string error_code) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    Picture *picture = new Picture();
    const int SIZE_NAME_PARAMETER = 6;
    std::string token = operation.params.substr(SIZE_NAME_PARAMETER);

    int success;
    if (operation.verb == "POST") {
        success = dbAdministrator->createPicture(email, token, picture);
    } else {
        picture->loadJson(operation.body);
        success = dbAdministrator->uploadPicture(email, token, picture);
    }

    delete dbAdministrator;
    Response* response = new Response();
    if (success == 0) {
        response->setContent("");
        response->setStatus(status_ok);
    } else if (success == 1) {
        response->setContent("{\"code\":" + error_code + ",\"message\":\"Could not upload.\"}");
        response->setStatus(500);
    }
    return response;
}

Response* ProfilePhoto::put(Message operation) {
    return putAndPost(operation, 200, std::string(COULD_NOT_PUT));
}

Response* ProfilePhoto::post(Message operation) {
    return putAndPost(operation, 201, std::string(COULD_NOT_POST));
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
        std::string friends_parse = dbAdministrator->getFriends(email);
        Friends *friends = new Friends();
        friends->loadJson(friends_parse);
        std::string message = "{\"friends\":[";
        for (int i = 0; i < friends->getNumberOfContacts(); i++) {
            std::string friend_email = friends->getContactAt(i);
            std::string personal_str = dbAdministrator->getPersonal(friend_email);
            Personal *personal = new Personal();
            personal->loadJson(personal_str);
            std::string picture_str = dbAdministrator->getPicture(friend_email);
            Picture *picture = new Picture();
            picture->loadJson(picture_str);
            OthersRecommendations *others_recommendations = new OthersRecommendations();
            std::string others_recomendations_parse = dbAdministrator->getOthersRecommendations(friend_email);
            others_recommendations->loadJson(others_recomendations_parse);
            int vote = others_recommendations->getNumberOfContacts();
            delete others_recommendations;
            std::ostringstream vote_str;
            vote_str << vote;

            //TODO
            OwnRecommendations *own_recommendations = new OwnRecommendations();
            std::string own_recommendations_parse = dbAdministrator->getOwnRecommendations(email);
            own_recommendations->loadJson(own_recommendations_parse);
            std::string voted_by_me;
            if (own_recommendations->search(friend_email) != -1) {
                voted_by_me = "true";
            } else {
                voted_by_me = "false";
            }

            message += "{\"email\":\"" + email + "\"" + ",\"first_name\":" + "\"" + personal->getFirstName() +
                        "\"" + ",\"last_name\":" + "\"" + personal->getLastName() + "\"" + ",\"votes\":" +
                        vote_str.str() + ",\"thumbnail\":" + "\"" + picture->getPicture() +
                        "\",\"voted_by_me\":" + voted_by_me + "}";
            if (i != (friends->getNumberOfContacts() - 1)) {
                message += ",";
            }
        }
        message += "]}";
        response->setContent(message);
        response->setStatus(200);
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                                        ",\"message\":\"Invalid credentials.\"}");
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
 std::cout << "ANTES PARAMETER" << std::endl;    
    std::map<std::string, std::string>* parameters = rp->parserParameters(operation.params);
 std::cout << "DESPUES PARAMETER" << std::endl;    
    std::string email_to_vote, token;
    std::cout << "EMAIL: " << (*parameters)["email"] << std::endl;
    std::cout << "TOKEN: " << (*parameters)["token"] << std::endl;

    bool exist_email = parameters->find("email") != parameters->end();
    if (exist_email) {
        email_to_vote = (*parameters)["email"];
    }
    bool existToken = parameters->find("token") != parameters->end();
    if (existToken) {
        token = (*parameters)["token"];
    }
    delete rp;

/*
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    int pos_email = operation.params.find("email=");
    int pos_token = operation.params.find("&token=");
    std::string email_to_vote = operation.params.substr(pos_email + 6, pos_token - 6);
    std::string token = operation.params.substr(pos_token + 7);
    CURL *curl = curl_easy_init();
    int number[3];
    email_to_vote = curl_easy_unescape(curl, email_to_vote.c_str(), email_to_vote.length(), number);

*/

    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        dbAdministrator->vote(email, email_to_vote);
        response->setContent("");
        response->setStatus(201);
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                                    ",\"message\":\"Invalid credentials.\"}");
        response->setStatus(401);
    }
    delete dbAdministrator;
    return response;
}

Response* Vote::erase(Message operation) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();

    std::string email = rp->extractEmail(operation.uri);
 std::cout << "ANTES PARAMETER" << std::endl;    
    std::map<std::string, std::string>* parameters = rp->parserParameters(operation.params);
 std::cout << "DESPUES PARAMETER" << std::endl;    
    std::string email_to_unvote, token;
    std::cout << "EMAIL: " << (*parameters)["email"] << std::endl;
    std::cout << "TOKEN: " << (*parameters)["token"] << std::endl;

    bool exist_email = parameters->find("email") != parameters->end();
    if (exist_email) {
        email_to_unvote = (*parameters)["email"];
    }
    bool existToken = parameters->find("token") != parameters->end();
    if (existToken) {
        token = (*parameters)["token"];
    }
    delete rp;

/*
    std::string email = rp->extractEmail(operation.uri);
    delete rp;
    int pos_email = operation.params.find("email=");
    int pos_token = operation.params.find("&token=");
    std::string email_to_unvote = operation.params.substr(pos_email + 6, pos_token - 6);
    std::string token = operation.params.substr(pos_token + 7);
    CURL *curl = curl_easy_init();
    int number[3];
    email_to_unvote = curl_easy_unescape(curl, email_to_unvote.c_str(), email_to_unvote.length(), number);
*/
    Response* response = new Response();
    bool rightCredentials = dbAdministrator->rightClient(email, token);
    if (rightCredentials) {
        dbAdministrator->unvote(email, email_to_unvote);
        response->setContent("");
        response->setStatus(204);
    } else {
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                                ",\"message\":\"Invalid credentials.\"}");
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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                                    ",\"message\":\"Invalid credentials.\"}");
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
        response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) +
                                         ",\"message\":\"Invalid credentials.\"}");
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

//  Prueba envio mail
Response* Facebook::post(struct Message operation) {
    Mail* mail = new Mail();

    Response* response = new Response();
        response->setContent(mail->Send()->getContent());
        response->setStatus(200);

    return response;
}

//  Para probar el envio de mensajeria desde un cliente
Firebase::Firebase() {
    this->functions["POST"] = post;
}

Firebase::~Firebase() {}

Response* Firebase::post(struct Message operation) {
    /*  std::vector<std::string> urlVector = split(operation.uri, '/');
    std::string toToken = urlVector[urlVector.size() -1];

    FirebaseService* firebaseService = new FirebaseService();
    Response* firebaseResponse = firebaseService->SendNotification(toToken, operation.body);

    Response* response = new Response();
    response->setContent(firebaseResponse->getContent());
    response->setStatus(200);

    return response;  */
    return NULL;
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
    RequestParse *rp = new RequestParse();
    double lat, lon, distance;
    int offset, limit;
    std::string token = "";
    std::string position = "";
    std::string lat_str = "";
    std::string lon_str = "";
    std::string distance_str = "";
    std::string limit_str = "";
    std::string offset_str = "";
    std::vector<std::string> *skills = new std::vector<std::string>();
    loadParameters(operation.params, &token, &lat_str, &lon_str, &distance_str, &position,
                   &limit_str, &offset_str, skills);


    //TODO
    LoginInformation *loginInformation = new LoginInformation();
    loginInformation->loadJson(operation.body.c_str());
    std::string email = loginInformation->getEmail();
    std::vector<std::string> *ids = dbAdministrator->getAllIds(email);


    Response* response = new Response();
    Authentication *auth = new Authentication();
    //LoginInformation *loginInformation = new LoginInformation();
    Credentials *credentials = new Credentials();
    bool rightDecode = auth->decode(token, loginInformation, credentials);
    delete auth;
    delete credentials;
    if (!rightDecode) {
       response->setContent("{\"code\":" + std::string(INVALID_CREDENTIALS) + ",\"message\":\"Invalid credentials.\"}");
       response->setStatus(401);
       return response;
    }
    bool isEmpthyOffset = std::strcmp(offset_str.c_str(), "") == 0;
    bool isEmpthyLimit = std::strcmp(limit_str.c_str(), "") == 0;
    if (isEmpthyOffset) {
        offset_str = OFFSET_DEFAULT;
    }
    offset = atof(offset_str.c_str());
    if (isEmpthyLimit) {
        limit_str = LIMIT_DEFAULT;
    }
    limit = atof(limit_str.c_str());
    bool isEmptyPosition = std::strcmp(position.c_str(), "") == 0;
    std::vector<std::string> *ids_match_position = NULL;
    if (!isEmptyPosition) {
        ids_match_position = searchByPosition(ids, position);
    }
    bool isEmptySkills = true;
    if (skills != NULL) {
        isEmptySkills = skills->size() == 0;
    }
    std::vector<std::string> *ids_match_skills = NULL;
    if (!isEmptySkills) {
        ids_match_skills = searchBySkills(ids, skills);
    }
    bool isEmpthyDistance = std::strcmp(distance_str.c_str(), "") == 0;
    if (isEmpthyDistance) {
        distance_str = "10.0";  //  Set distance by default 10.0 kms
    }
    bool isEmpthyLat = std::strcmp(lat_str.c_str(), "") == 0;
    bool isEmpthyLon = std::strcmp(lon_str.c_str(), "") == 0;
    bool isEmthySomeLatLon = isEmpthyLat || isEmpthyLon;
    std::vector<std::string> *ids_match_distance = NULL;
    std::map<std::string, std::string> *ids_match_distance_map = NULL;
    if (!isEmthySomeLatLon) {
        lat = atof(lat_str.c_str());
        lon = atof(lon_str.c_str());
        distance = atof(distance_str.c_str());
        ids_match_distance_map = searchByDistance(ids,  lat, lon, distance);
        if (ids_match_distance_map != NULL) {
            ids_match_distance = getKeys(ids_match_distance_map);
        }
    }
    std::vector<std::string> *ids_match_2 = NULL;
    std::vector<std::string> *ids_match_1 = NULL;
    std::vector<std::string> *ids_match = NULL;

    if (!isEmptyPosition && !isEmptySkills && !isEmthySomeLatLon) {
        ids_match_2 = intersection(ids_match_position, ids_match_skills);
        ids_match_1 = intersection(ids_match_2, ids_match_distance);
        ids_match = ids_match_1;
    } else if (!isEmptyPosition && !isEmptySkills) {
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
        //  If all the fields are empthy returning all users.
        ids_match = ids;
    }
    std::string message = generateMessage(ids_match, ids_match_distance_map, offset, limit);
    delete dbAdministrator;
    delete rp;
    response->setContent(message);
    response->setStatus(200);
    return response;
}

std::string Search::generateMessage(std::vector<std::string>* ids_match, std::map<std::string,
                                            std::string> *ids_match_distance_map, int offset, int limit) {
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    std::string results = "\"results\":[";
    if (ids_match->size() > 0) {
        for (int i = offset; i < ids_match->size() && i < (offset+limit); i++) {
            std::string id = (*ids_match)[i];
            std::string distance = "";
            if (ids_match_distance_map != NULL) {
                bool existId = ids_match_distance_map->find(id) != ids_match_distance_map->end();
                if (existId) {
                    distance = (*ids_match_distance_map)[id];
                }
            }
            std::string personal_str = dbAdministrator->getPersonal(id);
            Personal *personal = new Personal();
            personal->loadJson(personal_str);
            std::string picture_str = dbAdministrator->getPicture(id);
            Picture *picture = new Picture();
            picture->loadJson(picture_str);

            OthersRecommendations *others_recommendations = new OthersRecommendations();
            std::string others_recomendations_parse = dbAdministrator->getOthersRecommendations(id);
            others_recommendations->loadJson(others_recomendations_parse);
            int vote = others_recommendations->getNumberOfContacts();
            delete others_recommendations;
            std::ostringstream vote_str;
            vote_str << vote;
            results += "{\"email\":\"" + id + "\"" + ",\"first_name\":" + "\"" + personal->getFirstName() +
                        "\"" + ",\"last_name\":" + "\"" + personal->getLastName() + "\"" + ",\"distance\":" +
                        "\"" + distance + "\"" + ",\"votes\":" + vote_str.str() + ",\"thumbnail\":" + "\"" +
                        picture->getPicture() + "\"}";
            if ((i != (ids_match->size() - 1)) && (i != (offset + limit - 1))) {
                results += ",";
            }
            delete personal;
            delete picture;
        }
    }
    results += "]";
    std::ostringstream match_size_str;
    match_size_str << ids_match->size();
    std::ostringstream offset_str;
    offset_str << offset;
    std::string paging = "\"paging\":{\"total\":" + match_size_str.str() + ",\"offset\":" + offset_str.str() + "},";
    std::string message = "{" + paging + results + "}";
    delete dbAdministrator;
    return message;
}

std::vector<std::string>* Search::intersection(std::vector<std::string>* ids_match_position,
                                                                std::vector<std::string>* ids_match_skills) {
    std::vector<std::string>* ids_match = new std::vector<std::string>();
    for (int i = 0; i < ids_match_position->size(); i++) {
        std::string id_1 = (*ids_match_position)[i];
        for (int j = 0; j < ids_match_skills->size(); j++) {
            std::string id_2 = (*ids_match_skills)[j];
            bool match = strcmp(id_1.c_str(), id_2.c_str()) == 0;
            if (match) {
                ids_match->push_back(id_1);
            }
        }
    }
    return ids_match;
}

std::vector<std::string>* Search::searchBySkills(std::vector<std::string>* ids,
                                                    std::vector<std::string>* skills_to_match) {
    std::vector<std::string>* ids_match_skills = new std::vector<std::string>();
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    RequestParse *rp = new RequestParse();
    for (int i = 0; i < ids->size(); i++) {
        std::string id = (*ids)[i];
        bool isAdding = false;
        std::string skills_parse = dbAdministrator->getSkills(id);
        Skills *skills = new Skills();
        skills->loadJson(skills_parse);
        int number_of_skills = skills->getNumberOfSkills();
        for (int j = 0; j < number_of_skills; j++) {
            std::string skills_string = skills->getSkills(j);
            std::vector<std::string> skills_vector = rp->split(skills_string, ",");
            int count = 0;
            for (int k = 0; k < skills_vector.size(); k++) {
                std::string skill = skills_vector[k];
                for (int k = 0; k < skills_to_match->size(); k++) {
                    bool match_skill = strcmp(skill.c_str(), (*skills_to_match)[k].c_str()) == 0;
                    if (match_skill) {
                        ++count;
                    }
                }
            }
            bool match_with_all_skills = count == skills_to_match->size();
            if (match_with_all_skills && !isAdding) {
                isAdding  = true;
                ids_match_skills->push_back(id);
            }
        }
        delete skills;
    }
    delete dbAdministrator;
    delete rp;
    return ids_match_skills;
}

std::vector<std::string>* Search::searchByPosition(std::vector<std::string>* ids, std::string position) {
    std::vector<std::string>* ids_match_position = new std::vector<std::string>();
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    for (int i=0; i < ids->size(); i++) {
        std::string id = (*ids)[i];
        std::string expertise_parse = dbAdministrator->getExpertise(id);
        Expertise *expertises = new Expertise();
        expertises->loadJson(expertise_parse);
        int number_of_expertises = expertises->getNumberOfExpertises();
        for (int j=0; j < number_of_expertises; j++) {
            bool match_position = strcmp(expertises->getPosition(j).c_str(), position.c_str()) == 0;
            if (match_position) {
                ids_match_position->push_back(id);
            }
        }
        delete expertises;
    }
    delete dbAdministrator;
    return ids_match_position;
}

std::map<std::string, std::string>* Search::searchByDistance(std::vector<std::string>* ids,
                                                double lat_initial, double lon_initial, double distance) {
    std::map<std::string, std::string>* ids_match_distance = new std::map<std::string, std::string>();
    DataBaseAdministrator *dbAdministrator = new DataBaseAdministrator();
    for (int i=0; i < ids->size(); i++) {
        std::string id = (*ids)[i];
        std::string personal_parse = dbAdministrator->getPersonal(id);
        Personal *personal = new Personal();
        personal->loadJson(personal_parse);
        double lat_final = atof(personal->getLat().c_str());
        double lon_final = atof(personal->getLon().c_str());

        double distance_calculated = calculateDistance(lat_initial, lon_initial, lat_final, lon_final);
        bool rightDistance = 0 <= distance_calculated && distance_calculated <= distance;
        if (rightDistance) {
            std::ostringstream distance;
            distance << distance_calculated;
            ids_match_distance->insert(std::pair<std::string, std::string>(id, distance.str()));
        }

        delete personal;
    }
    delete dbAdministrator;
    return ids_match_distance;
}

void Search::loadParameters(std::string params, std::string *token, std::string *lat_str, std::string *lon_str,
                std::string *distance_str, std::string *position, std::string *limit_str, std::string *offset_str,
                                                                                std::vector<std::string> *skills) {
    //std::map<std::string, std::string>* parameters = new std::map<std::string, std::string>();
    RequestParse * rp = new RequestParse();



    std::map<std::string, std::string>* parameters = rp->parserParameters(params);


    /*
    std::vector<std::string> parameters_vector = rp->split(params, "&");
    for (int i = 0; i < parameters_vector.size(); i++) {
        std::string a_parameter = parameters_vector[i];
        std::vector<std::string> key_value = rp->split(a_parameter, "=");
        bool isValue = key_value.size() > 1;
        std::string value = "";
        if (isValue) {
            value =  URLDecode(key_value[1]);
        }
        parameters->insert(std::pair<std::string, std::string>(key_value[0], value));
    }
    */
    bool existSkills = parameters->find("skills") != parameters->end();
    if (existSkills) {
        *skills = rp->split((*parameters)["skills"], ",");
    }
    bool existLon = parameters->find("lon") != parameters->end();
    if (existLon) {
        *lon_str = (*parameters)["lon"];
    }
    bool existLat = parameters->find("lat") != parameters->end();
    if (existLat) {
        *lat_str = (*parameters)["lat"];
    }
    bool existDistance = parameters->find("distance") != parameters->end();
    if (existLat) {
        *distance_str = (*parameters)["distance"];
    }
    bool existPosition = parameters->find("position") != parameters->end();
    if (existPosition) {
        *position = (*parameters)["position"];
    }
    bool existToken = parameters->find("token") != parameters->end();
    if (existToken) {
        *token = (*parameters)["token"];
    }
    bool existLimit = parameters->find("limit") != parameters->end();
    if (existLimit) {
        *limit_str = (*parameters)["limit"];
    }
    bool existOffset = parameters->find("offset") != parameters->end();
    if (existToken) {
        *offset_str = (*parameters)["offset"];
    }
    delete rp;
}

std::string Search::URLDecode(std::string text) {
    text = curl_unescape(text.c_str(), 0);
    RequestParse * rp = new RequestParse();
    std::vector<std::string> words = rp->split(text, "+");
    delete rp;
    std::string text_decoding = "";

    for (int i=0; i< words.size(); i++) {
        if (i == 0) {
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
    if (rightLat(lat1) && rightLat(lat2) && rightLong(long1) && rightLong(long2)) {
        dist = sin(toRad(lat1)) * sin(toRad(lat2)) + cos(toRad(lat1)) * cos(toRad(lat2)) * cos(toRad(long1 - long2));
        dist = acos(dist);
        dist = earth_radio * dist;
    } else {
    }
    return dist;
}

std::vector<std::string>* Search::getKeys(std::map<std::string, std::string>* m) {
    std::vector<std::string>* v = new std::vector<std::string>();
    for (std::map<std::string, std::string>::iterator it = m->begin(); it != m->end(); ++it) {
      v->push_back(it->first);
    }
    return v;
}
