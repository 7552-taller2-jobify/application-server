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


// TODO DataBase example
        this->put("personal_smpiano@gmail.com", "{\n\t\"id\": \"1\",\n\t\"first_name\": \"Sergio Matías\",\n\t\"last_name\": \"Piano\",\n\t\"email\": \"smpiano@gmail.com\",\n\t\"birthday\": \"24/07/1984\",\n\t\"address\": {\n\t\t\"lat\": \"-34.61543532\",\n\t\t\"lon\": \"-58.37213459\"\n\t}\n}");
        this->put("smpiano@gmail.com", "admin");
        this->put("personal_facundoddefalco@gmail.com", "{\n\t\"id\": \"2\",\n\t\"first_name\": \"Facundo\",\n\t\"last_name\": \"De Falco\",\n\t\"email\": \"facundoddefalco@gmail.com\",\n\t\"birthday\": \"22/12/1989\",\n\t\"address\": {\n\t\t\"lat\": \"-34.61543532\",\n\t\t\"lon\": \"-58.37213459\"\n\t}\n}");
        this->put("facundoddefalco@gmail.com", "admin");
        this->put("personal_facusan@gmail.com", "{\n\t\"id\": \"3\",\n\t\"first_name\": \"Facundo\",\n\t\"last_name\": \"Sánchez Galindo\",\n\t\"email\": \"facusan@gmail.com\",\n\t\"birthday\": \"26/02/1987\",\n\t\"address\": {\n\t\t\"lat\": \"-34.61543532\",\n\t\t\"lon\": \"-58.37213459\"\n\t}\n}");
        this->put("facusan@gmail.com", "admin");
        this->put("personal_hernanfiuba2009@gmail.com", "{\n\t\"id\": \"4\",\n\t\"first_name\": \"Hernán\",\n\t\"last_name\": \"Arroyo García\",\n\t\"email\": \"hernanfiuba2009@gmail.com\",\n\t\"birthday\": \"25/02/1990\",\n\t\"address\": {\n\t\t\"lat\": \"-34.61543532\",\n\t\t\"lon\": \"-58.37213459\"\n\t}\n}");
        this->put("hernanfiuba2009@gmail.com", "admin");

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
