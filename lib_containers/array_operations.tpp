#include "list_operations.h"

#include <iostream>

#include <cassert>

//copy
template <typename T>
void array_copy(T A[], int A_array_size, T B[], int B_array_size){
	assert(A != NULL); assert(A_array_size >= 1);
	assert(B != NULL); assert(B_array_size >= 1);
	assert(A_array_size <= B_array_size);
	
	for(int i = 0; i <= (A_array_size-1); i++) B[i] = A[i];	
}

//search
//unsorted
template <typename T>
void array_find_max(T A[], int array_size, T& max, int& pos){
	assert(A != NULL);
	assert(array_size >= 1);
	
	max = A[0];
	pos = 0;
	for(int i = 0; i <= (array_size-1); i++){
		if(A[i] > max){
			max = A[i];
			pos = i;
		}
	}
}

template <typename T>
void array_find_max(T A[], int array_size, T& max){
	assert(A != NULL);
	assert(array_size >= 1);
	
	max = A[0];
	for(int i = 0; i <= (array_size-1); i++){
		if(A[i] > max)	max = A[i];
	}
}

template <typename T>
void array_find_min(T A[], int array_size, T& min, int& pos){
	assert(A != NULL);
	assert(array_size >= 1);
	
	T B[array_size]; 
	array_copy(A, array_size, B, array_size);
	array_multiply(B, array_size, -1.0);
	
	array_find_max(B, array_size, min, pos);
	
	min *= (-1.0);
}

template <typename T>
void array_find_min(T A[], int array_size, T& min){
	assert(A != NULL);
	assert(array_size >= 1);
	
	T B[array_size]; 
	array_copy(A, array_size, B, array_size);
	array_multiply(B, array_size, -1);
	
	array_find_max(B, array_size, min);
	
	min *= (-1);
}

//numerical operations
template <typename T, typename S>
void array_multiply(T A[], int array_size, S mul){
	assert(A != NULL);
	assert(array_size >= 1);
	
	for(int i = 0; i <= (array_size-1); i++) A[i] *= mul;
}


//list arrays
/* Given an array of lists Ls = [L1, L2, L3, ..., Ln]
 * returns in the argument K the list
 * K = L1 * L2 * L3 * ... * Ln, 
 * where the operation * is the concatenation of two lists */
template <typename T>
void list_array_concat(std::list<T>* Ls[], int array_size, std::list<T>& K){
	assert(Ls != NULL);
	for(int i = 0; i <= (array_size-1); i++) assert(Ls[i] != NULL);
	
	for(int i = 0; i <= (array_size-1); i++)  append(K, *Ls[i]);
}


//debug
template <typename T>
void array_print(T A[], int array_size){
	assert(A != NULL);
	assert(array_size >= 1);
	
	std::cout << "[ ";
	for(int i= 0; i <= (array_size-1); i++) std::cout << A[i] << " ";
	std::cout << "]";
}

template <typename T>
void array_print(std::ofstream& out, T A[], int array_size){
	assert(A != NULL);
	assert(array_size >= 1);
	
	out << "[ ";
	for(int i= 0; i <= (array_size-1); i++) out << A[i] << " ";
	out << "]";
}













