#include "recommendation.h"

#include "../lib_containers/array_operations.h"

using namespace std;

//const bool warnings = true;
//const bool on_warnings_continue = true;

/*
 ************************
 * LSH Create Neighbors * 
 ************************
*/

//constructor destructor
LSHCreateNeighborhoods::LSHCreateNeighborhoods(
												int LL,
												int k,
												int dim,
												int hashtable_size,
												
												RandomGenerator& Gen,
												
												list<Point*>& Users,
												
												int PP
											) :
												LSH(LL, Gen, k, dim),
												HashTable(LL, hashtable_size, dim),
												
												L(LL),
												
												P(PP)
											{
	for(auto u : Users){
		int Ls[L];  LSH.phis(*u, Ls, L);
		HashTable.save_point_to_bucket(u, Ls, L);
	}
	
	//HashTable.print();
}


//functor
void LSHCreateNeighborhoods::operator()(
										std::list<Point*>& Users,
										std::list< std::set<Point*> >& N_Users
									){
										
	for(auto u : Users){
		int Ls[L];  LSH.phis(*u, Ls, L);
		
		set<Point*> Witnesses; HashTable.get_k_NN(*u, Witnesses, cosine_dist, P, Ls, L);
		N_Users.push_back(Witnesses);
	}
}


/*
 ***********************************
 * Clustering Create Neighborhoods *
 ***********************************
 */

ClustreringCreateNeighborhoods::ClustreringCreateNeighborhoods(
																int kk,						//# clusters
																int ddim,
																double (*m) (Point& X, Point& Y),
									
																std::list<Point*>& Users,	//neighbors pool
																
																ClusteringAlgorithm& CCA,
																
																int PP
															) :
																CDS(Users.begin(), Users.end(), kk, ddim, m),
																CA(CCA),
																P(PP)
															{	
	CA(CDS);
}

//functor
void ClustreringCreateNeighborhoods::operator()(
												list<Point*>& Users,
												list< set<Point*> >& N_Users
											){
	int c;
	for(auto u : Users){
		if(CDS.member(u)) 	c = CDS.get_cluster(u);
		else  				CDS.arg_distance_from_centroids(*u, c);
		
		set<Point*> Neighbors;
		int i = 1;
		for(auto it = CDS.cluster_begin(c); it != CDS.cluster_end(c); ++it){
			
			if(i > P)  break;
			
			Neighbors.insert(*it);
			
			++i;
		}
		
		N_Users.push_back(Neighbors);
	}
}







