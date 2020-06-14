#include "io_methods.h"

#include <cassert>

#include "../lib_containers/string_operations.h"

using namespace std;

//constructor destructor
file2data::file2data(
						Sentiment& SSent,
						ItemsDictionary& IItems,
						const char* filename
					) :
						Sent(SSent),
						Items(IItems)
						{
	
	file_in.open(filename);
	assert(file_in.is_open());
}

file2data::~file2data(){
	file_in.close();
}

//private functions
bool file2data::read_line(){
	if(!file_in.eof()) getline(file_in, line);  line2tokenlist(line, '\t', tokens);
	return !file_in.eof();
}

void file2data::get_ids(){
	user_id_new = "u" + tokens.front(); tokens.pop_front(); 
	tweet_id	= "t" + tokens.front(); tokens.pop_front();
}

void file2data::create_tweet(list<PreferenceVector*>& Tweets){
	
	PreferenceVector* tweet = new PreferenceVector(Items, Sent, tokens, tweet_id.c_str());
	tokens.clear();
	
	Tweets.push_back(tweet);
	
	user_tweets.push_back(tweet);
}

void file2data::create_user(list<PreferenceVector*>& Users){
	
	PreferenceVector* user = new PreferenceVector(Items, Sent, user_id_old.c_str());
	
	for(auto it = user_tweets.begin(); it != user_tweets.end(); ++it) pref_v_add(*user, **it, *user);
	user_tweets.clear();
	
	Users.push_back(user);
}


//functor
void file2data::operator()(
							std::list<PreferenceVector*>& Users,
							std::list<PreferenceVector*>& Tweets
						){
	read_line();
	get_ids();
	create_tweet(Tweets);
	
	user_id_old = user_id_new;
	
	while(read_line()){	
		get_ids();
		if(user_id_new != user_id_old)  create_user(Users);
		create_tweet(Tweets);
		
		user_id_old = user_id_new;
	}
	
	create_user(Users);
}	









