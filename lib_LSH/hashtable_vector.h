#ifndef HASHTABLE_VECTOR_H

#define HASHTABLE_VECTOR_H

#include <list>
#include <set>

#include "hash_table.h"
#include "../lib_math/point.h"


class HashTableVector{
private:
	HashTable** V;
	int L;
	int tables_size;
public:
//constructor destructor	
HashTableVector(int LL, int table_size, int dim);
~HashTableVector();

//debug
void print();

//operations
/* save the Point P in the l-th hash table, in the h-th bucket*/
void save_point_to_bucket(Point* P, int l, int h);

/* saves the Point P  in the L HashTables pointed by the Ls*/
void save_point_to_bucket(Point* P, int Ls[], int array_size);

/* Applies the Exact NN algorithm in the l-th hash table, in the h-th bucket */
double get_NN_opt(
					Point& Q,									//Query Point
					Point& W,									//Witness
					double (*metric) (Point& P1, Point& P2),	//metric function
					int l,
					int h
				);
	
/* Applies Exact NN in all the L hashtables in the buckets pointed
 * by the Ls.*/
double get_NN_opt(
					Point& Q,									//Query Point
					Point& W,									//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					int Ls[],									//array of buckets
					int array_size
				);
	
/* Applies the Exact R-N algorithm in the l-th hash table, in the h-th bucket */
bool get_NN_dec(
					Point& Q,									//Query Point
					std::list<Point*>& WitnessList,				//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					double radius, 								//radius
					int l,
					int h
				);
	
	/* Applies Exact R-N in all the L hashtables in the buckets pointed
	 * by the Ls.*/
bool get_NN_dec(
					Point& Q,									//Query Point
					std::list<Point*>& WitnessList,				//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					double radius, 								//radius
					int Ls[],									//array of buckets
					int array_size
				);

void get_k_NN(
				Point& Q,							//Query Point
				std::set<Point*>& Witnesses,		//Witnesses
				double (*metric)(Point&, Point&),	//metric
				int k,								//num of neighbors
				int l,
				int h
			);
				
void get_k_NN(
				Point& Q,							//Query Point
				std::set<Point*>& Witnesses,		//Witnesses
				double (*metric)(Point&, Point&),	//metric
				int k,								//num of neighbors
				int Ls[],
				int array_size
			);

};


#endif
