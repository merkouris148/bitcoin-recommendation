#ifndef LSHS_VECTOR_H

#define LSHS_VECTOR_H

#include "../lib_math/point.h"

template <class LSHParameterVector>
class LSHsVector{
private:	
	LSHParameterVector** V;
	int L;
public:
//constructor destructor
	LSHsVector(int L, RandomGenerator& GGen, int size, int dim, int ww = 0);
	//LSHsVector(int L, RandomGenerator& GGen, int size, int dim);
	~LSHsVector();
//debug
	void print();
//hash functions
	void phis(Point& P, int phis_vector[], int array_size, int hash_table_size = 0);
	//void phis(Point& P, int phis_vector[], int array_size);
};


#include "LSHsVector.tpp"

#endif
