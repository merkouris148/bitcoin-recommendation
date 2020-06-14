#include "hashtable_vector.h"

#include <cassert>
#include <cstdlib>

#include <iostream>
#include <list>
#include <map>

#include "../lib_containers/array_operations.h"

using namespace std;

//constructor destructor
HashTableVector::HashTableVector(int LL, int table_size, int dim){
	assert(LL >= 1);			L 			= LL;
	assert(table_size >= 2);	tables_size = table_size;
	assert(dim >= 1);
	
	V = new HashTable* [L];
	for(int i = 0; i <= (L-1); i++)	 V[i] = new HashTable(table_size, dim);
}

HashTableVector::~HashTableVector(){
	for(int i = 0; i <= (L-1); i++)	 delete V[i];
	delete[] V;
}

//debug

void HashTableVector::print(){
	for(int l = 0; l <= (L-1); l++){
		std::cout << std::endl << "Hash Table #" << (l+1) << std::endl;
		V[l]->print();
	}
}

//operations
/* save the Point P in the l-th hash table, in the h-th bucket*/
void HashTableVector::save_point_to_bucket(Point* P, int l, int h){
	assert(P != NULL);
	assert(0 <= l && l <= (L-1));
	//assert(0 <= h && h <= (tables_size-1));
	
	V[l]->save_point_to_bucket(P, h);
}

/* saves the Point P  in the L HashTables pointed by the Ls*/
void HashTableVector::save_point_to_bucket(Point* P, int Ls[], int array_size){
	assert(Ls != NULL);
	assert(array_size == L);
	
	for(int l = 0; l <= (L-1); l++) save_point_to_bucket(P, l, Ls[l]);
}

/*
set<Point*> HashTableVector::get_bucket(int l, int h){
	assert(0 <= l && l <= (L-1));
	
	return V[l]->get_bucket(h);
}

set<Point*> HashTableVector::get_bucket(int Ls[], int array_size){
	assert(Ls != NULL);
	assert(array_size == L);
	
	set<Point*> Ps_out;
	for(int l = 0; l <= array_size-1; ++l){
		set<Point*> Ps_in = get_bucket(l, Ls[l]);
		Ps_out.insert(Ps_in.begin(), Ps_in.end());
	}
	
	return Ps_out;
}
*/

/* Applies the Exact NN algorithm in the l-th hash table, in the h-th bucket */
double HashTableVector::get_NN_opt(
					Point& Q,									//Query Point
					Point& W,									//Witness
					double (*metric) (Point& P1, Point& P2),	//metric function
					int l,
					int h
				){
	assert(0 <= l && l <= (L-1));
	//assert(0 <= h && h <= (tables_size-1));
	
	return V[l]->get_NN_opt(Q, W, metric, h);				
}

/* Applies Exact NN in all the L hashtables in the buckets pointed
	 * by the Ls.*/
double HashTableVector::get_NN_opt(
					Point& Q,									//Query Point
					Point& W,									//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					int Ls[],									//array of buckets
					int array_size
				){
	assert(Ls != NULL);
	assert(array_size == L);

	
	//initialize witnesses
	Point** Ws = new Point* [array_size]; for(int i = 0; i <= (array_size-1); i++)  Ws[i] = new Point(Q.get_dim());
	
	//Run exact LSH for each bucket in H
	int D[array_size];  for(int i = 0; i <= (array_size-1); i++)  D[i] = get_NN_opt(Q, *Ws[i], metric, i, Ls[i]);
	
	//set minimum distance and witness
	int d, j; array_find_min(D, array_size, d, j);
	W.set_point(*Ws[j]);
	
	//delete witnesses
	for(int i = 0; i <= (array_size-1); i++) delete Ws[i];
	delete[] Ws;
	
	
	return d;				
}

/* Applies the Exact R-N algorithm in the l-th hash table, in the h-th bucket */
bool HashTableVector::get_NN_dec(
					Point& Q,									//Query Point
					std::list<Point*>& WitnessList,				//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					double radius, 								//radius
					int l,
					int h
				){
	assert(0 <= l && l <= (L-1));
	//assert(0 <= h && h <= (tables_size-1));
	
	return V[l]->get_NN_dec(Q, WitnessList, metric, radius, h);				
}

/* Applies Exact R-N in all the L hashtables in the buckets pointed
	 * by the Ls.*/
bool HashTableVector::get_NN_dec(
					Point& Q,									//Query Point
					std::list<Point*>& WitnessList,				//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					double radius, 								//radius
					int Ls[],									//array of buckets
					int array_size
				){
	assert(Ls != NULL);
	assert(array_size == L);
	
	//initialize witnesses
	std::list<Point*>* Ws[array_size]; for(int i = 0; i <= (array_size-1); i++)  Ws[i] = new std::list<Point*>;
	
	//Run exact LSH (decision with radius R) for each bucket in H
	bool B[array_size];  for(int i = 0; i <= (array_size-1); i++)  B[i] = get_NN_dec(Q, *Ws[i], metric, radius, i, Ls[i]);
	
	//set b = true, iff there where any neighbors in any bucket h in H
	bool b = bool_array_or(B, array_size);
	//concat all the partial witness lists
	list_array_concat(Ws, array_size, WitnessList);


	//delete witnesses
	for(int i = 0; i <= (array_size-1); i++){ Ws[i]->clear(); delete Ws[i]; }
	
	
	WitnessList.sort(ptr_lexicograpgical_less);
	WitnessList.unique(ptr_equal);
	
	return b;				
}


void HashTableVector::get_k_NN(
								Point& Q,							//Query Point
								std::set<Point*>& Witnesses,		//Witnesses
								double (*metric)(Point&, Point&),	//metric
								int k,								//num of neighbors
								int l,
								int h
							){
	assert(0 <= l && l <= (L-1));
				
	V[l]->get_k_NN(Q, Witnesses, metric, k, h);			
}


void HashTableVector::get_k_NN(
								Point& Q,							//Query Point
								std::set<Point*>& Witnesses,		//Witnesses
								double (*metric)(Point&, Point&),	//metric
								int k,								//num of neighbors
								int Ls[],
								int array_size
							){
	assert(Ls != NULL);
	assert(array_size == L);
	
	//debug
	/*cout << endl;
	cout << endl;
	cout << "hash table vector" << endl;
	array_print(Ls, array_size); cout << endl;
	cout << endl;
	cout << endl;
	*/
	
	set<Point*> Witnesses_in;
	for(int i = 0; i <= L-1; ++i)  V[i]->get_k_NN(Q, Witnesses_in, metric, k, Ls[i]);
	
	//debug
	/*cout << endl;
	cout << endl;
	cout << "hash table vector Witness in size: " << Witnesses_in.size() << endl;
	cout << endl;
	cout << endl;
	*/
	
	//if there at most k neighbors found, return
	if(Witnesses_in.size() <= k){  Witnesses = Witnesses_in; return; }
	
	
	//else reduce the neighbors, keep the nearest
	multimap<double, Point*> k_NN_DS;
	for(auto W : Witnesses_in) k_NN_DS.insert(pair<double, Point*>(metric(*W, Q), W));
	
	int i = 1;
	for(auto it = k_NN_DS.begin(); it != k_NN_DS.end() && i <= k; ++it, ++i)  Witnesses.insert(it->second);
}

