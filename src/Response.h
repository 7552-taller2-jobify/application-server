// "Copyright 2016 <Jobify>"

#ifndef RESPONSE_H_
#define RESPONSE_H_

#include <string>
// #include <cstdint>
// #include <cinttypes>

class Response {
 private:
    long http_code;
    std::string content;

 public:
    Response();
    ~Response();
    std::string getContent();
    void setContent(std::string str);
    void setStatus(long code);
    long getStatus();
};

#endif // RESPONSE_H_
