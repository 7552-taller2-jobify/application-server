// "Copyright 2016 <Jobify>"

#ifndef DATABASE_H_
#define DATABASE_H_

#include <string>
#include <iostream>
#include <vector>
#include "leveldb/db.h"
#include "Logger.h"
#include "Constants.h"

const char PATH_DB[] = "./db";

class DataBase {
 private:
    leveldb::DB* db;
    leveldb::DB* main_db;
    leveldb::DB* test_db;
    leveldb::Options options;
    leveldb::Status status;
    DataBase();

 public:
    static DataBase& getInstance() {
        static DataBase an_instance;
        return an_instance;
    }
    ~DataBase();
    std::string get(std::string key);
    void put(std::string key, std::string value);
    void erase(std::string key);
    std::vector<struct PopularUser> SearchRange(std::string start, std::string end);
};

#endif  // DATABASE_H_
