#ifndef COSINE_LSH_H

#define COSINE_LSH_H

#include <list>

#include "../lib_math/point.h"
#include "../lib_math/random.h"
#include "hash_table.h"

class CosineParameters;

class CosineParameterVector{
private:
	CosineParameters** V;
	int vector_size;
	
	RandomGenerator& Gen;
public:
//constuctor - destructor
	CosineParameterVector(RandomGenerator& GGen, int size, int dim, int ww = 0 /*for compatability purpouses only */);
	~CosineParameterVector();
//debug
	void print();
//accessors
	int get_vector_size();
//hash fuctions
	int h(int i, Point& P);
	void g(Point& P, Point& G);
	int phi(Point& P, int hash_table_size = 0 /*for compatability purpouses only*/);
};

//Hash Table wraps for Cosine LSH
/*
void Cosine_save_point_to_bucket(
									HashTable& H,
									CosineParameterVector& V,
									Point* P,
									int hash_table_size
								);

double Cosine_get_NN_opt(
							HashTable& H,
							CosineParameterVector& V,
							Point& Q,
							Point& W,
							int hash_table_size
						);

bool Cosine_get_NN_dec(
						HashTable& H,
						CosineParameterVector& V,
						Point& Q, std::list<Point*>& WitnessList,
						double radius,
						int hash_table_size
					);
*/

#endif
