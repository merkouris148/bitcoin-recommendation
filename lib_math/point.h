#ifndef POINT_H

#define POINT_H

#include <fstream>
#include <list>
#include <set>
#include <string>

#include <cstdlib>
#include <climits>

#include "random.h"

class Point{
private:
	char* id;
	double* v;
	int dim;
public:
//functions
//constructor - destructor
	Point(int d, const char* iid = "");
	Point(std::string line, char delimeter);
	//Point(const Point& R);
	~Point();
//show
	void print(std::ofstream& out, char* separator = " ");
	void print(char* separator = " ");
	void print_id(std::ofstream& out);
//accessors
	double get_vi(int i);
	int get_dim();
	char* get_id();
	void get_array(double array[], int array_size);
//mutators
	void set_vi(int i, double x);
	void set_all_vis(double d);
	void set_from_array(double* Ds, int array_size);
	void set_point(Point& R);
	void set_id(char* iid);
//random points
	void set_random(RandomGenerator& Gen);
	void set_int_random(RandomGenerator& Gen);
	void set_normal(RandomGenerator& Gen);
//special points
	void set_binary();
//operations
	void scalar_product(double k);
	void scalar_divide(double k);
//metrics
	double norm();
};

//Operations
//Point operations
//Linear
void point_add(Point &X, Point& Y, Point &Sum);
void point_sub(Point& X, Point& Y, Point& Dif);
double inner_product(Point& P1, Point& P2);
int modulo_inner_product(Point& P1, Point& P2, int m /*modulo*/ = INT_MAX);

//Metrics
double euclidean_dist(Point& P1, Point& P2);
double point_cos(Point& P1, Point& P2);
double cosine_dist(Point& P1, Point& P2);

//Relations
bool equal(Point& X, Point& Y);
bool ptr_equal(Point* X, Point* Y);
/* Given two different sequences of the same length, a1a2...ak and b1b2...bk,
 * the first one is smaller than the second one for the lexicographical order,
 * if ai<bi (for the order of A), for the first i where ai and bi differ.
 * [Wikipedia]*/
bool lexicograpgical_less(Point& X, Point& Y);
bool ptr_lexicograpgical_less(Point* X, Point* Y);

/*
 *********
 * Other *
 *********
 */


//list<Point*> operations
//debug
void print(std::list<Point*>& Ps, const char* separator = "\n");
void print(std::set<Point*>& Ps, const char* separator = "\n");

//Point* Arrays
//predicates
bool Point_array_member(Point* Ps[], int array_size, Point& P);

//set<Point*> operations
//std::set<Point*> get_n_nearest_neighbors(Point& P, std::set<Point*>& Ps);

#endif
