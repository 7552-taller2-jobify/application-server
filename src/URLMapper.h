#ifndef URLMAPPER_H_
#define URLMAPPER_H_

#include <string>
#include <fstream>
#include <map>
#include <sstream>

using namespace std;

const string URL_FILE_PATH = "files/url.txt";

class URLMapper {
private:
	map<string, string> urls;
	ifstream file;

public:
	URLMapper();
	~URLMapper();
	string find(string key_to_match);
};

#endif
