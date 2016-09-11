#ifndef DATABASE_H_
#define DATABASE_H_

#include "leveldb/db.h"

using namespace std;

const string PATH_DB = "./db";

class DataBase {
private:
	leveldb::DB* db;
	leveldb::Options options;
	leveldb::Status status;

public:
	DataBase(string path);
	~DataBase();
	string get(string key);
	void put(string key, string value);
	void erase(string key);
};

#endif
