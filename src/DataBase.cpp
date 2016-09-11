#include "DataBase.h"

DataBase::DataBase(string path) {
	this->options.create_if_missing = true;
	this->status = leveldb::DB::Open(options, path, &(this->db));
	//TODO use log to save if it was created ok or not
}

DataBase::~DataBase() {
	delete this->db;	
}

string DataBase::get(string key) {
	string value;
	this->status = this->db->Get(leveldb::ReadOptions(), key, &value);
	//TODO use log to save if it was possible to get the value
	return value;
}

void DataBase::put(string key, string value) {
	this->status = this->db->Put(leveldb::WriteOptions(), key, value);
	//TODO use log to save if it was possible to put the pair key-value
}

void DataBase::erase(string key) {
	this->status = this->db->Delete(leveldb::WriteOptions(), key);
	//TODO use log to save if it was possible to delete the key and its associated value
}
