#include "clustering_algorithms.h"

#include <iostream>

#include "../lib_containers/array_operations.h"

using namespace std;

const bool debug = true;

//initialization
void set_rand_centroids(ClusteringDataStructure& CDS, RandomGenerator& Gen){
	for(int i = 1; i <= CDS.get_k(); i++) CDS.set_centroid_rand(i, Gen);
}

void k_means_pp(ClusteringDataStructure& CDS, RandomGenerator& Gen){
	//Choose a centroid uniformly at random 
	int ran = Gen.uniform_get_integer(0, CDS.get_num_of_points());
	//cout << "ran: " << ran << endl;
	CDS.set_centroid( 1,  ran);
	
	int k = CDS.get_k();
	int n = CDS.get_num_of_points();
	
	for(int t = 1; t <= (k-1); t++){
		//compute D array
		int array_size = n-t;
		double D[array_size];
		CDS.distance_from_centroids(D, array_size, t);
		
		//normalize D array
		/*
		double max_D; array_find_max(D, array_size, max_D);
		
		assert(max_D != 0);
		array_multiply(D, array_size, 1 / max_D);
		*/
		
		//compute P array
		double P[array_size];
		for(int r = 0; r <= (array_size- 1); r ++) P[r] = array_sum_of_squares(D, (r+1));
		double x = Gen.uniform_get_real(P[0], P[array_size-1]);
		int r = array_binary_find(P, 0, array_size - 1, x);
		CDS.set_centroid((t+1), r);
	}
}

//assignment
bool loyds_assignment(ClusteringDataStructure& CDS){
	bool changed = false;
	
	for(auto p = CDS.begin(); p != CDS.end(); p++){
		int c; CDS.arg_distance_from_centroids(*get_point(p), c);
		
		bool changed_new = CDS.set_cluster(p, c); changed = changed or changed_new;
	}
	
	return changed;
}

//update
bool k_means(ClusteringDataStructure& CDS){
	bool changed = false;
	
	for(int c = 1; c <= CDS.get_k(); c++){
		Point M(CDS.get_dim()); CDS.get_mean(c, M);
		
		bool changed_new = CDS.set_centroid(c, M); changed = changed or changed_new;
	}
	
	return changed;
}

bool loyd_pam(ClusteringDataStructure& CDS){
	bool changed = false;
	
	for(int c = 1; c <= CDS.get_k(); c++){
		Point M(CDS.get_dim()); CDS.get_medoid(c, M);
		
		bool changed_new = CDS.set_centroid(c, M); changed = changed or changed_new;
	}
	
	return changed;
}

/*
 ************************
 * Clustering Algorithm *
 ************************
*/

ClusteringAlgorithm::ClusteringAlgorithm(
											void (*init)(ClusteringDataStructure&, RandomGenerator&),
											bool (*a)(ClusteringDataStructure&),
											bool (*up)(ClusteringDataStructure&),
							
											RandomGenerator& GGen,
											int nnumber_of_iterations
										) :
											initialize(init),
											assign(a),
											update(up),
											
											Gen(GGen),
											number_of_iterations(nnumber_of_iterations){}


//clustering algorithm
void ClusteringAlgorithm::operator ()(ClusteringDataStructure& CDS){
	assert(number_of_iterations >= 1);
	
	initialize(CDS, Gen);
	int i = 1;
	bool clustering_changed = true;
	while(i <= number_of_iterations && clustering_changed){
		bool assignment_changed = assign(CDS);
		bool centroids_changed 	= update(CDS);
		
		clustering_changed = assignment_changed and centroids_changed;
		
		i++;
	}						
}
