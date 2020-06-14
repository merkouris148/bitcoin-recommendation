#include <cassert>

#include "integer.h"

//relations
int max(int a, int b){
	if(a >= b)  return a;
	else 		return b;
}

int min(int a, int b){
	if(a >= b)  return b;
	else 		return a;
}

//operations
int modulo(int r, int m){
	assert(m > 1);
	
	while(r < 0){
		r += m;
	}
	while(r > (m-1)){
		r -= m;
	}
	return r;
}

int int_pow(int base, int exp){
	assert(exp >= 0);
	
	int bas_exp = 1;		//= base^0
	for(int i = 1; i <= exp; i++) bas_exp *= base;
	
	return bas_exp;
}

//metrics
int hamming_dist(int n1, int n2){
 
    int x = n1 ^ n2; 
    int setBits = 0; 
  
    while (x > 0) { 
        setBits += x & 1; 
        x >>= 1; 
    } 
  
    return setBits; 
} 
