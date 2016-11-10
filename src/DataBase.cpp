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
        this->put("personal_smpiano@gmail.com", "{\"id\":\"1\",\"first_name\":\"Sergio Matías\",\"last_name\":\"Piano\",\"email\":\"smpiano@gmail.com\",\"birthday\":\"24/07/1984\",\"address\":{\"lat\":\"-34.61543532\",\"lon\":\"-58.37213459\"}}");
        this->put("smpiano@gmail.com", "admin");
        this->put("personal_facundoddefalco@gmail.com", "{\"id\":\"2\",\"first_name\":\"Facundo\",\"last_name\":\"De Falco\",\"email\":\"facundoddefalco@gmail.com\",\"birthday\":\"22/12/1989\",\"address\":{\"lat\":\"-34.61543532\",\"lon\":\"-58.37213459\"}}");
        this->put("facundoddefalco@gmail.com", "admin");
        this->put("personal_facusan@gmail.com", "{\"id\":\"3\",\"first_name\":\"Facundo\",\"last_name\":\"Sánchez Galindo\",\"email\":\"facusan@gmail.com\",\"birthday\":\"26/02/1987\",\"address\":{\"lat\":\"-34.61543532\",\"lon\":\"-58.37213459\"}}");
        this->put("facusan@gmail.com", "admin");
        this->put("personal_hernanfiuba2009@gmail.com", "{\"id\":\"4\",\"first_name\":\"Hernán\",\"last_name\":\"Arroyo García\",\"email\":\"hernanfiuba2009@gmail.com\",\"birthday\":\"25/02/1990\",\"address\":{\"lat\":\"-34.61543532\",\"lon\":\"-58.37213459\"}}");
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
