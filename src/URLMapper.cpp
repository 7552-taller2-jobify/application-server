#include "URLMapper.h"

URLMapper::URLMapper() {
	if(!file.is_open()) {
		file.open(URL_FILE_PATH.c_str());
	}
	string line;
	while (getline(file, line)) {
		stringstream ss_line(line);
		string parameters[2];
		ss_line >> parameters[0] >> parameters[1]; 
		urls[parameters[0]] = parameters[1];
	}

	file.close();
}

URLMapper::~URLMapper() {
	
}

string URLMapper::find(string key_to_match) {
	for(map<string, string>::iterator it = this->urls.begin(); it != this->urls.end(); it++) {
		if (key_to_match == it->first) {
			return it->second;
		}
	}
	return "";
}
