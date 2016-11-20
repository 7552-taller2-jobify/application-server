// "Copyright 2016 <Jobify>"

#include "Attendant.h"
#include <string>
#include <map>
#include <sstream>
#include <vector>

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

    Response* response = new Response();;
    if (dbAdministrator->existsClient(loginInformation->getEmail())) {
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
    }

    int success = dbAdministrator->addClient(personal, operation);
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

Facebook::Facebook() {
    this->functions["GET"] = get;
    this->functions["POST"] = post;
}

Facebook::~Facebook() {
}
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

Firebase::~Firebase() {
}
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
Category::~Category() {
}
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
Skill::~Skill() {
}
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
JobPosition::~JobPosition() {
}
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
