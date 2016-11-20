// "Copyright 2016 <Jobify>"

#include "DataBaseAdministrator.h"
#include <string>

DataBaseAdministrator::DataBaseAdministrator() {
    this->auth = new Authentication();
}

DataBaseAdministrator::~DataBaseAdministrator() {
    delete this->auth;
}

bool DataBaseAdministrator::existsClient(std::string email) {
    return (DataBase::getInstance().get(email) != "");
}

bool DataBaseAdministrator::rightClient(LoginInformation *loginInformation) {
    std::string email = loginInformation->getEmail();
    std::string password = loginInformation->getPassword();
    bool existsClient = this->existsClient(email);
    if (existsClient) {
        std::string credentials_parser = DataBase::getInstance().get(email);
        Credentials *credentials = new Credentials();
        credentials->loadJson(credentials_parser);
        std::string token_calculated = this->auth->encode(email, password, credentials->getIncrementalNumber());
        bool rightCredentials = strcmp(credentials->getToken().c_str(), token_calculated.c_str()) == 0;
        if (rightCredentials) {
            return true;
        }
    }
    return false;
}

bool DataBaseAdministrator::rightClient(std::string email, std::string token) {
    bool existsClient = this->existsClient(email);
    if (existsClient) {
        std::cout << "exists client" << std::endl;
        std::string credentials_parser = DataBase::getInstance().get(email);
        Credentials *credentials = new Credentials();
        credentials->loadJson(credentials_parser);
        std::string token_stored = credentials->getToken();
        bool rightCredentials = strcmp(token_stored.c_str(), token.c_str()) == 0;
        if (rightCredentials) {
            return true;
        }
    }
    std::cout << "not exists client" << std::endl;
    return false;
}

std::string DataBaseAdministrator::getPersonalLogin(std::string email) {
    std::string personal_parser =  DataBase::getInstance().get("PERSONAL_" + email);
    std::string picture_parser =  DataBase::getInstance().get("PICTURE_" + email);
    Personal *personal = new Personal();
    personal->loadJson(personal_parser);
    Picture *picture = new Picture();
    bool hasPicture = strcmp(picture_parser.c_str(), "") != 0;
    if (hasPicture) {
        picture->loadJson(picture_parser);
    } else {
        picture->setPicture("");
    }
    std::string credentials_parser =  DataBase::getInstance().get(email);
    Credentials *credentials = new Credentials();
    credentials->loadJson(credentials_parser);
    std::string token = credentials->getToken();
    UserService *userService = new UserService();
    std::string loginInformation_parser = userService->loginInformation(personal, picture, token);
    return loginInformation_parser;
}

// Returns 0 if success, 1 if email exists, 2 if there are empty fields
int DataBaseAdministrator::addClient(Personal *personal, LoginInformation *loginInformation, struct Message operation) {
    std::string email = loginInformation->getEmail();
    std::string password = loginInformation->getPassword();

    if (this->existsClient(email)) {
        return 1;
    }
    if (personal->emptyFields()) {
        return 2;
    }
    bool hasLoginFacebook = strcmp(operation.params.c_str(), "app=facebook") == 0;
    if (hasLoginFacebook) {
        DataBase::getInstance().put("PERSONAL_" + email, personal->createJsonFile());
    } else {
        int incremental_number = 0;
        std::string token = this->auth->encode(email, password, incremental_number);
        Credentials *credentials = new Credentials();
        credentials->setToken(token);
        credentials->setIncrementalNumber(incremental_number);
        std::string credentials_parser = credentials->createJsonFile();
        DataBase::getInstance().put(email, credentials_parser);
        DataBase::getInstance().put("PERSONAL_" + email, personal->createJsonFile());
    }
    return 0;
}

// Returns 0 if success, 1 if credential invalid
int DataBaseAdministrator::uploadPersonal(std::string email, std::string token, Personal *upload_personal) {
    bool rightCredential = this->rightClient(email, token);
    if (rightCredential) {
        std::string actual_personal_parser =  DataBase::getInstance().get("PERSONAL_" + email);
        Personal *actual_personal = new Personal();
        actual_personal->loadJson(actual_personal_parser);
        actual_personal->setFirstName(upload_personal->getFirstName());
        actual_personal->setLastName(upload_personal->getLastName());
        actual_personal->setGender(upload_personal->getGender());
        actual_personal->setBirthday(upload_personal->getBirthday());
        actual_personal->setCity(upload_personal->getCity());
        actual_personal->setAddress(upload_personal->getAddress()[0], upload_personal->getAddress()[1]);

        DataBase::getInstance().erase("PERSONAL_" + email);
        DataBase::getInstance().put("PERSONAL_" + email, actual_personal->createJsonFile());
        return 0;
    }
    return 1;
}

std::string DataBaseAdministrator::getPersonal(std::string email) {
    std::string personal_parser =  DataBase::getInstance().get("PERSONAL_" + email);
    Personal *personal = new Personal();
    personal->loadJson(personal_parser);
    UserService *userService = new UserService();
    std::string personal_parser_modify = userService->getPersonal(personal);
    return personal_parser_modify;
}

// Returns 0 if success, 1 if credential invalid
int DataBaseAdministrator::uploadSummary(std::string email, std::string token, Summary *upload_summary) {
    bool rightCredential = this->rightClient(email, token);
    if (rightCredential) {
        DataBase::getInstance().erase("SUMMARY_" + email);
        DataBase::getInstance().put("SUMMARY_" + email, upload_summary->createJsonFile());
        return 0;
    }
    return 1;
}

std::string DataBaseAdministrator::getSummary(std::string email) {
    std::string summary_parser = DataBase::getInstance().get("SUMMARY_" + email);
    return summary_parser;
}

// Returns 0 if success, 1 if credential invalid
int DataBaseAdministrator::uploadExpertise(std::string email, std::string token, Expertise *upload_expertise) {
    bool rightCredential = this->rightClient(email, token);
    if (rightCredential) {
        DataBase::getInstance().erase("EXPERTISE_" + email);
        DataBase::getInstance().put("EXPERTISE_" + email, upload_expertise->createJsonFile());
        return 0;
    }
    return 1;
}

std::string DataBaseAdministrator::getExpertise(std::string email) {
    return DataBase::getInstance().get("EXPERTISE_" + email);
}

// Returns 0 if success, 1 if credential invalid
int DataBaseAdministrator::uploadSkills(std::string email, std::string token, Skills *upload_skills) {
    bool rightCredential = this->rightClient(email, token);
    if (rightCredential) {
        DataBase::getInstance().erase("SKILLS_" + email);
        DataBase::getInstance().put("SKILLS_" + email, upload_skills->createJsonFile());
        return 0;
    }
    return 1;
}

std::string DataBaseAdministrator::getSkills(std::string email) {
    return DataBase::getInstance().get("SKILLS_" + email);
}

// Returns 0 if success, 1 if credential invalid
int DataBaseAdministrator::uploadPicture(std::string email, std::string token, Picture *upload_picture) {
    bool rightCredential = this->rightClient(email, token);
    if (rightCredential) {
        DataBase::getInstance().erase("PICTURE_" + email);
        DataBase::getInstance().put("PICTURE_" + email, upload_picture->createJsonFile());
        return 0;
    }
    return 1;
}

std::string DataBaseAdministrator::getPicture(std::string email) {
    return DataBase::getInstance().get("PICTURE_" + email);
}

// Returns 0 if success, 1 if credential invalid
int DataBaseAdministrator::addSolicitude(std::string email, std::string token, struct Solicitude new_solicitude) {
    bool rightCredential = this->rightClient(email, token);
    if (rightCredential) {
        Solicitudes *solicitudes = new Solicitudes();
        if (DataBase::getInstance().get("SOLICITUDES_" + email) == "") {
            DataBase::getInstance().put("SOLICITUDES_" + email, "{\"solicitudes\":[]}");
        }
        solicitudes->loadJson(DataBase::getInstance().get("SOLICITUDES_" + email));
        DataBase::getInstance().erase("SOLICITUDES_" + email);
        solicitudes->addSolicitude(new_solicitude);
        DataBase::getInstance().put("SOLICITUDES_" + email, solicitudes->createJsonFile());
        delete solicitudes;
        return 0;
    }
    return 1;
}

std::string DataBaseAdministrator::getSolicitudes(std::string email) {
    return DataBase::getInstance().get("SOLICITUDES_" + email);
}

// Returns -1 if there is no solicitude, other if success
int DataBaseAdministrator::addFriend(std::string email, struct Solicitude solicitude_to_delete) {
    Solicitudes *solicitudes = new Solicitudes();
    if (DataBase::getInstance().get("SOLICITUDES_" + email) == "") {
        DataBase::getInstance().put("SOLICITUDES_" + email, "{\"solicitudes\":[]}");
    }
    solicitudes->loadJson(DataBase::getInstance().get("SOLICITUDES_" + email));
    DataBase::getInstance().erase("SOLICITUDES_" + email);
    int return_code = solicitudes->removeSolicitude(solicitude_to_delete);
    DataBase::getInstance().put("SOLICITUDES_" + email, solicitudes->createJsonFile());
    delete solicitudes;
    Friends *friends = new Friends();
    if (DataBase::getInstance().get("FRIENDS_" + email) == "") {
        DataBase::getInstance().put("FRIENDS_" + email, "{\"friends\":[]}");
    }
    friends->loadJson(DataBase::getInstance().get("FRIENDS_" + email));
    DataBase::getInstance().erase("FRIENDS_" + email);
    friends->addContact(solicitude_to_delete.email);
    DataBase::getInstance().put("FRIENDS_" + email, friends->createJsonFile());
    delete friends;
    return return_code;
}

// Returns -1 if there is no solicitude, other if success
int DataBaseAdministrator::removeSolicitude(std::string email, struct Solicitude solicitude_to_delete) {
    Solicitudes *solicitudes = new Solicitudes();
    if (DataBase::getInstance().get("SOLICITUDES_" + email) == "") {
        DataBase::getInstance().put("SOLICITUDES_" + email, "{\"solicitudes\":[]}");
    }
    solicitudes->loadJson(DataBase::getInstance().get("SOLICITUDES_" + email));
    DataBase::getInstance().erase("SOLICITUDES_" + email);
    int return_code = solicitudes->removeSolicitude(solicitude_to_delete);
    DataBase::getInstance().put("SOLICITUDES_" + email, solicitudes->createJsonFile());
    delete solicitudes;
    return return_code;
}

std::string DataBaseAdministrator::getFriends(std::string email) {
    if (DataBase::getInstance().get("FRIENDS_" + email) == "") {
        DataBase::getInstance().put("FRIENDS_" + email, "{\"friends\":[]}");
    }
    return DataBase::getInstance().get("FRIENDS_" + email);
}

std::string DataBaseAdministrator::getOwnRecommendations(std::string email) {
    std::string own_recommendation_email = ("OWN_RECOMMENDATIONS_" + email);
    if (DataBase::getInstance().get(own_recommendation_email) == "") {
        DataBase::getInstance().put(own_recommendation_email, "{\"own_recommendations\":[]}");
    }
    return DataBase::getInstance().get(own_recommendation_email);
}

std::string DataBaseAdministrator::getOthersRecommendations(std::string email) {
    std::string others_recommendation_email = ("OTHERS_RECOMMENDATIONS_" + email);
    if (DataBase::getInstance().get(others_recommendation_email) == "") {
        DataBase::getInstance().put(others_recommendation_email, "{\"others_recommendations\":[]}");
    }
    return DataBase::getInstance().get(others_recommendation_email);
}

void DataBaseAdministrator::vote(std::string email, std::string email_to_vote) {
    OwnRecommendations *own_recommendations = new OwnRecommendations();
    own_recommendations->loadJson(this->getOwnRecommendations(email));
    own_recommendations->addContact(email_to_vote);
    std::string own_recommendation_email = ("OWN_RECOMMENDATIONS_" + email);
    DataBase::getInstance().erase(own_recommendation_email);
    DataBase::getInstance().put(own_recommendation_email, own_recommendations->createJsonFile());
    delete own_recommendations;

    OthersRecommendations *others_recommendations = new OthersRecommendations();
    others_recommendations->loadJson(this->getOthersRecommendations(email_to_vote));
    others_recommendations->addContact(email);
    std::string others_recommendation_email = ("OTHERS_RECOMMENDATIONS_" + email_to_vote);
    DataBase::getInstance().erase(others_recommendation_email);
    DataBase::getInstance().put(others_recommendation_email, others_recommendations->createJsonFile());
    delete others_recommendations;
}

void DataBaseAdministrator::unvote(std::string email, std::string email_to_unvote) {
    OwnRecommendations *own_recommendations = new OwnRecommendations();
    own_recommendations->loadJson(this->getOwnRecommendations(email));
    own_recommendations->removeContact(email_to_unvote);
    std::string own_recommendation_email = ("OWN_RECOMMENDATIONS_" + email);
    DataBase::getInstance().erase(own_recommendation_email);
    DataBase::getInstance().put(own_recommendation_email, own_recommendations->createJsonFile());
    delete own_recommendations;

    OthersRecommendations *others_recommendations = new OthersRecommendations();
    others_recommendations->loadJson(this->getOthersRecommendations(email_to_unvote));
    others_recommendations->removeContact(email);
    std::string others_recommendation_email = ("OTHERS_RECOMMENDATIONS_" + email_to_unvote);
    DataBase::getInstance().erase(others_recommendation_email);
    DataBase::getInstance().put(others_recommendation_email, others_recommendations->createJsonFile());
    delete others_recommendations;
}

std::vector<struct PopularUser> DataBaseAdministrator::searchRange(const std::string start, const std::string end) {
    std::vector<struct PopularUser> users;
    const leveldb::Slice& start_slice = leveldb::Slice(start);
    const leveldb::Slice& end_slice = leveldb::Slice(end);
    leveldb::Iterator *db_iterator = DataBase::getInstance().getIterator();
    for (db_iterator->Seek(start_slice); (db_iterator->Valid() &&
                            (strcmp(db_iterator->key().ToString().c_str(), end.c_str()) <= 0)); db_iterator->Next()) {     
        if (!db_iterator->value().empty()) {
            struct PopularUser user;
            const std::string LABEL = "OTHERS_RECOMMENDATIONS_";
            user.email = db_iterator->key().ToString().substr(LABEL.length());
            OthersRecommendations *others_recommendations = new OthersRecommendations();
            others_recommendations->loadJson(db_iterator->value().ToString());
            user.votes = others_recommendations->getNumberOfContacts();
            delete others_recommendations;
            if (users.size() < MAX_POPULAR_USERS) {
                users.push_back(user);
                std::sort(users.begin(), users.end(), std::greater<struct PopularUser>());
            } else {
                if (users.back().votes < user.votes) {
                    users.pop_back();
                    users.push_back(user);
                    std::sort(users.begin(), users.end(), std::greater<struct PopularUser>());
                }
            }
        }
    }
    delete db_iterator;
    return users;
}

std::string DataBaseAdministrator::getMostPopularUsers() {
    std::string result = "{\"most_popular_users\":[";
    const std::string begin = "OTHERS_RECOMMENDATIONS_";
    const std::string end = "OTHERS_RECOMMENDATIONT_";
    std::vector<struct PopularUser> users = this->searchRange(begin, end);
    for (int i = 0; i < users.size(); i++) {
        std::ostringstream s;
        s << users.at(i).votes;
        std::string votes_parsed = s.str();
        result += ("{\"email\":\"" + users.at(i).email + "\",\"votes\":" + votes_parsed + "},");
    }
    if (users.size() > 0) {
        result = result.substr(0, result.length() - 1);
    }
    result += "]}";
    return result;
}
