// "Copyright 2016 <Jobify>"

#include "DataBaseAdministrator.h"
#include <string>
#include <algorithm>
#include <vector>
#include <functional>

DataBaseAdministrator::DataBaseAdministrator() {
    this->auth = new Authentication();
}

DataBaseAdministrator::~DataBaseAdministrator() {
    delete this->auth;
}

bool DataBaseAdministrator::existsClient(std::string email) {
    return DataBase::getInstance().exists(email);
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
        std::string credentials_parser = DataBase::getInstance().get(email);
        Credentials *credentials = new Credentials();
        credentials->loadJson(credentials_parser);
        std::string token_stored = credentials->getToken();
        bool rightCredentials = strcmp(token_stored.c_str(), token.c_str()) == 0;
        if (rightCredentials) {
            return true;
        }
    }
    return false;
}

std::string DataBaseAdministrator::getToken(std::string email) {
    bool existsClient = this->existsClient(email);
    if (existsClient) {
        std::string credentials_parser = DataBase::getInstance().get(email);
        Credentials *credentials = new Credentials();
        credentials->loadJson(credentials_parser);
        return credentials->getToken();
    }
    return "";
}

std::string DataBaseAdministrator::getEmailFromToken(std::string token) {
    Authentication *auth = new Authentication();
    return auth->getEmailFromToken(token);
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
int DataBaseAdministrator::addClient(Personal *personal, Picture *picture, LoginInformation *loginInformation,
                                                                                        struct Message operation) {
    std::string email = loginInformation->getEmail();
    std::string password = loginInformation->getPassword();

    if (this->existsClient(email)) {
        return 1;
    }
    bool hasLoginFacebook = strcmp(operation.params.c_str(), "app=facebook") == 0;
    if (!hasLoginFacebook) {
        if (personal->emptyFields()) {
            return 2;
        }
    }
    /*if (hasLoginFacebook) {
        DataBase::getInstance().put("PERSONAL_" + email, personal->createJsonFile());
    } else {*/
        int incremental_number = 0;
        std::string token = this->auth->encode(email, password, incremental_number);
        Credentials *credentials = new Credentials();
        credentials->setToken(token);
        credentials->setIncrementalNumber(incremental_number);
        std::string credentials_parser = credentials->createJsonFile();
        DataBase::getInstance().put(email, credentials_parser);
        DataBase::getInstance().put("PERSONAL_" + email, personal->createJsonFile());
        DataBase::getInstance().put("PICTURE_" + email, picture->createJsonFile());
    //}
    IdsDataBase *idsDB = new IdsDataBase();
    std::string ids_parse = DataBase::getInstance().get("IDS");
    idsDB->loadJson(ids_parse);
    idsDB->addId(email);
    DataBase::getInstance().erase("IDS");
    DataBase::getInstance().put("IDS", idsDB->createJsonFile());
    delete idsDB;
    return 0;
}

// Returns 0 if success, 1 if credential invalid
int DataBaseAdministrator::createPersonal(std::string email, std::string token, Personal *upload_personal) {
    bool rightCredential = this->rightClient(email, token);
    if (rightCredential) {
        Personal *actual_personal = new Personal();
        actual_personal->setFirstName(upload_personal->getFirstName());
        actual_personal->setLastName(upload_personal->getLastName());
        actual_personal->setGender(upload_personal->getGender());
        actual_personal->setBirthday(upload_personal->getBirthday());
        actual_personal->setAddress(upload_personal->getAddress()[0], upload_personal->getAddress()[1]);
        DataBase::getInstance().put("PERSONAL_" + email, actual_personal->createJsonFile());
        return 0;
    }
    return 1;
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
        actual_personal->setAddress(upload_personal->getAddress()[0], upload_personal->getAddress()[1]);

        DataBase::getInstance().erase("PERSONAL_" + email);
        DataBase::getInstance().put("PERSONAL_" + email, actual_personal->createJsonFile());
        return 0;
    }
    return 1;
}

std::string DataBaseAdministrator::getProfilePersonal(std::string email) {
    std::string personal_parser =  DataBase::getInstance().get("PERSONAL_" + email);
    Personal *personal = new Personal();
    personal->loadJson(personal_parser);
    UserService *userService = new UserService();
    std::string personal_parser_modify = userService->getPersonal(personal);
    return personal_parser_modify;
}

std::string DataBaseAdministrator::getPersonal(std::string email) {
    std::string personal_parser =  DataBase::getInstance().get("PERSONAL_" + email);
    return personal_parser;
}

// Returns 0 if success, 1 if credential invalid
int DataBaseAdministrator::createSummary(std::string email, std::string token, Summary *upload_summary) {
    bool rightCredential = this->rightClient(email, token);
    if (rightCredential) {
        DataBase::getInstance().put("SUMMARY_" + email, upload_summary->createJsonFile());
        return 0;
    }
    return 1;
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
    std::string summary_parse = DataBase::getInstance().get("SUMMARY_" + email);
    bool isEmpty = strcmp(summary_parse.c_str(), "") == 0;
    if (isEmpty) {
        summary_parse = "{\"summary\":\"\"}";
    }
    return summary_parse;
}

// Returns 0 if success, 1 if credential invalid
int DataBaseAdministrator::createExpertise(std::string email, std::string token, Expertise *upload_expertise) {
    bool rightCredential = this->rightClient(email, token);
    if (rightCredential) {
        DataBase::getInstance().put("EXPERTISE_" + email, upload_expertise->createJsonFile());
        return 0;
    }
    return 1;
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
    std::string expertises_parse =  DataBase::getInstance().get("EXPERTISE_" + email);
    bool isEmpty = strcmp(expertises_parse.c_str(), "") == 0;
    if (isEmpty) {
        expertises_parse = "{\"expertises\":[]}";
    }
    return expertises_parse;
}

// Returns 0 if success, 1 if credential invalid
int DataBaseAdministrator::createSkills(std::string email, std::string token, Skills *upload_skills) {
    bool rightCredential = this->rightClient(email, token);
    if (rightCredential) {
        DataBase::getInstance().put("SKILLS_" + email, upload_skills->createJsonFile());
        return 0;
    }
    return 1;
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
    std::string skills_parse =  DataBase::getInstance().get("SKILLS_" + email);
    bool isEmpty = strcmp(skills_parse.c_str(), "") == 0;
    if (isEmpty) {
        skills_parse = "{\"every_skill\":[]}";
    }
    return skills_parse;
}

// Returns 0 if success, 1 if credential invalid
int DataBaseAdministrator::createPicture(std::string email, std::string token, Picture *upload_picture) {
    bool rightCredential = this->rightClient(email, token);
    if (rightCredential) {
        DataBase::getInstance().put("PICTURE_" + email, upload_picture->createJsonFile());
        return 0;
    }
    return 1;
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
    std::string picture_parse =  DataBase::getInstance().get("PICTURE_" + email);
    bool isEmpty = strcmp(picture_parse.c_str(), "") == 0;
    if (isEmpty) {
        picture_parse = "{\"picture\":\"\"}";
    }
    return picture_parse;
}

// Returns 0 if success, 1 if credential invalid
int DataBaseAdministrator::addSolicitude(std::string email, std::string token, struct Solicitude new_solicitude) {
    bool rightCredential = this->rightClient(new_solicitude.email, token);
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
    std::string solicitudes_parse = DataBase::getInstance().get("SOLICITUDES_" + email);
    bool isEmpty = strcmp(solicitudes_parse.c_str(), "") == 0;
    if (isEmpty) {
        solicitudes_parse = "{\"solicitudes\":[]}";
    }
    return solicitudes_parse;
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
    if (return_code != -1) {
        Friends *friends = new Friends();
        if (DataBase::getInstance().get("FRIENDS_" + email) == "") {
            DataBase::getInstance().put("FRIENDS_" + email, "{\"friends\":[]}");
        }
        friends->loadJson(DataBase::getInstance().get("FRIENDS_" + email));
        DataBase::getInstance().erase("FRIENDS_" + email);
        friends->addContact(solicitude_to_delete.email);
        DataBase::getInstance().put("FRIENDS_" + email, friends->createJsonFile());
        delete friends;

        Friends *friends_other_user = new Friends();
        std::string email_friend = solicitude_to_delete.email;
        if (DataBase::getInstance().get("FRIENDS_" + email_friend) == "") {
            DataBase::getInstance().put("FRIENDS_" + email_friend, "{\"friends\":[]}");
        }
        friends_other_user->loadJson(DataBase::getInstance().get("FRIENDS_" + email_friend));
        DataBase::getInstance().erase("FRIENDS_" + email_friend);
        friends_other_user->addContact(email);
        DataBase::getInstance().put("FRIENDS_" + email_friend, friends_other_user->createJsonFile());
        delete friends_other_user;
    }
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


        std::string pop_email = users.at(i).email;
        std::string personal_str = this->getPersonal(pop_email);
        Personal *personal = new Personal();
        personal->loadJson(personal_str);
        std::string picture_str = this->getPicture(pop_email);
        Picture *picture = new Picture();
        picture->loadJson(picture_str);




        std::ostringstream s;
        s << users.at(i).votes;
        std::string votes_parsed = s.str();
        result += ("{\"email\":\"" + users.at(i).email + "\",\"first_name\":\"" + personal->getFirstName() +
                   "\",\"last_name\":\"" + personal->getLastName() + "\",\"thumbnail\":\"" +
                    picture->getPicture() + "\",\"votes\":" + votes_parsed + "},");
    }
    if (users.size() > 0) {
        result = result.substr(0, result.length() - 1);
    }
    result += "]}";
    return result;
}

std::vector<std::string>* DataBaseAdministrator::getAllIds(std::string own_id) {
    std::string ids_parser = DataBase::getInstance().get("IDS");
    IdsDataBase *idsDB = new IdsDataBase();
    idsDB->loadJson(ids_parser);
    std::vector<std::string>* ids = idsDB->getIds();
    for (int i = 0; i < ids->size(); i++) {
        if (ids->at(i) == own_id) {
            ids->erase(ids->begin() + i);
            break;
        }
    }
    delete idsDB;
    return ids;
}

// Returns 0 if successfully, and 1 in others case
int DataBaseAdministrator::resetPassword(std::string email) {
    int result = 1;
    std::string credentials_parser = DataBase::getInstance().get(email);
    Credentials *credentials = new Credentials();
    credentials->loadJson(credentials_parser);
    std::string token = credentials->getToken();
    Authentication *auth = new Authentication();
    LoginInformation *loginInformation = new LoginInformation();
    bool rightDecode = auth->decode(token, loginInformation, credentials);
    if (rightDecode) {
        loginInformation->setPassword(PASSWORD_DEFAULT);
        int incremental_number = 0;
        std::string token_new = auth->encode(email, PASSWORD_DEFAULT, incremental_number);
        credentials->setToken(token_new);
        credentials->setIncrementalNumber(incremental_number);
        DataBase::getInstance().erase(email);
        DataBase::getInstance().put(email, credentials->createJsonFile());
        result =  0;
    }
    delete credentials;
    delete loginInformation;
    delete auth;
    return result;
}

void DataBaseAdministrator::deleteExpertises(std::string email) {
    bool exists = DataBase::getInstance().exists("EXPERTISE_" + email);
    if (exists) {
        DataBase::getInstance().erase("EXPERTISE_" + email);
        std::string parse = "{\"expertises\":[]}";
        DataBase::getInstance().put("EXPERTISE_" + email, parse);
    }
}

void DataBaseAdministrator::deleteSkills(std::string email) {
    bool exists = DataBase::getInstance().exists("SKILLS_" + email);
    if (exists) {
        DataBase::getInstance().erase("SKILLS_" + email);
        std::string parse = "{\"every_skill\":[]}";
        DataBase::getInstance().put("SKILLS_" + email, parse);
    }
}

void DataBaseAdministrator::deleteSummary(std::string email) {
    bool exists = DataBase::getInstance().exists("SUMMARY_" + email);
    if (exists) {
        DataBase::getInstance().erase("SKILLS_" + email);
        std::string parse = "{\"summary\":\"\"}";
        DataBase::getInstance().put("SUMMARY_" + email, parse);
    }
}

void DataBaseAdministrator::deletePicture(std::string email) {
    bool exists = DataBase::getInstance().exists("PICTURE_" + email);
    if (exists) {
        DataBase::getInstance().erase("PICTURE_" + email);
        std::string parse = "{\"picture\":\"\"}";
        DataBase::getInstance().put("PICTURE_" + email, parse);
    }
}

bool DataBaseAdministrator::checkPasswordIsNull(std::string email) {
    std::string credentials_parser = DataBase::getInstance().get(email);
    Credentials *credentials = new Credentials();
    credentials->loadJson(credentials_parser);
    std::string token = credentials->getToken();
    Authentication *auth = new Authentication();
    LoginInformation *login_information = new LoginInformation();
    bool rightDecode = auth->decode(token, login_information, credentials);
    if (rightDecode) {
        bool result = (login_information->getPassword() == "");
        delete login_information;
        return result;
    } 
    return false;
}
