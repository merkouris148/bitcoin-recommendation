#ifndef LEXICONS_H

#define LEXICONS_H

#include <map>
#include <string>
#include <vector>

#include <iostream>

/*
 *************
 * Sentiment *
 *************
*/

class Sentiment : public std::map<std::string, double>{
public:
//constructor destructor
	Sentiment(const std::string& filename);
//operations
	bool member(const std::string& token);
	double operator[](const std::string& token);
};

//debug
std::ostream& operator<<(std::ostream& out, const Sentiment& sent);




/*
 ********************
 * Items Dictionary *
 ********************
*/

class ItemsDictionary : public std::map<std::string, int>{
private:
	int n_items; //# curencies
	std::vector<std::string> items_names;
public:
//constructor destructor
	ItemsDictionary(const std::string& filename);
//debug
	void print();
//operations
	bool member(const std::string& token);
	int get_n_items();
	std::string get_items_name(int value);
};

//debug
std::ostream& operator<<(std::ostream& out, const ItemsDictionary& items);


#endif
