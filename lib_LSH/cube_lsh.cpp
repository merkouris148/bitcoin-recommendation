#include <iostream>

#include <cassert>
#include <cstdlib>

#include "../lib_math/point.h"
#include "cube_lsh.h"
#include "hash_table.h"


using namespace std;

//debug
const bool debug = false;


//constructor destructor
CubeParameterVector::CubeParameterVector(RandomGenerator& GGen, int size, int dim, int ww /* = 0 for compatability only*/) : 
	Gen(GGen){
	
	assert(size >= 1); vector_size = size;
	assert(dim >= 1);
	
	
	//generate params //fs
	V = new int[vector_size];
	for(int i = 0; i <= (vector_size-1); i++) 	V[i] = Gen.uniform_get_integer(0, 2);  //see coment in random.h
}

CubeParameterVector::~CubeParameterVector(){
	delete[] V;
}

//debug
void CubeParameterVector::print(){	
	cout << "[";
	for(int i = 0; i <= (vector_size-1); i++) cout << " " << V[i];
	cout << " ]";
}

//accesseors
int CubeParameterVector::get_vector_size(){
	return vector_size;
}

//hash functions
void CubeParameterVector::g(Point& Gin, Point& Gout){
	assert(Gin.get_dim() == Gout.get_dim() && Gout.get_dim() == vector_size);
	
	for(int i = 0; i <= (vector_size-1); i++) Gout.set_vi( (i+1), modulo((Gin.get_vi(i+1) + V[i]), 2) );
}

int CubeParameterVector::phi(Point& Gin, int hash_table_size /*= 0 for compatability purpouses only*/){
	g(Gin, Gin); //overwrite Gin
	Point B(vector_size); B.set_binary();
	
	return inner_product(B, Gin);
}
