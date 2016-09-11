#include "DataBase.h"

DataBase::DataBase(){
     this->options;
    options.create_if_missing = true;
    this->writeOptions;

}
void DataBase::open(){

    leveldb::Status status = leveldb::DB::Open(this->options, "./db", &this->db);

    if (false == status.ok())
    {
        cerr << "Unable to open/create test database './db'" << endl;
        cerr << status.ToString() << endl;
	}
	
}
void DataBase::putHola(){
	this->db->Put(this->writeOptions, "1", "HOLA DATABASE AND REQUESTADMINISTRATOR");
}
DataBase::~DataBase(){
	
}
