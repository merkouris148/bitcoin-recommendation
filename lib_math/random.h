#ifndef RANDOM_H

#define RANDOM_H

#include <cmath>
#include <cstdlib>
#include <ctime>

//constants
const int max_rand = 1000;
//const double my_max_rand_double = 1000.0;

class RandomGenerator{
private:
	int seed;
	int prev_rand;
	int normal_call_parity;
public:
//constructor destructor
	RandomGenerator();
	RandomGenerator(int s);
	~RandomGenerator();
//get pseudorandom numbers
	int get_a_natural();
	int get_an_integer();
	double get_a_pos_real();
	double get_a_real();
//distributions
	int uniform_get_integer(int a, int b);			//returns an integer in [a, b) intersection Z //(*)
	double uniform_get_real(double a, double b);	//returns a double in [a, b)
	double normal_get_real();
};

/* (*) Observation
 * To get an integer in {a, a+1, a+2, ..., b-1, b}
 * ([a, b] intersection Z) call uniform_get_integer(a, b+1).
*/

#endif
