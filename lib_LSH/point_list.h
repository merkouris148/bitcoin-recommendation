#ifndef POINT_LIST_H

#define POINT_LIST_H

#include "../lib_math/point.h"
#include <list>


class PointList{
private:
	std::list<Point*> L;
	int dim;			//dim of all the Points
//memory bit
	bool erase_items;
public:
//cunstructor - destructor
	PointList(int d,bool mem_bit = false);
	~PointList();
//debug
	void print(char* separator = "\n");
//accessors
	std::list<Point*>::iterator begin();
	std::list<Point*>::iterator end();
	int get_size();
//operations
	double exact_NN_opt(Point& Q, Point& W, double (*metric) (Point& P1, Point& P2));
	bool exact_NN_dec(Point& Q, std::list<Point*>& WitnessList, double (*metric) (Point& P1, Point& P2), double radius);
//expand
	void emplace_back(Point* P);
};


#endif
