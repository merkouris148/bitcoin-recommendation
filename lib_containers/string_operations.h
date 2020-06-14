#ifndef STRING_OPERATIONS_H

#define STRING_OPERATIONS_H

#include <string>
#include <list>

//<head><delimeter><tail>
std::pair<std::string, std::string> split_head_tail(std::string& line, char delimeter);

void line2tokenlist(std::string& line, char delimeter, std::list<std::string>& tokens);

#endif
