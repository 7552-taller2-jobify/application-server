#ifndef DATABASE_H_
#define DATABASE_H_

#include <iostream>
#include "leveldb/db.h"
#include "Logger.h"

using namespace std;

const string PATH_DB = "./db";

class DataBase {
private:
	leveldb::DB* db;
	leveldb::Options options;
	leveldb::Status status;
	Logger *logger;

public:
	DataBase(string path, Logger *logger);
	~DataBase();
	string get(string key);
	void put(string key, string value);
	void erase(string key);
};

#endif
