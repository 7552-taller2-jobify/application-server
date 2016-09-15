#include "DataBase.h"

DataBase::DataBase(string path, Logger *logger) {
	this->logger = logger;
	this->options.create_if_missing = true;
	this->status = leveldb::DB::Open(options, path, &(this->db));
	if(this->status.ok()) {
		this->logger->log(info, "Database opened successfully.");
	} else {
		this->logger->log(error, "The database could not be opened.");
	}
}

DataBase::~DataBase() {
	delete this->db;	
}

string DataBase::get(string key) {
	string value;
	this->status = this->db->Get(leveldb::ReadOptions(), key, &value);
	if(this->status.ok()) {
		this->logger->log(info, "A value has been retrieved successfully.");
	} else {
		this->logger->log(info, "The key " + key + " does not exist.");
	}
	return value;
}

void DataBase::put(string key, string value) {
	this->status = this->db->Put(leveldb::WriteOptions(), key, value);
	if(this->status.ok()) {
		this->logger->log(info, "A new pair key-value has been put successfully.");
	} else {
		this->logger->log(info, "The new pair key-value could not be put in the Database.");
	}
}

void DataBase::erase(string key) {
	this->status = this->db->Delete(leveldb::WriteOptions(), key);
	if(this->status.ok()) {
		this->logger->log(info, "The pair key-value associated to the key " + key + " has been deleted successfully.");
	} else {
		this->logger->log(info, "The pair key-value associated to the key " + key + " could not be deleted.");
	}
}