#include "Logger.cpp"
#include <gtest/gtest.h>

class TestLogger : public ::testing::Test { 
public:
	Logger *log;
	ifstream file;

	void SetUp() { 
		log = new Logger();
	}
 
	~TestLogger() {
		delete log;
	}
};
 
TEST_F(TestLogger, testLoggerWellCreated) {
	ASSERT_TRUE(ifstream(LOG_FILE_PATH.c_str()));
	file.open(LOG_FILE_PATH.c_str());
	string line;
	getline(file, line);
	ASSERT_EQ(line+"\n", LOG_FILE_CREATED);
	file.close();
}

TEST_F(TestLogger, testLog) {
	file.open(LOG_FILE_PATH.c_str());
	string line;
	int first_count = 0, second_count = 0;
	while(getline(file, line)) {
		first_count++;
	}
	log->log(error, "Error message.");
	log->log(warn, "Warn message.");
	log->log(info, "Info message.");
	log->log(debug, "Debug message.");
	file.close();
	file.open(LOG_FILE_PATH.c_str());
	while(getline(file, line)) {
		second_count++;
	}
	ASSERT_EQ(second_count, first_count + 4);
	file.close();
}
