#ifndef CLUSTERING_ALGORITHMS_H

#define CLUSTERING_ALGORITHMS_H

#include "clustering_data_structure.h"

#include <fstream>
#include <iostream>

/* Implemantaion of functions of the form:
 * 	f: CDS --> CDS*/

//initialization
void set_rand_centroids(ClusteringDataStructure& CDS, RandomGenerator& Gen);
void k_means_pp(ClusteringDataStructure& CDS, RandomGenerator& Gen);

//assignment
bool loyds_assignment(ClusteringDataStructure& CDS);

//update
bool k_means(ClusteringDataStructure& CDS);
bool loyd_pam(ClusteringDataStructure& CDS);



/*
 ************************
 * Clustering Algorithm *
 ************************
*/


class ClusteringAlgorithm{
private:
	void (*initialize)(ClusteringDataStructure&, RandomGenerator&);
	bool (*assign)(ClusteringDataStructure&);
	bool (*update)(ClusteringDataStructure&);
	
	RandomGenerator& Gen;
	
	int number_of_iterations;
public:
	ClusteringAlgorithm(
							void (*init)(ClusteringDataStructure&, RandomGenerator&),
							bool (*a)(ClusteringDataStructure&),
							bool (*up)(ClusteringDataStructure&),
							
							RandomGenerator& GGen,
							int nnumber_of_iterations
						);

//functor
	void operator()(ClusteringDataStructure& CDS);	
};

#endif
