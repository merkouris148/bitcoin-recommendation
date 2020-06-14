#ifndef CUBE_LSH_H

#define CUBE_LSH_H

#include <list>

#include "../lib_math/point.h"
#include "../lib_math/random.h"
#include "../lib_math/hypercube_graph.h"
#include "hash_table.h"


#include "euclidean_lsh.h"
#include "cosine_lsh.h"

typedef int CubeParameters;

class CubeParameterVector{
private:
	CubeParameters* V;
	int vector_size;
	
	RandomGenerator& Gen;
public:
//constuctor - destructor
	CubeParameterVector(RandomGenerator& GGen, int size, int dim, int ww = 0);
	~CubeParameterVector();
//debug
	void print();
//accessors
	int get_vector_size();
//hash fuctions
	void g(Point& Gin, Point& Gout);
	int phi(Point& Gin, int hash_table_size = 0); //for compatability purpouses only
};

//Hash Table wraps for Cosine LSH
/* 
 * An LSHParameterVector is a class that
 * has a g method s.t. void g(Point& P, Point& G),
 * an implemetation of a function g: R^d --> R^k,
 * where k is the (parameter) vector_size.
 */
 //Hash Table wraps
template <class LSHParameterVector>
void Cube_save_point_to_bucket(
									HashTable& H,
									CubeParameterVector& V,
									LSHParameterVector& LSH,
									Point* P,
									int hash_table_size
								);

template <class LSHParameterVector>
double Cube_get_NN_opt(
							HashTable& H,
							CubeParameterVector& V,
							HyperCubeGraph& G,
							int probes,
							LSHParameterVector& LSH,
							Point& Q,
							Point& W,
							int hash_table_size
						);

template <class LSHParameterVector>
bool Cube_get_NN_dec(
						HashTable& H,
						CubeParameterVector& V,
						HyperCubeGraph& G,
						int probes,
						LSHParameterVector& LSH,
						Point& Q,
						std::list<Point*>& WitnessList,
						double radius,
						int hash_table_size
					);


#include "cube_lsh.tpp"

#endif
