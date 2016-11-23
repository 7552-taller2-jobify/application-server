// "Copyright 2016 <Jobify>"

#include "DataBase.h"
#include <string>
#include <vector>
#include <algorithm>

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
    std::string value = "";
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

leveldb::Iterator* DataBase::getIterator() {
    return this->db->NewIterator(leveldb::ReadOptions());
}
