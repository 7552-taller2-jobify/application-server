// "Copyright 2016 <Jobify>"

#include "DataBase.h"
#include <string>

DataBase::DataBase() {
    this->options.create_if_missing = true;
    this->status = leveldb::DB::Open(options, PATH_DB, &(this->db));
    if (this->status.ok()) {
        Logger::getInstance().log(info, "Database opened successfully.");
    } else {
        Logger::getInstance().log(error, "The database could not be opened.");
    }
}

DataBase::~DataBase() {
    delete this->db;
}

std::string DataBase::get(std::string key) {
    std::string value;
    this->status = this->db->Get(leveldb::ReadOptions(), key, &value);
    if (this->status.ok()) {
        Logger::getInstance().log(info,
                        "A value has been retrieved successfully.");
    } else {
        Logger::getInstance().log(info, "The key " + key + " does not exist.");
        value = "";
    }
    return value;
}

void DataBase::put(std::string key, std::string value) {
    this->status = this->db->Put(leveldb::WriteOptions(), key, value);
    if (this->status.ok()) {
        Logger::getInstance().log(info,
                        "A new pair key-value has been put successfully.");
    } else {
        Logger::getInstance().log(info,
                "The new pair key-value could not be put in the Database.");
    }
}

void DataBase::erase(std::string key) {
    this->status = this->db->Delete(leveldb::WriteOptions(), key);
    if (this->status.ok()) {
        Logger::getInstance().log(info,
                "The pair key-value associated to the key " + key +
                " has been deleted successfully.");
    } else {
        Logger::getInstance().log(info,
                "The pair key-value associated to the key " + key +
                " could not be deleted.");
    }
}




std::vector<struct PopularUser> DataBase::SearchRange(std::string start, std::string end) {
    //struct PopularUser *users = new struct PopularUser[MAX_POPULAR_USERS];
    int less_votes = -1;
    std::vector<struct PopularUser> users;
    const leveldb::Slice& start_slice = leveldb::Slice(start);
    const leveldb::Slice& end_slice = leveldb::Slice(end);
    leveldb::Iterator *db_iterator = this->db->NewIterator(leveldb::ReadOptions());
    bool condition = (db_iterator->Valid() && (strcmp(db_iterator->key().ToString().c_str(), end.c_str()) <= 0));
    for (db_iterator->Seek(start_slice); condition; db_iterator->Next()) {          
        if (!db_iterator->value().empty()) {
            struct PopularUser user;
            user.email = db_iterator->key().ToString();
            //user.votes = db_iterator->value().ToString();
            //Habria que hacer un get de la DB, levantar un OthersRecommendations y sacar la cantidad de elementos.
            users.push_back(user);
        }
    }
    delete db_iterator;
    return users;
}
