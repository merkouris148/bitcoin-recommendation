#ifndef ARRAY_OPERATIONS_H

#define ARRAY_OPERATIONS_H

#include <list>
#include <fstream>

/*
 **********************
 *  Array Operations  *
 **********************
*/

/*
 **********************
 * Template Operations*
 **********************
*/

//copy array A to B
/* The operator = must've benn defined */
template <typename T>
void array_copy(T From[], int From_size, T To[], int To_size);
/* returns in argument array B a subarray of A from i to j
 * in the first j - (i-1) positions of B the subarray A(i, j)*/
void array_get_subarray(int A[], int A_array_size, int B[], int B_array_size, int i, int j);

//search
//unsorted
/* The operator < must have benn defined for t in T, for both array_find_max() and
 * array_find_min() */

template <typename T>
void array_find_max(T A[], int array_size, T& max, int& pos);
template <typename T>
void array_find_max(T A[], int array_size, T& max);

/* The operation t * s must have benn defined where t in T, s in S */
template <typename T>
void array_find_min(T A[], int array_size, T& min, int& pos);
template <typename T>
void array_find_min(T A[], int array_size, T& min);


//numerical operations
/* The operation t * s must have benn defined where t in T, s in S */
template <typename T, typename S>
void array_multiply(T A[], int array_size, S mul);


//list arrays
/* Given an array of lists Ls = [L1, L2, L3, ..., Ln]
 * returns in the argument K the list
 * K = L1 * L2 * L3 * ... * Ln, 
 * where the operation * is the concatenation of two lists */
template <typename T>
void list_array_concat(std::list<T>**& Ls, std::list<T>& K);

//debug
template <typename T>
void array_print(T A[], int array_size);

template <typename T>
void array_print(std::ofstream& out, T A[], int array_size);

#include "array_operations.tpp"

/*
 *******************
 * Typed Operations*
 *******************
*/

//double arrays

//search
int array_binary_find(double A[], int s, int t, double key);

//numerical operations
double array_sum(double A[], int array_size);
double array_sum_of_squares(double A[], int array_size);

//boolean arrays
bool bool_array_and(bool A[], int array_size);
bool bool_array_or(bool A[], int array_size);

//find
void double_array_find_k_max(
								double A[], int A_array_size,
								int k,
								double M[], int M_array_size,
								int Pos[], int Pos_array_size
							);


void double_array_find_k_min(
								double A[], int A_array_size,
								int k,
								double M[], int M_array_size,
								int Pos[], int Pos_array_size
							);


#endif
