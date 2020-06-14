#include <iostream>

#include <cassert>
#include <cstdlib>

#include "cosine_lsh.h"
#include "hash_table.h"
#include "../lib_math/point.h"

using namespace std;

//debug
const bool debug = false;

/*
**************
* Parameters *
**************
*/

class CosineParameters{
private:
	Point* r;
public:
//constructor destuctor
	CosineParameters(int dim, RandomGenerator& Gen){		
		r = new Point(dim);  r->set_normal(Gen);
	};
	~CosineParameters(){
		delete r;
	};
//debuging
	void print(){ cout << "r: "; r->print(); cout << endl;};
//accessors
	Point& get_r()  {return *r;};
};

/*
 *******
 * LSH *
 *******
*/

//Euclidean LSH
//constructor destructor
CosineParameterVector::CosineParameterVector(RandomGenerator& GGen, int size, int dim, int ww /*= 0*/) : 
	Gen(GGen){

	assert(size >= 1);	vector_size = size;
	assert(dim >= 1);
	
	//initialize vector_size his, h1, h2, .., hk
	//we initialize vector_size hs, by initializing vector_size h parameters
	V = new CosineParameters* [vector_size];
	for(int  i = 0; i <= (vector_size-1); i++)	V[i] = new CosineParameters(dim, Gen);
}

CosineParameterVector::~CosineParameterVector(){
	//delete the vector_size hs
	//delete the V
	for(int i = 0; i <= vector_size-1; i++)  delete V[i];
	delete[] V;
}

//debug
void CosineParameterVector::print(){	
	for(int i = 0; i <= (vector_size-1); i++) V[i]->print();
}

//accessors
int CosineParameterVector::get_vector_size(){
	return vector_size;
}

//hash functions
int CosineParameterVector::h(int i, Point& P){
	if(inner_product(P, V[i]->get_r()) >= 0) 	return 1;
	else  										return 0;
}

void CosineParameterVector::g(Point& P, Point& G){
	assert(G.get_dim() == vector_size);
	
	for(int i = 0; i <= (vector_size-1); i++)  G.set_vi((i+1), h(i, P));
}

int CosineParameterVector::phi(Point& P, int hash_table_size /*= 0 for compatability purpouses only*/){
	Point G(vector_size); g(P, G);
	
	Point B(vector_size);  B.set_binary();
	
	return inner_product(G, B);
}

//Hash Table wraps for Cosine LSH
/*
void Cosine_save_point_to_bucket(
									HashTable& H,
									CosineParameterVector& V,
									Point* P,
									int hash_table_size
								){
	assert(P != NULL);
	assert(hash_table_size == int_pow(2, V.get_vector_size()));
	
	H.save_point_to_bucket(P, V.phi(*P, hash_table_size));
}

double Cosine_get_NN_opt(
											HashTable& H,
											CosineParameterVector& V,
											Point& Q,
											Point& W,
											int hash_table_size
										){
	assert(hash_table_size == int_pow(2, V.get_vector_size()));
	
	return H.get_NN_opt(Q, W, cosine_dist, V.phi(Q, hash_table_size));
}

bool Cosine_get_NN_dec(
										HashTable& H,
										CosineParameterVector& V,
										Point& Q,
										list<Point*>& WitnessList,
										double radius,
										int hash_table_size
									){
	assert(hash_table_size == int_pow(2, V.get_vector_size()));
	
	return H.get_NN_dec(Q, WitnessList, cosine_dist, radius, V.phi(Q, hash_table_size));
}
*/
