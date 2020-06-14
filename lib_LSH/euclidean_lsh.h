#ifndef EUCLIDEAN_LSH_H

#define EUCLIDEAN_LSH_H

#include <list>

#include "../lib_math/point.h"
#include "../lib_math/random.h"
#include "hash_table.h"


class EuclideanParameters;

class EuclideanParameterVector{
private:	
	EuclideanParameters** V;
	int vector_size;
	int w;	

	RandomGenerator& Gen;
public:
//constructor - destructor
	EuclideanParameterVector(RandomGenerator& GGen, int size, int dim, int ww);
	~EuclideanParameterVector();
//debug
	void print();
//accessors
	int get_vector_size();
//hash functions
	int h(int i, Point& P);
	void g(Point& P, Point& G);
	int phi(Point& P, int hash_table_size);
};

#endif
