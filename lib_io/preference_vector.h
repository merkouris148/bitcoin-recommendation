#ifndef PREFERENCE_VECTOR_H

#define PREFERENCE_VECTOR_H

#include <string>
#include <map>
#include <list>
#include <set>

#include <iostream>
#include <fstream>

#include "../lib_io/lexicons.h"
#include "../lib_math/point.h"

const double alpha = 15;

class PreferenceVector : public Point{
private:
	std::set<int> references;

public:
//constructor destructor
	PreferenceVector(						
						ItemsDictionary& Items,
						Sentiment& Sent,
						
						std::list<std::string>& terms,
						
						const char* iid = ""
					);

	PreferenceVector(						
						ItemsDictionary& Items,
						Sentiment& Sent,
						
						const char* iid = ""
					);
					
	PreferenceVector(
						int dim,
						const char* iid = ""
					);

//debug
	void print(std::ostream& out = std::cout);

//accessors
	std::set<int> get_references();
	int get_n_references();

//friends
	friend void pref_v_add(PreferenceVector& P1, PreferenceVector& P2, PreferenceVector& P);
};


//operations
void pref_v_add(PreferenceVector& P1, PreferenceVector& P2, PreferenceVector& P);


//show
std::ostream& operator<<(std::ostream& out, const std::set<int>& s);


/*
 ********
 * Rate *
 ********
 */

double average_rating(Point& U);
double sum_on_neighbors(PreferenceVector& U, std::set<PreferenceVector*>& N);

//normalize
void normalize_PV(PreferenceVector& U, PreferenceVector& N);
void normalize_PVs(std::list<PreferenceVector*>& Us);

//rate
void rate(PreferenceVector& U, std::set<PreferenceVector*>& N_U, PreferenceVector& R_U);
void rate(std::list<PreferenceVector*>& Us, std::list< std::set<PreferenceVector*> >& N_Us, std::list<PreferenceVector*>& R_Us);

//predicates
bool valid_PV(PreferenceVector& PV);

//keep valid PVs
void keep_valid_PVs(std::list<PreferenceVector*>& PVs);



/*
 *****************
 * Recommend Top *
 *****************
 */


void recommend_top_items(
							PreferenceVector& PV,
							int top_k,
							int RecommendedItems[],
							int RecommendedItems_array_size	
						);

#endif
