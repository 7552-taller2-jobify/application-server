// "Copyright 2016 <Jobify>"

#include <gtest/gtest.h>
#include <string>
#include "Logger.h"

class TestLogger : public ::testing::Test {
 public:
    std::ifstream file;

    void SetUp() {
        file.open(LOG_FILE_PATH);
    }

    void TearDown() {
        file.close();
    }
};

/*
TEST_F(TestLogger, testLoggerWellCreated) {
>>>>>>> 7fa22b3754e5510eb4d88a5439eb58d4f1aa8437
    ASSERT_TRUE(std::ifstream(LOG_FILE_PATH));
    std::string line;
    std::getline(file, line);
    ASSERT_EQ(line+"\n", LOG_FILE_CREATED);
}

TEST_F(TestLogger, testLog) {
    std::string line;
    const std::string MESSAGE = "Error message.";
    char aux;
    Logger::getInstance().log(error, MESSAGE);
    file.seekg(0, file.end);
    while (true) {
        file.seekg(-2, file.cur);
        file.get(aux);
        if (aux == ':') {
            file.get(aux);
            break;
        }
    }
    std::getline(file, line);
    ASSERT_EQ(line, MESSAGE);
}  */
