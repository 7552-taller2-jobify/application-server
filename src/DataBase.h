#ifndef DATABASE_H_
#define DATABASE_H_

#include <iostream>
#include "leveldb/db.h"
#include "Logger.h"

const std::string PATH_DB = "./db";

class DataBase {
private:
    leveldb::DB* db;
    leveldb::Options options;
    leveldb::Status status;

public:
    DataBase(std::string path);
    ~DataBase();
    std::string get(std::string key);
    void put(std::string key, std::string value);
    void erase(std::string key);
};

#endif
