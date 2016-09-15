#include "Logger.cpp"
#include <gtest/gtest.h>

class TestLogger : public ::testing::Test { 
public:
	Logger *log;
	ifstream file;

	void SetUp() { 
		log = new Logger();
		file.open(LOG_FILE_PATH.c_str());
	}
 
	void TearDown() {
		delete log;
		file.close();
	}
};
 
TEST_F(TestLogger, testLoggerWellCreated) {
	ASSERT_TRUE(ifstream(LOG_FILE_PATH.c_str()));
	string line;
	getline(file, line);
	ASSERT_EQ(line+"\n", LOG_FILE_CREATED);
}

TEST_F(TestLogger, testLog) {
	string line;
	const string MESSAGE = "Error message.";
	char aux;
	log->log(error, MESSAGE);
	file.seekg(0, file.end);
	while(true) {
		file.seekg(-2, file.cur);
		file.get(aux);
		if(aux == ':') {
			file.get(aux);
			break;
		}
	}
	getline(file, line);
	ASSERT_EQ(line, MESSAGE);
}
