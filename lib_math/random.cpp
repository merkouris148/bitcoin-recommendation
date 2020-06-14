#include <iostream>

#include <cstdlib>
#include <cassert>

#include "random.h"

using namespace std;

const bool debug = true;

//constructor destructor
RandomGenerator::RandomGenerator(){
	srand(time(NULL));
	seed = rand();
	prev_rand = 0;
		
	normal_call_parity = 0;
}

RandomGenerator::RandomGenerator(int s){
	srand(0);
	seed = rand();
	prev_rand = 0;
		
	normal_call_parity = 0;
}

RandomGenerator::~RandomGenerator(){;}


//get pseudorandom numbers
int RandomGenerator::get_a_natural(){
	int new_rand = (seed + prev_rand) % max_rand;
	prev_rand = new_rand;
	
	
	return new_rand;
}

int RandomGenerator::get_an_integer(){
	return uniform_get_integer(-max_rand, max_rand);
}

double RandomGenerator::get_a_pos_real(){
	double integer_part = get_a_natural();
	double decimal_part = ((double)get_a_natural())/((double)max_rand);
	
	return integer_part + decimal_part;
}

double RandomGenerator::get_a_real(){
	return uniform_get_real(-max_rand, max_rand);
}

int RandomGenerator::uniform_get_integer(int a, int b){
	assert(a < b);
	
	/* return a point in the [a, b] segment,
	 * as a convex combination of a, b*/
	return a + ((double) get_a_natural() / (double) max_rand ) * (b - a);
}

double RandomGenerator::uniform_get_real(double a, double b){
	assert(a < b);
    
	/* return a point in the [a, b] segment,
	 * as a convex combination of a, b*/
	return a + ((double) get_a_pos_real() / (double) max_rand ) * (b - a); 	
}

double RandomGenerator::normal_get_real(){
	double U, V, S;
	
	do{
		U = uniform_get_real(-1, 1);
		V = uniform_get_real(-1, 1);
	
		S = U*U + V*V;
		
	}while(! (0 < S && S <= 1.0) );  // !!! S should be in (0, 1] !!!
	/* !!! avoiding !!! :
	 * dividing by zero,
	 * log of negative number
	 * and sqrt of non positive numbers*/ 
	
	double X = U*sqrt(-2.0*log(S)/S);	double Y = V*sqrt(-2.0*log(S)/S);
	
	if(normal_call_parity == 0){
		normal_call_parity = (normal_call_parity + 1) % 2;
		
		return X;
	}
	else{
		normal_call_parity = (normal_call_parity + 1) % 2;
		
		return Y;
	}
}
