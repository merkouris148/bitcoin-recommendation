#include "lexicons.h"

#include <fstream>
#include <sstream>

#include <cassert>

#include "../lib_containers/string_operations.h"

using namespace std;

/*
 *************
 * Sentiment *
 *************
*/

//constructor - destructor
Sentiment::Sentiment(const string& filename){
	ifstream file_in(filename);	assert(file_in.is_open());

	string line;
	while(getline(file_in, line)){
		pair<string, string> p = split_head_tail(line, '\t');
		insert(pair<string, double>(p.first, stod(p.second)));
	}
	
	file_in.close();
}

//operations
bool Sentiment::member(const string& token){
	auto it = find(token); return it != end();
}

double Sentiment::operator[](const string& token){
	if(member(token))   return map::operator[](token);
	else 				return 0;
}

//debug
ostream& operator<<(ostream& out, const Sentiment& sent){
	for(auto it = sent.begin(); it != sent.end(); ++it) out << it->first << "\t" << it->second << endl;
	
	return out;
}

/*
 *******************
 * ItemsDictionary *
 *******************
*/

//constructor destructor
ItemsDictionary::ItemsDictionary(const string& filename){
	ifstream file_in(filename);	assert(file_in.is_open());
	
	string line; int item_id = 1;
	while(getline(file_in, line)){
		
		stringstream sline(line); string token;  bool first_it = true;
		while(getline(sline, token, '\t')){
			
			//keep a name for the key
			if(first_it){ items_names.push_back(token); first_it = false; }
			
			insert(pair<string, int>(token, item_id));
		}
		
		++item_id;
	}
	n_items = item_id - 1;
	
	items_names.shrink_to_fit();
	file_in.close();
}

//operations
bool ItemsDictionary::member(const string& token){
	auto it = find(token); return it != end();
}

//accessors
int ItemsDictionary::get_n_items(){
	return n_items;
}

string ItemsDictionary::get_items_name(int v){
	assert(1 <= v && v <= items_names.size());
	
	return items_names[v];
}

//show
ostream& operator<<(ostream& out, const ItemsDictionary& items){
	for(auto it = items.begin(); it != items.end(); ++it) out << it->first << "\t" << it->second << endl;
	
	return out;
}
