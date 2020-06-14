#ifndef HYPERCUBE_GRAPH_H

#define HYPERCUBE_GRAPH_H

class HyperCubeGraph{
private:
	int deg;
	int n;
	//int m;
	int** adj_list;
public:
//constructor destructor
	HyperCubeGraph(int degree);
	~HyperCubeGraph();
//debug
	void print();
//accessor
	void get_neighbors(int v, int neigh_v[]);
	int get_deg();
};


#endif
