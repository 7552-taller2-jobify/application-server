// "Copyright 2016 <Jobify>"

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <string>

enum LogStatus {error = 0, warn, info, debug};

const char LOG_FILE_PATH[] = "files/log.txt";
const char LOG_FILE_CREATED[] = "---LOG FILE CREATED---\n";

class Logger {
 private:
    Logger();
    std::fstream file;
    const std::string getCurrentDateParsed();

 public:
    static Logger& getInstance() {
        static Logger an_instance;
        return an_instance;
    }
    ~Logger();
    void log(enum LogStatus status, const std::string message);
};

#endif  // LOGGER_H_
