#include <cassert>
#include <cstdlib>
#include <cmath>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "point_list.h"

using namespace std;

//constants
const int buffer_size = 10000;

PointList::PointList(int d, bool mem_bit){
	//mem bit{
		erase_items = mem_bit;
	//}
	
	assert(d >= 1);
	dim = d;
}

//
PointList::~PointList(){
	while(!L.empty()){
		if(erase_items) delete L.front(); 
		L.pop_front();
	}
}

//input - output
void PointList::print(char* separator){
	if(!L.empty()){
		for(auto it = L.begin(); it != L.end(); it++){
			(*it)->print(" ");
			cout << separator;
		}
	}
}

//accessors
int PointList::get_size(){return L.size();}

list<Point*>::iterator PointList::begin(){return L.begin();}

list<Point*>::iterator PointList::end(){return L.end();}


//operations
double PointList::exact_NN_opt(Point& Q, Point& W, double (*metric) (Point& P1, Point& P2)){
	if(L.empty()) return HUGE_VAL;
	
	W.set_point(*(L.front()));
	double d = metric(Q, W);
	for(auto it = L.begin(); it != L.end(); it++){
		if(metric(Q, **it) < d){
			W.set_point(**it);
			d = metric(Q, **it);
		}
	}
	
	return d;
}

bool PointList::exact_NN_dec(Point& Q, list<Point*>& WitnessList, double (*metric) (Point& P1, Point& P2), double radius){
	if(L.empty()) return false;

	assert(WitnessList.empty());
	for(auto it = L.begin(); it != L.end(); it++){
		if(metric(Q, **it) < radius) WitnessList.emplace_back(*it);
	}
	
	return WitnessList.empty();
}

//expand
void PointList::emplace_back(Point* P){
	assert(P != NULL);
	assert(P->get_dim() == dim);
	
	L.emplace_back(P);
}





