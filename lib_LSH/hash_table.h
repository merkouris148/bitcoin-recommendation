#ifndef HASH_TABLE_H

#define HASH_TABLE_H

#include <list>
#include <set>

#include "point_list.h"

class HashTable{
private:
	PointList** Array;
	int TableSize;
public:
//constructor destructor
	HashTable(int TS, int dim);
	~HashTable();
//debug

	/* Prints the points of each bucket in th form:
	 * 		[h] P1 P2 P3 ...
	 * where h in {1, .., TableSize} and Pi Points*/
	void print(char* separator = "\n");

//accessor

	/* Returns the size of the HashTable, the number
	 * of the buckets*/
	int get_table_size();

//opetarions

	/* Saves a point P in the h-th bucket */
	void save_point_to_bucket(Point* P, int h);

	//std::set<Point*> get_bucket(int h);

	/* Applies the Exact NN algorithm in h-th bucket*/
	double get_NN_opt(
					Point& Q,									//Query Point
					Point& W,									//Witness
					double (*metric) (Point& P1, Point& P2),	//metric function
					int h										//num of bucket
				);

	/* Applies the Exact NN algorithm in the buckets
	 * pointed by the index array H.
	 * 
	 * Returns the minimum distance and the point W that
	 * achives this distance with query point Q.
	 * 
	 * Namely: let d = get_NN_opt(Q, W, metric, H, array_size)
	 * d = min {d_h | d_h = get_NN_opt(Q, W_h, metric, h), where h in H}
	 * 
	 * Also W is a point s.t. metric(Q, W) = d.*/
	double get_NN_opt(
					Point& Q,									//Query Point
					Point& W,									//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					int H[],									//array of bucket indexes
					int array_size
				);
	
	/* Applies the Exact R-N algorithm in h-th bucket*/
	bool get_NN_dec(
					Point& Q,									//Query Point
					std::list<Point*>& WitnessList,				//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					double radius, 								//radius
					int h										//num_of_bucket
				);
	
	/* Applies the Exact R-N algorithm in the buckets
	 * pointed by the index array H.
	 * 
	 * Returns true if there is at least one Point P s.t.
	 * metric(Q, p) <= R, R > 0, else return false.
	 * 
	 * Also return in the argument WitnessList a list of the
	 * Points tha satisfy the above criterion*/
	bool get_NN_dec(
					Point& Q,									//Query Point
					std::list<Point*>& WitnessList,				//Witness
					double (*metric) (Point& P1, Point& P2),	//metric_func
					double radius, 								//radius
					int H[],									//num_of_bucket
					int array_size
				);
	
	void get_k_NN(
					Point& Q,							//Query Point
					std::set<Point*>& Witnesses,		//Witnesses
					double (*metric)(Point&, Point&),	//metric
					int k,								//num of neighbors
					int h								//bucket
				);
};


#endif
