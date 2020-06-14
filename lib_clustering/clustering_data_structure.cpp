#include "clustering_data_structure.h"

#include <iostream>
#include <algorithm>

#include <cassert>
#include <cmath>
#include <cstdlib>

#include "../lib_containers/list_operations.h"
#include "../lib_containers/array_operations.h"


const bool debug = true;

using namespace std;



//map_iterator functions
//accessors
int get_cluster(map_iterator it){ return it->second.first; }
list_iterator get_list_it(map_iterator it){ return it->second.second; }
Point* get_point(map_iterator it){ return it->first; }

//mutators
void map_it_set_cluster(map_iterator it, int c){ it->second.first = c; }
void map_it_set_list_it(map_iterator it, list_iterator lit){ it->second.second = lit; }


//list
//accessors
list_iterator last(point_list& Ls){ return --Ls.end(); }


//constructor - destructor
ClusteringDataStructure::ClusteringDataStructure(
													std::list<Point*>::iterator s,
													std::list<Point*>::iterator t,
													int kk,
													int ddim,
													double (*m) (Point& X, Point& Y)
												){
													
	assert(kk >= 1); 	k = kk;
	assert(ddim >= 1); 	dim = ddim;
	
	//metrics
	metric = m;
	
	//construct data structures
	/* We have k+1 clusters where the cluster 0 is the
	 * Cluster of unassigned points.
	 * 
	 * At the beginig each Point will be assigned to
	 * cluster 0*/
	Cluster_to_Points = new list<Point*>* [k+1];
	for(int i = 0; i <= k; i++) Cluster_to_Points[i] = new list<Point*>;
	
	/* At the beginnig each class has an arbitary centroid
	 * the (0, 0, ..., 0) in R^d.
	 * 
	 * The cluster 0 will always has an arbitary centroid
	 * (0, 0,.., 0) since cluster 0 is not an actual cluster*/
	Cluster_to_Centroid = new Point* [k+1];
	for(int i = 0; i <= k; i++) Cluster_to_Centroid[i] = new Point(dim);
	
	//initialize Data Structures
	for(auto p = s; p != t; p++){
		Cluster_to_Points[0]->emplace_back(*p);
		Point_to_Cluster.insert( make_pair( *p, make_pair(0, last(*Cluster_to_Points[0])) ) );
	}
}


ClusteringDataStructure::~ClusteringDataStructure(){
	//clear Point_to_Cluster
	Point_to_Cluster.clear();
	
	//delete Cluster_to_Points
	for(int i = 0; i <= k; i++){
		Cluster_to_Points[i]->clear();
		delete Cluster_to_Points[i];
	}
	delete[] Cluster_to_Points;
	
	//delete Cluster_to_Centroid
	for(int i = 0; i <= k; i++) delete Cluster_to_Centroid[i];
	delete[] Cluster_to_Centroid;
}

//debug

//cout
void ClusteringDataStructure::print_centroids(){
	//Print Centroids
	cout << "Centroids: " << endl;
	for(int i = 1; i <= k; i++){
		cout << "[" << i << "] ";
		Cluster_to_Centroid[i]->print();
		cout << endl;
	}
	cout << endl;
	
	//Print Clustering
	
}

void ClusteringDataStructure::print_clustering(){
	for(int i = 0; i <= k; i++){
		cout << "Cluster #" << i << endl;
		for(auto l = Cluster_to_Points[i]->begin(); l != Cluster_to_Points[i]->end(); l++){
			cout << "\t";
			(*l)->print();
			cout << endl;
		}
	}
	cout << endl;
}

void ClusteringDataStructure::print_stats(){
	cout << "# points: " << get_num_of_points(); cout << endl;
	cout << "# points assinged: " << get_num_of_points_assigned(); cout << endl << endl;
	
	for(int i = 0; i <= k; i++){
		cout << "# points assigned to Cluster " << i << ": " << get_cluster_size(i);
		cout << endl;
	}
	cout << endl;
}

void ClusteringDataStructure::print_all(){
	cout << "print centroids..." << endl << endl;
	print_centroids();
	cout << "printing clustering..." << endl << endl;
	print_clustering();
	cout << "printing statistics..." << endl << endl;
	print_stats();
}

//fstream

void ClusteringDataStructure::print_centroids(ofstream& out){
	assert(out.is_open());
	
	//Print Centroids
	out << "Centroids: " << endl;
	for(int i = 1; i <= k; i++){
		out << "[" << i << "] ";
		Cluster_to_Centroid[i]->print(out);
		out << endl;
	}
	out << endl;
	
	//Print Clustering
	
}

void ClusteringDataStructure::print_clustering(ofstream& out){
	assert(out.is_open());
	
	for(int i = 0; i <= k; i++){
		out << "Cluster #" << i << endl;
		for(auto l = Cluster_to_Points[i]->begin(); l != Cluster_to_Points[i]->end(); l++){
			out << "\t";
			(*l)->print_id(out);
			out << " ";
		}
		out << endl;
	}
	out << endl;
}

void ClusteringDataStructure::print_stats(ofstream& out){
	assert(out.is_open());
	
	out << "# points: " << get_num_of_points(); out << endl;
	out << "# points assinged: " << get_num_of_points_assigned(); out << endl << endl;
	
	for(int i = 0; i <= k; i++){
		out << "# points assigned to Cluster " << i << ": " << get_cluster_size(i);
		out << endl;
	}
	out << endl;
}

void ClusteringDataStructure::print_all(ofstream& out){
	assert(out.is_open());
	
	out << "print centroids..." << endl << endl;
	print_centroids(out);
	out << "printing clustering..." << endl << endl;
	print_clustering(out);
	out << "printing statistics..." << endl << endl;
	print_stats(out);
}


//mutators
bool ClusteringDataStructure::set_cluster(map_iterator m, int c){
	assert(1 <= c && c <= k);
	
	
	//cluster remains unchanged
	if(::get_cluster(m) == c)  return false;
	
	
	//erase point from cluster
	Cluster_to_Points[::get_cluster(m)]->erase(get_list_it(m));
	
	//put point to new cluster
	Cluster_to_Points[c]->emplace_back(get_point(m));
		
	//update map
	map_it_set_cluster(m, c);							//set cluster
	map_it_set_list_it(m, last(*Cluster_to_Points[c]));	//set position
		
	
	//cluster chaned
	return true;
}


bool ClusteringDataStructure::set_centroid(int c, Point& new_cent){
	assert(1 <= c && c <= k);
	
	if(equal(new_cent, *Cluster_to_Centroid[c]))  	return false;  //return unchanged
	
	Cluster_to_Centroid[c]->set_point(new_cent);	return true;   //return changed
}

bool ClusteringDataStructure::set_centroid(int c, int r){
	return set_centroid(c, *get_point(next(begin(), r)));
}

void ClusteringDataStructure::set_centroid_rand(int c, RandomGenerator& Gen){
	assert(1 <= c && c <= k);
	
	Cluster_to_Centroid[c]->set_random(Gen);
}


//accessors
map<Point*, position>::iterator ClusteringDataStructure::begin(){
	return Point_to_Cluster.begin();
}

map<Point*, position>::iterator ClusteringDataStructure::end(){
	return Point_to_Cluster.end();
}

list<Point*>::iterator ClusteringDataStructure::cluster_begin(int c){
	assert(1 <= c && c <= k);
	
	return Cluster_to_Points[c]->begin();
}

list<Point*>::iterator ClusteringDataStructure::cluster_end(int c){
	assert(1 <= c && c <= k);
	
	return Cluster_to_Points[c]->end();
}

int ClusteringDataStructure::get_num_of_points(){
	return Point_to_Cluster.size();
}

int ClusteringDataStructure::get_num_of_points_assigned(){
	return get_num_of_points() - Cluster_to_Points[0]->size();
}

int ClusteringDataStructure::get_cluster_size(int c){
	assert(0 <= c && c <= k);
	
	return Cluster_to_Points[c]->size();
}

int ClusteringDataStructure::get_k(){
	return k;
}

int ClusteringDataStructure::get_dim(){
	return dim;
}

int ClusteringDataStructure::get_cluster(Point* P){
	assert(member(P));
	
	return Point_to_Cluster[P].first;
}

bool ClusteringDataStructure::member(Point* P){
	auto it = Point_to_Cluster.find(P); return it != Point_to_Cluster.end();
}

double ClusteringDataStructure::metric_func(Point& X, Point& Y){
	return metric(X, Y);
}

//metrics
/* 
 * Given a point P returns
 * d = min{metric(P, c) | c is one of the first t centroids, t >= 1}
*/
double ClusteringDataStructure::distance_from_centroids(Point& P, int t){
	assert(1 <= t && t <= k);
	
	double dist_from_cent[t];
	for(int j = 0; j <= (t-1); j++)	dist_from_cent[j] = metric(P, *Cluster_to_Centroid[j+1]);
	
	double d; array_find_min(dist_from_cent, t, d);
	
	
	return d;
}

//returns also the centroid tha minimizes the distance
double ClusteringDataStructure::arg_distance_from_centroids(Point& P, int& c){
	
	double dist_from_cent[k];
	for(int j = 0; j <= (k-1); j++)  dist_from_cent[j] = metric(P, *Cluster_to_Centroid[j+1]);
	
	double d; array_find_min(dist_from_cent, k, d, c); c++;
	
	return d;
}

/* The same as above where t = k
 * min{metric(P, C) | C centroid}*/
double ClusteringDataStructure::distance_from_centroids(Point& P){
	return distance_from_centroids(P, k);
}



/* 
 * Computes the distance of the Centroids (defined above) for every
 * point in the DS:
 * 
 * Returns in argument array D an araay s.t.
 * D[i] = min{metric(P, c) | P is the i-th point in the DS,
 * 							 c is one of the first t centroids, t >= 1}
*/
void ClusteringDataStructure::distance_from_centroids(double D[], int array_size, int t /* subarray of centroids array*/){
	assert(1 <= t && t <= k);
	
	//cout << "ClusteringDataStructure::distance_from_centroids:" << endl;  //debug
	//cout << "array_size: " << array_size << endl;							//debug
	//cout << "t: " << t << endl;											//debug
	
	int i = 0;
	for(auto m = begin(); m != end(); m++){
		if(!Point_array_member(Cluster_to_Centroid, k+1, *get_point(m))){
			
			//cout << "i: " << i << endl;  //debug
			assert(( "D array out of bounds!", i <= (array_size - 1) ));
			
			D[i] = distance_from_centroids( *get_point(m), t );	
			
			//cout << "D[i]" << D[i] << endl;
			
			i++;
		}
	}
}

/* The same as above where t = k
 * D[i] = min{metric(P, C) | P is the i-th point in the DS,
 * 							 C a centroid}
 */
void ClusteringDataStructure::distance_from_centroids(double D[], int array_size){
	distance_from_centroids(D, array_size, k);
}

void ClusteringDataStructure::distance_from_centroids(Point& P, double Ds[], int array_size){
	assert(array_size == k);
	
	for(int i = 0 ; i <= k-1; i++)	Ds[i] = metric(P, *Cluster_to_Centroid[i+1]);
}

/* 
 * computes the mean point of a cluster M,
 * M = (1/n)*sum{P | P in cluster c}
 */
void ClusteringDataStructure::get_mean(int c, Point& M){
	assert(M.get_dim() == dim);
	assert(1 <= c && c <= k);
	
	if(get_cluster_size(c) == 0){
		M.set_point(*Cluster_to_Centroid[c]);
	}
	else{
		M.set_all_vis(0);
	
		for(auto p = Cluster_to_Points[c]->begin(); p != Cluster_to_Points[c]->end(); p++)  point_add(M, **p, M);
	
		M.scalar_divide(get_cluster_size(c));
	}
}



void ClusteringDataStructure::get_medoid(int c, Point& M){
	assert(M.get_dim() == dim);
	assert(1 <= c && c <= k);
	
	if(get_cluster_size(c) == 0){
		M.set_point(*Cluster_to_Centroid[c]);
	}
	else{
		double min = HUGE_VAL;
		Point* MinPoint;
		int i = 1;				//debug
		for(auto t = Cluster_to_Points[c]->begin(); t != Cluster_to_Points[c]->end(); t++){
			
			double S = 0;
			for(auto i = Cluster_to_Points[c]->begin(); i != Cluster_to_Points[c]->end(); i++){
				S += metric(**t, **i);
			}
		
			if(S < min){
				min = S;
				MinPoint = *t;
			}
			i++;
		}
		
		M.set_point(*MinPoint);
	}
}

double ClusteringDataStructure::average_dist_from_cluster(Point& P, int c){
	assert(1 <= c && c <= k);
	
	double S = 0;
	for(auto p : *Cluster_to_Points[c]) S += metric(P, *p);
	
	return S/Cluster_to_Points[c]->size();
}


//objective functions
/*
 * computes
 * sum{ (min{metric(P, C) | C centroid})^2 | P in DS}
 */
double ClusteringDataStructure::objective_function(){
	double D[get_num_of_points()];
	
	distance_from_centroids(D, get_num_of_points());
	
	return array_sum_of_squares(D, get_num_of_points());
}

double ClusteringDataStructure::cluster_silhouette(int c){
	assert(1 <= c && c <= k);
	
	double S = 0;
	for(auto p : *Cluster_to_Points[c]){
		
		int c2;
		/*find second best*/{
			double D[k]; distance_from_centroids(*p, D, k);
			D[c] = HUGE_VAL;							//min = +oo
			double min; array_find_min(D, k, min, c2); 	//now min = second_best_min
		}
		
		double a = average_dist_from_cluster(*p, c);
		double b = average_dist_from_cluster(*p, c2+1);
		
		S += (b - a)/max(a, b);
	}
	
	return S/get_cluster_size(c);
}

double ClusteringDataStructure::total_silhouette(){
	assert(get_num_of_points_assigned() == get_num_of_points());  //all points are assigned
	
	double S = 0;
	for(int c = 1; c <= k; c++){
		S += cluster_silhouette(c)*get_cluster_size(c);  //S = (S1/n1)*n1 + (S2/n2)*n2 + .. + (Sk/nk)*nk
	}
	
	return S/get_num_of_points();
}

void ClusteringDataStructure::get_silhouette_vector(double V[], int vector_size){
	assert(vector_size == k);
	
	for(int c = 1; c <= k; c++) V[c-1] = cluster_silhouette(c);
}










