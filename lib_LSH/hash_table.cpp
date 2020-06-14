#include <iostream>

#include <cassert>

#include <map>

#include "hash_table.h"
#include "point_list.h"

#include "../lib_containers/array_operations.h"

#include "../lib_math/random.h"
#include "../lib_math/integer.h"

using namespace std;

const bool debug = true;

//constructor destructor
HashTable::HashTable(int TS, int dim){
	assert(TS >= 1);
	
	TableSize = TS;
	
	Array = new PointList* [TS];
	for(int i = 0; i <= (TableSize-1); i++)  Array[i] = new PointList(dim);
}

HashTable::~HashTable(){
	for(int i = 0; i <= (TableSize-1); i++)  delete Array[i];
	delete[] Array;
}

//debug
void HashTable::print(char* separator){
	cout << "Hash Table:" << endl;
	for(int i = 0; i <= (TableSize-1); i++){
		cout << "[" << i << "] ";
		Array[i]->print(", ");
		cout << endl;
	}
}

//accessors
int HashTable::get_table_size(){return TableSize;}

//operations
void HashTable::save_point_to_bucket(Point* P, int h){
	assert(P != NULL);
	
	h = modulo(h, TableSize);
		
	Array[h]->emplace_back(P);
}

/*
set<Point*> HashTable::get_bucket(int h){
	set<Point*> Ps;
	
	h = modulo(h, TableSize);
	
	for(auto it = Array[h]->begin(); it != Array[h]->end(); ++it)  Ps.insert(*it);
	
	return Ps;
}
*/

double HashTable::get_NN_opt(
					Point& Q,									//Query Point
					Point& W,									//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					int h
				){
	
	h = modulo(h, TableSize);				
	return Array[h]->exact_NN_opt(Q, W, metric);
}

/* Applies the Exact NN algorithm in the buckets
	 * pointed by the index array H.
	 * 
	 * Returns the minimum distance and the point W that
	 * achives this distance with query point Q.
	 * 
	 * Namely: let d = get_NN_opt(Q, W, metric, H, array_size)
	 * d = min {d_h | d_h = get_NN_opt(Q, W_h, metric, h), where h in H}
	 * 
	 * Also W is a point s.t. metric(Q, W) = d.*/
double HashTable::get_NN_opt(
					Point& Q,									//Query Point
					Point& W,									//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					int H[],
					int array_size
				){
	assert(H != NULL);
	assert(array_size >= 1);
	
	//initialize witnesses
	Point** Ws = new Point* [array_size]; for(int i = 0; i <= (array_size-1); i++)  Ws[i] = new Point(Q.get_dim());
	
	//Run exact LSH for each bucket in H
	int D[array_size];  for(int i = 0; i <= (array_size-1); i++)  D[i] = get_NN_opt(Q, *Ws[i], metric, H[i]);
	
	//set minimum distance and witness
	int d, j; array_find_min(D, array_size, d, j);
	W.set_point(*Ws[j]);
	
	//delete witnesses
	for(int i = 0; i <= (array_size-1); i++) delete Ws[i];
	delete[] Ws;
	
	
	return d;
}

bool HashTable::get_NN_dec(
					Point& Q,									//Query Point
					list<Point*>& WitnessList,					//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					double radius, 								//radius
					int h
				){

	h = modulo(h, TableSize);				
	return Array[h]->exact_NN_dec(Q, WitnessList, metric, radius);				
}

/* Applies the Exact R-N algorithm in the buckets
	 * pointed by the index array H.
	 * 
	 * Returns true if there is at least one Point P s.t.
	 * metric(Q, p) <= R, R > 0, else return false.
	 * 
	 * Also return in the argument WitnessList a list of the
	 * Points tha satisfy the above criterion*/
bool HashTable::get_NN_dec(
					Point& Q,									//Query Point
					list<Point*>& WitnessList,					//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					double radius, 								//radius
					int H[],
					int array_size
				){
	assert(H != NULL);
	assert(array_size >= 1);
	
	//initialize witnesses
	list<Point*>* Ws[array_size]; for(int i = 0; i <= (array_size-1); i++)  Ws[i] = new list<Point*>;
	
	//Run exact LSH (decision with radius R) for each bucket in H
	bool B[array_size];  for(int i = 0; i <= (array_size-1); i++)  B[i] = get_NN_dec(Q, *Ws[i], metric, radius, H[i]);
	
	//set b = true, iff there where any neighbors in any bucket h in H
	bool b = bool_array_or(B, array_size);
	//concat all the partial witness lists
	list_array_concat(Ws, array_size, WitnessList);
	
	//delete witnesses
	for(int i = 0; i <= (array_size-1); i++){ Ws[i]->clear(); delete Ws[i]; }
	
	
	return b;
}

//set<Point*> operations
void HashTable::get_k_NN(
							Point& Q,
							std::set<Point*>& Witnesses,
							double (*metric)(Point&, Point&),
							int k,
							int h
						){
	assert(k >= 1);
	
	h = modulo(h, TableSize);
	
	//map sorts (dist, point) pairs by dist
	multimap<double, Point*> k_NN_DS;
	for(auto it = Array[h]->begin(); it != Array[h]->end(); ++it)  k_NN_DS.insert(pair<double, Point*>(metric(**it, Q), *it));
	
	
	//pick the nearest k points
	int i = 1;
	for(auto it = k_NN_DS.begin(); it != k_NN_DS.end() && i <= k; ++it, ++i)  Witnesses.insert(it->second);
}
