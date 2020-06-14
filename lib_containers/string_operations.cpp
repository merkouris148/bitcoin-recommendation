#include "string_operations.h"

#include <sstream>

using namespace std;

//<head><delimeter><tail>
pair<string, string> split_head_tail(string& line, char delimeter){
	pair<string, string> p;
	
	p.first = line.substr(0, line.find_first_of(delimeter));
	p.second = line.substr(line.find_first_of(delimeter) + 1);
	
	return p;
}

void line2tokenlist(std::string& line, char delimeter, std::list<string>& tokens){
	stringstream sline(line);
	
	string token;
	while(getline(sline, token, delimeter)) tokens.push_back(token);	
}
