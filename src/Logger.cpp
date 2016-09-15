#include "Logger.h"

Logger::Logger() {
	if(!file.is_open()) {
		file.open(LOG_FILE_PATH.c_str(), fstream::out | fstream::app);
	}
	file.seekg(0, ios::end);  
	if(file.tellg() == 0) {    
		file << LOG_FILE_CREATED;
		file.flush(); 
	}
}

Logger::~Logger() {
	file.close();
}

const string Logger::getCurrentDateParsed() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X ", &tstruct);
    return buf;
}

void Logger::log(enum LogStatus status, const string message) {
	string status_aux;
	switch(status) {
		case error:
			status_aux = "Error: ";
			break;
		case warn:
			status_aux = "Warn: ";
			break;
		case info:
			status_aux = "Info: ";
			break;
		case debug:
			status_aux = "Debug: ";
			break;
		default:
			status_aux = "Error: ";			
	}
	file << getCurrentDateParsed() << status_aux << message << "\n";
	file.flush();
}