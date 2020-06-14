#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

#include <list>
#include <queue>

#include "point.h"
#include "integer.h"
#include "../lib_containers/array_operations.h"

const bool warnings = true;
const bool on_warnings_continue = true;

using namespace std;

//functions defintions
//Constructor/Distactor
Point::Point(int d, const char* iid /*= ""*//*passing as value*/){
	assert(d >= 1);
	
	dim = d;
	v = new double[d];
	id = new char[strlen(iid)+1]; strcpy(id,iid);
	
	//each new point is initialized to (0, 0, .., 0)
	for(int i = 0; i <= (d-1); i++) v[i] = 0;
}

Point::Point(string line, char delimeter){
	assert(!line.empty());
	
	stringstream sline(line);
	string token;
	
	//get id{
		getline(sline, token, delimeter);
		id = new char [token.length()+1]; strcpy(id, token.c_str());
	//}
	
	//get coordinates{
		queue<double> q;
		while(getline(sline, token, delimeter)) q.push(stod(token));
		
		dim = q.size();
		v = new double[dim];
		for(int i = 0; i <= dim-1; i++){
			v[i] = q.front(); q.pop();
		} 
	//}
}

Point::~Point(){
	delete[] id;
	delete[] v;
}


//Show
void Point::print(ofstream& out, char* separator){
	//P{
		assert(separator != NULL);
	//}
	
	out << id << " ";
	for(int i = 1; i <= get_dim(); i++){
		out << get_vi(i) << separator;
	}
}

void Point::print(char* separator){
	//P{
		assert(separator != NULL);
	//}
	
	cout << id << " ";
	for(int i = 1; i <= get_dim(); i++){
		cout << get_vi(i) << separator;
	}
}

void Point::print_id(std::ofstream& out){
	out << id;
}

//Accessors
double Point::get_vi(int i){
	//P{
		assert(1 <= i && i <= get_dim());
	//}
	
	return v[i-1];
}

int Point::get_dim(){
	return dim;
}

char* Point::get_id(){
	return id;
}

void Point::get_array(double array[], int array_size){
	assert(array_size >= dim);
	
	for(int i = 1; i <= dim; ++i) array[i-1] = get_vi(i);
}

//Mutators
void Point::set_vi(int i, double x){
	//P{
		assert(1 <= i && i <= get_dim());
	//}
	
	v[i-1] = x;
}

void Point::set_all_vis(double d){
	for(int i = 1; i <= get_dim(); i++) set_vi(i, d);
}

void Point::set_from_array(double* Ds, int array_size){
	assert(get_dim() == array_size);
	
	for(int i = 1; i <= get_dim(); i++) set_vi(i, Ds[i-1]);
}

void Point::set_point(Point& R){
	//P{
		assert(get_dim() == R.get_dim());
	//}
	
	for(int i = 1; i <= get_dim(); i++)		set_vi(i, R.get_vi(i));
}


//Random Points
void Point::set_random(RandomGenerator& Gen){
	for(int i = 1; i <= get_dim(); i++) set_vi(i, Gen.get_a_real());
}

void Point::set_int_random(RandomGenerator& Gen){
	for(int i = 1; i <= get_dim(); i++)  set_vi(i, Gen.get_an_integer());
}

void Point::set_normal(RandomGenerator& Gen){	
	for(int i = 1; i <= get_dim(); i++)  set_vi(i, Gen.normal_get_real());
}

//special points
void Point::set_binary(){
	for(int i = 1; i <= get_dim(); i++) set_vi(i, int_pow(2, (i-1)));
}

//Operations
//double operations
double double_add(double x, double y){return x + y;}

double double_sub(double x, double y){return x - y;}

double double_mul(double x, double y){return x * y;}

double double_div(double x, double y){assert(y != 0); return x / y;}

//integer operations
int modulo_mul(int x, int y, int m){return modulo(x*y, m);}  //a bit different operation



//Point operations
//double point_op
void point_op(Point& X, Point& Y, Point& Z, double (*op)(double x, double y)){
	//P{
		assert(X.get_dim() == Y.get_dim() && Y.get_dim() == Z.get_dim());
	//}
	
	int d = X.get_dim();
	for(int i = 1; i <= d; i++)	Z.set_vi(i, op(X.get_vi(i), Y.get_vi(i)));
}

void point_mul(Point& X, Point& Y, Point& Prod){
	point_op(X, Y, Prod, double_mul);
}

void point_div(Point& X, Point& Y, Point& Quo){
	point_op(X, Y, Quo, double_div);
}

double point_sum(Point& X){
	int S = 0;
	for(int i = 1; i <= X.get_dim(); i++)	S += X.get_vi(i);
	
	return S;
}

//Vector Operations (well defined math operations)
void point_add(Point& X, Point& Y, Point& Sum){
	return point_op(X, Y, Sum, double_add);
}

void point_sub(Point& X, Point& Y, Point& Dif){
	return point_op(X, Y, Dif, double_sub);
}

void Point::scalar_product(double k){
	
	for(int i = 1; i <= get_dim(); i++) set_vi(i, get_vi(i)*k);
}

void Point::scalar_divide(double k){
	scalar_product(1/k);
}

double inner_product(Point& P1, Point& P2){
	//P{
		assert(P1.get_dim() == P2.get_dim());
	//}
	
	Point Temp(P1.get_dim());
	point_mul(P1, P2, Temp);
	return point_sum(Temp);
}

int modulo_inner_product(Point& P1, Point& P2, int m){
	//P{
		assert(P1.get_dim() == P2.get_dim());
		assert(m > 1);
	//}
	
	Point Temp(P1.get_dim());
	for(int i = 1; i <= P1.get_dim(); i++) Temp.set_vi(i, modulo_mul(P1.get_vi(i), P2.get_vi(i), m));
	
	return modulo(point_sum(Temp), m);
}

//Metrics
double Point::norm(){
	return sqrt(inner_product(*this, *this));
}

double euclidean_dist(Point& P1, Point& P2){
	//P{
		assert(P1.get_dim() == P2.get_dim());
	//}
	
	int d = P1.get_dim();
	
	Point Temp(d, "");
	point_sub(P1, P2, Temp);
	double n = Temp.norm();
	
	if(n == 0 && warnings){
		if(!equal(P1, P2)){
			cout << "Warning: P1 != P2, but d(P1, P2) computed to 0" << endl;
			assert(on_warnings_continue);
		}
	}
	
	return n;
}

double point_cos(Point& P1, Point& P2){
	return inner_product(P1, P2)/(P1.norm()*P2.norm());
}

double cosine_dist(Point& P1, Point& P2){
	
	double n = 1 - point_cos(P1, P2);
	
	if(n == 0 && warnings){
		if(!equal(P1, P2)){
			cout << "Warning: P1 != P2, but d(P1, P2) computed to 0" << endl;
			assert(on_warnings_continue);
		}
	}
	
	return n;
}


//Relations
bool equal(Point& X, Point& Y){
	assert(X.get_dim() == Y.get_dim());
	
	int dim = X.get_dim();
	
	bool B[dim];
	for(int i=1; i <= dim; i++) B[i-1] = X.get_vi(i) == Y.get_vi(i);
	
	return bool_array_and(B, dim);
}

bool ptr_equal(Point* X, Point* Y){
	assert(X != NULL);
	assert(Y != NULL);
	
	return equal(*X, *Y);
}

/* Given two different sequences of the same length, a1a2...ak and b1b2...bk,
 * the first one is smaller than the second one for the lexicographical order,
 * if ai<bi (for the order of A), for the first i where ai and bi differ.
 * [Wikipedia]*/
bool lexicograpgical_less(Point& X, Point& Y){
	assert(X.get_dim() == Y.get_dim());
	
	int dim = X.get_dim();
	
	//find first i s.t. X[i] != Y[i]
	int i = 1; while(i <= (dim-1) && X.get_vi(i) == Y.get_vi(i)) i++;
	
	return X.get_vi(i) < Y.get_vi(i);
}

bool ptr_lexicograpgical_less(Point* X, Point* Y){
	assert(X != NULL);
	assert(Y != NULL);
	
	return lexicograpgical_less(*X, *Y);
}


/*
 *********
 * Other *
 *********
 */



//list<Point*> operations

//debug
void print(std::list<Point*>& Ps, const char* separator /*= "\n"*/){
	cout << endl << "[" << separator;
	for(auto l : Ps){
		l->print();
		cout << separator;
	}
	cout << "]" << endl;
}

void print(std::set<Point*>& Ps, const char* separator /*= "\n"*/){
	cout << endl << "[" << separator;
	for(auto l : Ps){
		l->print();
		cout << separator;
	}
	cout << "]" << endl;
}

//Point* Arrays
//predicates
bool Point_array_member(Point* Ps[], int array_size, Point& P){
	assert(Ps != NULL);
	assert(array_size >= 1);
	
	for(int i = 0; i <= (array_size-1); i++){
		if(equal(P, *Ps[i])) return true;
	}
	
	return false;
}

