#include <iostream>

#include <cassert>

//constructor destructor
template <class LSHParameterVector>
LSHsVector<LSHParameterVector>::LSHsVector(int LL, RandomGenerator& GGen, int size, int dim, int ww /*= 0*/){
	assert(LL >= 1);	L = LL;
	
	V = new LSHParameterVector*[L];
	for(int i = 0; i <= (L-1); i++)  V[i] = new LSHParameterVector(GGen, size, dim, ww);
}

template <class LSHParameterVector>
LSHsVector<LSHParameterVector>::~LSHsVector(){
	for(int i = 0; i <= (L-1); i++)  delete V[i];
	delete[] V;
}

//debug
template <class LSHParameterVector>
void LSHsVector<LSHParameterVector>::print(){
	for(int i = 0; i <= (L-1); i++){
		std::cout << "Parameter Vector #" << (i+1) << std::endl;
		V[i]->print();
		std::cout << std::endl << std::endl;
	}
}

//hash functions
template <class LSHParameterVector>
void LSHsVector<LSHParameterVector>::phis(Point& P, int phis_vector[], int array_size, int hash_table_size /*= 0 for compatability purpouses only*/){
	assert(phis_vector != NULL);
	assert(array_size == L);
	
	for(int i = 0; i <= (L-1); i++)  phis_vector[i] = V[i]->phi(P, hash_table_size);
}
