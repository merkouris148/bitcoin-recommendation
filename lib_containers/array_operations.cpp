#include <iostream>

#include <cassert>
#include <cstdlib>
#include <cmath>

#include "array_operations.h"
#include "../lib_math/integer.h"

using namespace std;

//integer arrays

//copy
void array_get_subarray(int A[], int A_array_size, int B[], int B_array_size, int i, int j){
	assert(A != NULL); assert(A_array_size >= 1);
	assert(B != NULL); assert(B_array_size >= 1);
	assert(i >= 0 && j >= 0);
	assert(j >= i);
	assert(B_array_size >= j - (i-1));
	
	int l = 0;
	for(int k = i; k <= j; k++){
		B[l] = A[k];
		l++;
	}
}

//double arrays
//search
int array_binary_find(double A[], int s, int t, double key){
	//cout << "key: " << key << endl;
	
	assert(A != NULL);
	assert(0 <= s && 0 <= t);
	assert(s < t);
	assert(A[s] < key && key <=  A[t]); //key in array
	
	
	
	//A[s] < key <= A[t] holds
	if(t == s+1)  return t;
	
	int median = s + floor((t - s)/2);
	//cout << "s: " << s << " t: " << t << endl;	//debug
	//cout << "median: " << median << endl;			//debug
	if(key <= A[median]) 	return array_binary_find(A, s, median, key);
	else 					return array_binary_find(A, median, t, key);
}


//numerical operations
double array_sum(double A[], int array_size){
	assert(A != NULL);
	assert(array_size >= 1);
	
	double S = 0;
	for(int i = 0; i <= (array_size-1); i++) S += A[i];
	
	return S;
}


double array_sum_of_squares(double A[], int array_size){
	assert(A != NULL);
	assert(array_size >= 1);
	
	double B[array_size];
	for(int i = 0; i <= (array_size-1); i++) B[i] = pow(A[i], 2);
	
	return array_sum(B, array_size);
}


//boolean arrays
bool bool_array_and(bool A[], int array_size){
	assert(A != NULL);
	assert(array_size >= 1);
	
	bool b = A[0];
	for(int i = 0; i <= (array_size-1); i++){
		b = b and A[i];
		if(!b) return false;
	}
	
	return true;
}

bool bool_array_or(bool A[], int array_size){
	assert(A != NULL);
	assert(array_size >= 1);
	
	bool b = A[0];
	for(int i = 0; i <= array_size; i++){
		b = b or A[i];
		if(b) return true;
	}
	
	return false;
}


//find
void double_array_find_k_max(
								double A[], int A_array_size,
								int k,
								double M[], int M_array_size,
								int Pos[], int Pos_array_size
							){
	assert(A != NULL && M != NULL && Pos != NULL);
	assert(k >= 1);
	assert(M_array_size == k && Pos_array_size == k);
	assert(A_array_size >= k);
	
	double B[A_array_size];
	array_copy(A, A_array_size, B, A_array_size);
	
	for(int i = 1; i <= k; ++i){
		
		array_find_max(B, A_array_size, M[i-1], Pos[i-1]);
		
		B[Pos[i-1]] = -HUGE_VAL;
	}
}

void double_array_find_k_min(
								double A[], int A_array_size,
								int k,
								double M[], int M_array_size,
								int Pos[], int Pos_array_size
							){
	assert(A != NULL && M != NULL && Pos != NULL);
	assert(k >= 1);
	assert(M_array_size == k && Pos_array_size == k);
	assert(A_array_size >= k);
	
	double B[A_array_size];
	array_copy(A, A_array_size, B, A_array_size);
	
	array_multiply(B, A_array_size, -1);
	double_array_find_k_max(B, A_array_size, k, M, M_array_size, Pos, Pos_array_size);							
	array_multiply(M, M_array_size, -1);
}
