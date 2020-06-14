#ifndef LSHPARAMETERVECTOR_OPERATIONS_H
 
#define LSHPARAMETERVECTOR_OPERATIONS_H

#include "../lib_math/point.h"
#include "hash_table.h"
#include "euclidean_lsh.h"
#include "cosine_lsh.h"

#include <list>

//LSHParameterVector = EuclideanParameterVector | CosineParameterVector

template <class LSHParaeterVector>
void save_point_to_bucket(
							HashTable& H,
							LSHParaeterVector& V,
							Point* P,
							int hash_table_size
						);

template <class LSHParaeterVector>
double get_NN_opt(
					HashTable& H,
					LSHParaeterVector& V,
					Point& Q,
					Point& W,
					double (*metric) (Point& P1, Point& P2),
					int hash_table_size
				);

template <class LSHParaeterVector>
bool get_NN_dec(
					HashTable& H,
					LSHParaeterVector& V,
					Point& Q,									//Query Point
					std::list<Point*>& WitnessList,					//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					double radius, 								//radius
					int hash_table_size
				);


#include "LSHParameterVector_operations.tpp"

#endif
