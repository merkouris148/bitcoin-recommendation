#include <iostream>

#include <cassert>
#include <cstdlib>

#include "euclidean_lsh.h"
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

class EuclideanParameters{
private:
	Point* v;
	double t;
public:
//constructor destuctor
	EuclideanParameters(int dim, int w, RandomGenerator& Gen){		
		v = new Point(dim);  v->set_normal(Gen);
		t = Gen.uniform_get_integer(0, w);
	};
	~EuclideanParameters(){	delete v;};
//debuging
	void print(){ cout << "v: "; v->print(); cout << " t: " << t << endl;};
//accessors
	Point& get_v()  {return *v;};
	double get_t()  {return t;};
};


/*
 *******
 * LSH *
 *******
*/


//Euclidean LSH
//constructor destructor
EuclideanParameterVector::EuclideanParameterVector(RandomGenerator& GGen, int params_size, int dim, int ww) : 
	Gen(GGen){
	
	assert(params_size >= 1);	vector_size = params_size;
	assert(dim >= 1);
	assert(ww >= 1);  			w = ww;
	
	
	//initialize vector_size his, h1, h2, .., hk
	//we initialize vector_size hs, by initializing vector_size h parameters
	//<v, t, w>
	V = new EuclideanParameters* [vector_size];
	for(int  i = 0; i <= (vector_size-1); i++){
		V[i] = new EuclideanParameters(dim, w, Gen);
	}
}

EuclideanParameterVector::~EuclideanParameterVector(){
	//delete the vector_size hs
	//delete the V
	for(int i = 0; i <= (vector_size-1); i++)		delete V[i];
	delete[] V;
}

//debug
void EuclideanParameterVector::print(){	
	for(int i = 0; i <= (vector_size-1); i++) V[i]->print();
}

//accessors
int EuclideanParameterVector::get_vector_size(){
	return vector_size;
}

//hash functions
int EuclideanParameterVector::h(int i, Point& P){
	assert(0 <= i && i <= (vector_size-1));
	
	return (int) floor((inner_product(P, V[i]->get_v()) + V[i]->get_t()) / w);
}

void EuclideanParameterVector::g(Point& P, Point& G){
	assert(G.get_dim() == vector_size);
	
	for(int i = 0; i <= (vector_size-1); i++)  G.set_vi((i+1), h(i, P));
}

int EuclideanParameterVector::phi(Point& P, int hash_table_size){
	assert(hash_table_size >= 2);
	
	//Calculate the vector_size his and put them in to vector_size-point G
	Point G(vector_size); g(P, G);
	
	//initialize a random integer point
	Point R(vector_size);  R.set_int_random(Gen);
	
	return (int) modulo_inner_product(G, R, hash_table_size);
}

//operations
/*
void Euclidean_save_point_to_bucket(
										HashTable& H,
										EuclideanParameterVector& V,
										Point* P,
										int hash_table_size
									){
	assert(P != NULL);
	
	H.save_point_to_bucket(P, V.phi(*P, hash_table_size));
}

double Euclidean_get_NN_opt(
								HashTable& H,
								EuclideanParameterVector& V,
								Point& Q,
								Point& W,
								int hash_table_size
							){
								
	return H.get_NN_opt(Q, W, euclidean_dist, V.phi(Q, hash_table_size));
}

bool Euclidean_get_NN_dec(
							HashTable& H,
							EuclideanParameterVector& V,
							Point& Q,
							list<Point*>& WitnessList,
							double radius,
							int hash_table_size
						){
							
	return H.get_NN_dec(Q, WitnessList, euclidean_dist, radius, V.phi(Q, hash_table_size));
}
*/
