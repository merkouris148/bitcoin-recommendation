#include <iostream>

#include <cassert>

#include "hypercube_graph.h"
#include "integer.h"

using namespace std;

//constructor distructor
HyperCubeGraph::HyperCubeGraph(int degree){
	assert(degree >= 1);  //we don't handle trivial graphs with V = {v}, E = {}
	
	deg = degree;			//in a cubig graph degree is also the length of the string labels
	n = int_pow(2, deg);
	
	adj_list = new int*[n];
	for(int i = 0; i <= (n-1); i++)	adj_list[i] = new int[deg];
	
	//fill the adjancy list
	//for every vertex v in V
	for(int v = 0; v <= (n-1); v++){
		int neigh = 0;
		//for every other vertex u in V
		for(int u = 0; u <= (n-1); u++){
			//hamming_distance(u, v) == 1 --> {v, u} in E
			if(hamming_dist(v, u) == 1){
				adj_list[v][neigh] = u;
				neigh ++;
			}
		}
	}
}

HyperCubeGraph::~HyperCubeGraph(){
	for(int i = 0; i <= (n-1); i++)	delete[] adj_list[i];
	
	delete[] adj_list;
}

//debug
void HyperCubeGraph::print(){
	for(int i = 0; i <= (n-1); i++){
		cout << "[" << i << "]" << "  ";
		for(int j = 0; j <= (deg-1); j++){
			cout << adj_list[i][j] << " ";
		}
		cout << endl;
	}
}

//accessor
void HyperCubeGraph::get_neighbors(int v, int neigh_v[]){
	assert(0 <= v && v <= (n-1));
	assert(neigh_v != NULL);
	
	for(int i = 0; i <= (deg-1); i++)	neigh_v[i] = adj_list[v][i];
}

int HyperCubeGraph::get_deg(){
	return deg;
}
