#ifndef INTEGER_H

#define INTEGER_H

//relations
/* if a >= b 	return a
 * else 		return b*/
int max(int a, int b);


/* if a >= b 	return b
 * else 		return a*/
int min(int a, int b);

//operations
int modulo(int r, int m);
int int_pow(int base, int exp);

//metrics
int hamming_dist(int n1, int n2);

#endif
