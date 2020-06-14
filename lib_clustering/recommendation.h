#ifndef RECOMMENDATION_H

#define RECOMMENDATION_H

#include "../lib_math/point.h"
#include "../lib_LSH/cosine_lsh.h"
#include "../lib_LSH/LSHsVector.h"
#include "../lib_LSH/hashtable_vector.h"
#include "../lib_math/random.h"
#include "clustering_data_structure.h"
#include "clustering_algorithms.h"

/*
 ****************************
 * LSH Create Neighborhoods *
 ****************************
 */

class LSHCreateNeighborhoods{
private:
	LSHsVector<CosineParameterVector> LSH;
	HashTableVector HashTable;
	
	int L;
	
	int P;
public:
//constructor destructor
	LSHCreateNeighborhoods(
							int LL,
							int k,
							int dim,
							int hashtable_size,
							
							RandomGenerator& Gen,
							
							std::list<Point*>& Users,		//neighbors pool
							
							int PP
						);
	//~LSHCreateNeighborhoods();

//functor
	void operator()(std::list<Point*>& Users, std::list< std::set<Point*> >& N_Users);
};


/*
 ***********************************
 * Clustering Create Neighborhoods *
 ***********************************
 */

class ClustreringCreateNeighborhoods{
private:
	ClusteringDataStructure CDS;
	ClusteringAlgorithm& CA;
	int P;
public:
//constructor destructor
	ClustreringCreateNeighborhoods(
									int kk,						//# clusters
									int ddim,
									double (*m) (Point& X, Point& Y),
									
									std::list<Point*>& Users,	//neighbors pool
									
									ClusteringAlgorithm& CCA,
									
									int PP
								);
	//~ClustreringCreateNeighborhoods();

//functor
	void operator()(std::list<Point*>& Users, std::list< std::set<Point*> >& N_Users);

};



#endif
