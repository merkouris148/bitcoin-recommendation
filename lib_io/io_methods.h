#ifndef IO_METHODS_H

#define IO_METHODS_H

#include <list>
#include <string>

#include <fstream>
#include <iostream>

#include "preference_vector.h"
			
class file2data{
private:
//file
	std::ifstream file_in;

//datastructures
	std::string line;
	std::list<std::string> tokens;
	std::list<PreferenceVector*> user_tweets;

//ids
	std::string user_id_new;
	std::string user_id_old;
	std::string tweet_id;

//constants	
	Sentiment& Sent;
	ItemsDictionary& Items;
	

//private functions
	bool read_line();
	void get_ids();
	void create_tweet(std::list<PreferenceVector*>& Tweets);
	void create_user(std::list<PreferenceVector*>& Users);

public:
//constructor destructor
	file2data(
				Sentiment& SSent,
				ItemsDictionary& IItems,
				const char* filename
			);
			
	~file2data();
	
//functor
	void operator()(
						std::list<PreferenceVector*>& Users,
						std::list<PreferenceVector*>& Tweets
					);
};

#endif
