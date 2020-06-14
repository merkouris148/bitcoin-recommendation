#include <cassert>


template <class LSHParaeterVector>
void save_point_to_bucket(
							HashTable& H,
							LSHParaeterVector& V,
							Point* P,
							int hash_table_size
						){
	assert(P != NULL);
	
	H.save_point_to_bucket(P, V.phi(*P, hash_table_size));							
}

template <class LSHParaeterVector>
double get_NN_opt(
					HashTable& H,
					LSHParaeterVector& V,
					Point& Q,
					Point& W,
					double (*metric) (Point& P1, Point& P2),
					int hash_table_size
				){
					
	return H.get_NN_opt(Q, W, metric, V.phi(Q, hash_table_size));				
}

template <class LSHParaeterVector>
bool get_NN_dec(
					HashTable& H,
					LSHParaeterVector& V,
					Point& Q,									//Query Point
					std::list<Point*>& WitnessList,					//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					double radius, 								//radius
					int hash_table_size
				){
					
	return H.get_NN_dec(Q, WitnessList, metric, radius, V.phi(Q, hash_table_size));
}
