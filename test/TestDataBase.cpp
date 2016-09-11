#include "DataBase.cpp"
#include <gtest/gtest.h>

class TestDataBase : public ::testing::Test { 
public:
	DataBase *db;

	void SetUp() { 
		db = new DataBase("./testdb", new Logger());
		db->put("10", "Diez");
		db->put("5", "Cinco");
		db->put("7", "Siete");
	}
 
	~TestDataBase() {
		delete db;
	}
};
 
TEST_F(TestDataBase, testDBWellCreated) {
	ASSERT_EQ(db->get("10"), "Diez");
	ASSERT_EQ(db->get("5"), "Cinco");
	ASSERT_EQ(db->get("7"), "Siete");
}

TEST_F(TestDataBase, testDeleteKey) {
	db->erase("5");
	ASSERT_EQ(db->get("5"), "");
	ASSERT_EQ(db->get("10"), "Diez");
	ASSERT_EQ(db->get("7"), "Siete");
}

TEST_F(TestDataBase, testAddKey) {
	db->put("8", "Ocho");
	ASSERT_EQ(db->get("8"), "Ocho");
	ASSERT_EQ(db->get("10"), "Diez");
	ASSERT_EQ(db->get("5"), "Cinco");
	ASSERT_EQ(db->get("7"), "Siete");
}
