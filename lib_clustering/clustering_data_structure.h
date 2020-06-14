#ifndef CLUSTERING_DATA_STRUCTURE_H

#define CLUSTERING_DATA_STRUCTURE_H

#include <map>
#include <utility>
#include <list>
#include <fstream>

#include "../lib_math/point.h"
#include "../lib_math/random.h"

//typedefs
typedef std::pair<
					int, 							/*cluster number */
					std::list<Point*>::iterator /* pointer to a list position*/
				> position;


typedef std::map< Point*, position /*, ptr_lexicograpgical_less*/ > point_pos_map;
typedef std::pair< Point*, position > point_pos_pair;

typedef std::list<Point*> point_list;

typedef point_pos_map::iterator map_iterator;
typedef point_list::iterator list_iterator;

//map_iterator functions
//accessors
int get_cluster(map_iterator it);
list_iterator get_list_it(map_iterator it);
Point* get_point(map_iterator it);

/*
 * Complexity:
 * 	Space 				= 2n = O(n)
 *  Time (Construction) = O(nlogn)
 * 
 * (see alos the coments above the methods declaration)
 */
class ClusteringDataStructure{
private:
	/* The map Cluster_to_Point, given a Point P in
	 * the map return in O(logn) the posistion of P in
	 * Cluster_to_Points Data Structure (DS)*/
	point_pos_map Point_to_Cluster;
	
	point_list** Cluster_to_Points;
	
	Point** Cluster_to_Centroid;
	
	//constants
	int k;		//number of clusters
	int dim;	//dimention

	//metrics
	double (*metric) (Point& X, Point& Y);

public:
//constuctor - destructor
	//Constuctor = O(nlogn)
	ClusteringDataStructure(
								point_list::iterator s,
								point_list::iterator t,
								int kk,
								int ddim,
								double (*m) (Point& X, Point& Y)
							);
	~ClusteringDataStructure();
//debug
//cout
	void print_clustering();
	void print_centroids();
	void print_stats();
	void print_all();

//output to file
	void print_clustering(std::ofstream& out);
	void print_centroids(std::ofstream& out);
	void print_stats(std::ofstream& out);
	void print_all(std::ofstream& out);

//accessors
	point_pos_map::iterator begin();
	point_pos_map::iterator end();
	
	std::list<Point*>::iterator cluster_begin(int c);
	std::list<Point*>::iterator cluster_end(int c);

	int get_num_of_points();
	int get_cluster_size(int c);
	int get_num_of_points_assigned();
	int get_k();
	int get_dim();
	
	int get_cluster(Point* P);
	
	bool member(Point* P);

	double metric_func(Point& X, Point& Y);
	
//mutators
	bool set_cluster(point_pos_map::iterator it, int c);	// = Θ(1)
	bool set_centroid(int c, Point& new_cent);				// = Θ(1)
	bool set_centroid(int c, int r);						// = θ(1)
	void set_centroid_rand(int c, RandomGenerator& Gen);	// = Θ(1)

//metrics
	/* 
	 * Given a point P returns
	 * d = min{metric(P, c) | c is one of the first t centroids, t >= 1}
	 */
	double distance_from_centroids(Point& P, int t);					// = O(t)
	
	/* The same as above where t = k
	 * min{metric(P, C) | C centroid}*/
	double distance_from_centroids(Point& P);							// = O(k)
	double arg_distance_from_centroids(Point& P, int& c);

	/* 
	 * Computes the distance of the Centroids (defined above) for every
	 * point in the DS:
	 * 
	 * Returns in argument array D an araay s.t.
	 * D[i] = min{metric(P, c) | P is the i-th point in the DS,
	 * 							 c is one of the first t centroids, t >= 1}
	 */
	void distance_from_centroids(double D[], int array_size, int t);	// = O(n*t)
	
	/* The same as above where t = k
	 * D[i] = min{metric(P, C) | P is the i-th point in the DS,
	 * 							 C a centroid}
	 */
	void distance_from_centroids(double D[], int array_size);			// = O(n*k)
	
	void distance_from_centroids(Point& P, double Ds[], int array_size);
	
	/* 
	 * computes the mean point of a cluster M,
	 * M = (1/n)*sum{P | P in cluster c}
	 */
	void get_mean(int c, Point& M);										// = O(a), a = # Points in Cluster c
	
	/*
	 * Computes the medoid of a cluster M,
	 * M = arg min{ sum{metric(t, i) | i in cluster c, t != i} | t in cluster c }
	 */
	void get_medoid(int c, Point& M);									// = O(a^2), a = # Points in Cluster c
	
	double average_dist_from_cluster(Point& P, int c);
	
//objective function
	/*
	 * computes
	 * sum{ (min{metric(P, C) | C centroid})^2 | P in DS}
	 */
	double objective_function();										// = O(n)
	
//silhouette
	double cluster_silhouette(int c);
	double total_silhouette();
	void get_silhouette_vector(double V[], int array_size);
};

#endif
