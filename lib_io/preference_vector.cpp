#include "preference_vector.h"

#include <cmath>
#include <cassert>

#include "../lib_containers/array_operations.h"

using namespace std;

//constructor destructor
PreferenceVector::PreferenceVector(
									ItemsDictionary& Items,
									Sentiment& Sent,
						
									std::list<std::string>& terms,
									
									const char* iid /*= ""*/
								) : 
									Point(Items.get_n_items(), iid)
								{
	
	//get the references to items
	for(auto it = terms.begin(); it != terms.end(); ++it){
		
		if(Items.member(*it)) references.insert(Items[*it]);
	}
	
	//totalscore
	double totalscore = 0;
	for(auto it = terms.begin(); it != terms.end(); ++it) totalscore += Sent[*it];
	
	//sentiment
	double sentiment = totalscore / sqrt(pow(totalscore, 2) + alpha);
	
	//vector
	for(auto it = references.begin(); it != references.end(); ++it) set_vi(*it, 1);
	scalar_product(sentiment);
}

PreferenceVector::PreferenceVector(
						ItemsDictionary& Items,
						Sentiment& Sent,
						
						const char* iid /*= ""*/
					) :
						Point(Items.get_n_items(), iid){}

PreferenceVector::PreferenceVector(int dim, const char* iid /*= ""*/) :	Point(dim, iid){}


//debug
void PreferenceVector::print(ostream& out /*= cout*/){
	//derived info
	out << "references" << endl;
	out << get_references(); out << endl; out << endl;
	
	/*out << "total_score" << endl;
	out << get_totalscore(); out << endl; out << endl;
	
	out << "sentiment" << endl;
	out << get_sentiment(); out << endl; out << endl;
	*/
	
	out << "vector" << endl;
	Point::print(); out << endl; out << endl;
}

//accessors
set<int> PreferenceVector::get_references(){
	return references;
}

int PreferenceVector::get_n_references(){
	return references.size();
}

//operations
void pref_v_add(PreferenceVector& PV1, PreferenceVector& PV2, PreferenceVector& PV){
	//add points
	point_add(PV1, PV2, PV);
	
	//PV.references = PV1.references U PV2.references
	for(auto r : PV1.references) PV.references.insert(r);
	for(auto r : PV2.references) PV.references.insert(r);
}

//show
ostream& operator<<(ostream& out, const set<int>& s){
	out << "{";
	for(auto it = s.begin(); it != s.end(); ++it) out << *it << ", ";
	out << "}";
	
	return out;
}




/*
 ********
 * Rate *
 ********
 */

double sum_on_neighbors(PreferenceVector& U, set<PreferenceVector*>& Vs){
	double S = 0;
	for(auto v : Vs) S += point_cos(U, *v);
	
	assert(S != 0);
	return 1/S;
}

double average_rating(PreferenceVector& U){
	int dim 			= U.get_dim();
	int refered_coins 	= U.get_n_references(); 
	
	Point Y(dim); Y.set_all_vis(1);
	
	assert(refered_coins != 0);
	return inner_product(U, Y)/refered_coins;
}


//normalize
void normalize_PV(PreferenceVector& U, PreferenceVector& N){
	assert(U.get_dim() == N.get_dim()); int dim = U.get_dim();
	
	double m = average_rating(U);
	
	for(int i = 1; i <= dim; ++i)  if(U.get_vi(i) != 0)  N.set_vi(i, U.get_vi(i) - m );
}


void normalize_PVs(list<PreferenceVector*>& Us){
	for(auto u : Us)  normalize_PV(*u, *u);
}


//rate
void rate(PreferenceVector& U, set<PreferenceVector*>& Vs, PreferenceVector& R){
	assert(U.get_dim() == R.get_dim()); int dim = U.get_dim();
	assert(!Vs.empty());
	
	double z = sum_on_neighbors(U, Vs);
	
	for(int i = 1; i <= dim; ++i){
		double S = 0;
		for(auto v : Vs){
			//debug
			//Point Zero(dim);
			//assert(!equal(*v, Zero));
			
			S += point_cos(U, *v) * v->get_vi(i);
		}
		R.set_vi(i, z * S);
	}
}

void rate(list<PreferenceVector*>& Us, list< set<PreferenceVector*> >& N_Us, list<PreferenceVector*>& R_Us){
	PreferenceVector* U = Us.front(); int dim = U->get_dim();
	
	//traverse both users list and users neighbors list
	auto Us_it 		= Us.begin();
	auto N_Us_it	= N_Us.begin();
	
	for(; Us_it != Us.end() && N_Us_it != N_Us.end(); ++Us_it, ++N_Us_it){
		
		PreferenceVector* R = new PreferenceVector(dim, (*Us_it)->get_id());
		
		rate(**Us_it, *N_Us_it, *R);  R_Us.push_back(R);
	}
	
	
	//check if both list ended
	assert(Us_it == Us.end() && N_Us_it == N_Us.end());
}


//predicates
bool valid_PV(PreferenceVector& PV){
	return PV.get_n_references() != 0;
}

//keep valid PVs
void keep_valid_PVs(list<PreferenceVector*>& PVs){
	
	list< list<PreferenceVector*>::iterator > to_be_erased;
	for(auto it = PVs.begin(); it != PVs.end(); ++it)  if(!valid_PV(**it))  to_be_erased.push_back(it);
	
	for(auto err = to_be_erased.begin(); err != to_be_erased.end(); ++err){
		delete **err;
		PVs.erase(*err);
	}
}

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
						){
	
	assert(RecommendedItems != NULL);
	assert(top_k >= 1);
	assert(RecommendedItems_array_size >= top_k);						
	
	
	int dim = PV.get_dim();
	double point_array[dim];  PV.get_array(point_array, dim);
		
	set<int> items = PV.get_references();
	for(auto i : items) point_array[i-1] = -HUGE_VAL;
		
	double M[top_k];
		
	double_array_find_k_max(
							point_array, dim,
							top_k,
							M, top_k,
							RecommendedItems, top_k
						);
						
	//items id >= 1;
	for(int i = 0; i <= top_k-1; ++i) RecommendedItems[i] += 1;
}

