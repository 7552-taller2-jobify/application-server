#ifndef DATABASE_H_
#define DATABASE_H_
#include <iostream>
#include "leveldb/db.h"

using namespace std;

class DataBase{
private:
		leveldb::DB* db;
		leveldb::Options options;
		leveldb::WriteOptions writeOptions;

public:
		DataBase();
		~DataBase();
		void open();
		void putHola();

};
#endif
