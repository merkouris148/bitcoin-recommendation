#include "../lib_containers/array_operations.h"
#include "../lib_containers/list_operations.h"
#include "../lib_math/integer.h"


//Hash Table wraps
template <class LSHParameterVector>
void Cube_save_point_to_bucket(
									HashTable& H,
									CubeParameterVector& C,
									LSHParameterVector& LSH,
									Point* P,
									int hash_table_size
								){
	assert(P != NULL);
	assert(hash_table_size == int_pow(2, C.get_vector_size()));
	
	Point G(LSH.get_vector_size()); LSH.g(*P, G);
	H.save_point_to_bucket(P, C.phi(G, hash_table_size));								

}

template <class LSHParameterVector>
double Cube_get_NN_opt(
							HashTable& H,
							CubeParameterVector& C,
							HyperCubeGraph& G,
							int probes,
							LSHParameterVector& LSH,
							Point& Q,
							Point& W,
							double (*metric) (Point& P1, Point& P2)
						){
	assert(H.get_table_size() == int_pow(2, C.get_vector_size()));
	assert(C.get_vector_size() == G.get_deg());
	assert(probes >= 1);
	
	//get the necessary constants
	int hash_table_size 	= H.get_table_size();
	int deg					= G.get_deg();
	int array_size			= min(deg, probes) + 1;		//we will search min{deg, probes} neighbors + 1, the bucket that the query point is hashed
	int LSH_vector_size		= LSH.get_vector_size();
	
	//φ_cube(P) = ψ_cube(φ_lsh(P)) = (ψ_cube o φ_lsh) (P)
	Point J(LSH_vector_size); LSH.g(Q, J);
	int h = C.phi(J, hash_table_size);
	
	
	int N[deg];  G.get_neighbors(h, N);								//get the neighbors in graph
	int M[array_size];												//get the search space M,
	array_get_subarray(N, deg, M, array_size, 0, (array_size-2));	//array M contains the buckets tha must be searched,
																	//namely N(h) U {h = φ_cube(Q)}
	M[array_size - 1] = h;											//the last bucket searched is φ_cube(Q)

	
	return H.get_NN_opt(Q, W, metric, M, array_size);
}

template <class LSHParameterVector>
bool Cube_get_NN_dec(
						HashTable& H,
						CubeParameterVector& C,
						HyperCubeGraph& G,
						int probes,
						LSHParameterVector& LSH,
						Point& Q,
						std::list<Point*>& WitnessList,
						double (*metric) (Point& P1, Point& P2),
						double radius
					){
	assert(H.get_table_size() == int_pow(2, C.get_vector_size()));
	assert(C.get_vector_size() == G.get_deg());
	assert(probes >= 1);
	
	//get the necessary constants
	int hash_table_size 	= H.get_table_size();
	int deg					= G.get_deg();
	int array_size			= min(deg, probes) + 1;		//we will search min{deg, probes} neighbors + 1, the bucket that the query point is hashed
	int LSH_vector_size		= LSH.get_vector_size();
	
	//φ_cube(P) = ψ_cube(φ_lsh(P)) = (ψ_cube o φ_lsh) (P)
	Point J(LSH_vector_size); LSH.g(Q, J);
	int h = C.phi(J, hash_table_size);
	
	
	int N[deg];  G.get_neighbors(h, N);								//get the neighbors in graph
	int M[array_size];												//get the search space M,
	array_get_subarray(N, deg, M, array_size, 0, (array_size-2));	//array M contains the buckets tha must be searched,
																	//namely N(h) U {h = φ_cube(Q)}
	M[array_size - 1] = h;											//the last bucket searched is φ_cube(Q)

	
	return H.get_NN_dec(Q, WitnessList, metric, radius, M, array_size);	
}
