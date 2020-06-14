#include <list>
#include <string>
#include <set>

#include <iostream>
#include <fstream>
#include <string>

#include <cassert>
#include <cmath>
#include <cstring>

#include "./lib_io/preference_vector.h"
#include "./lib_io/io_methods.h"
#include "./lib_containers/list_operations.h"
#include "./lib_containers/set_operations.h"
#include "./lib_containers/array_operations.h"
#include "./lib_math/random.h"
#include "./lib_clustering/recommendation.h"
#include "./lib_clustering/clustering_algorithms.h"

using namespace std;

//constants
const string vader_lexicon 	= "./lexicons/vader_lexicon.csv";
const string coins_queries 	= "./lexicons/coins_queries.csv";

//hash table parameters
const int L 				= 5;

//clustering parameters
const int n_iterations		= 1000;
const int n_clusters		= 500;

//recommendation parameters
const int P						= 50;
const int n_to_recommend_users	= 5;
const int n_to_recommend_tweets	= 2;



/*
 *********************
 * Helping Functions *
 *********************
 */

bool check_arguments(int argc, char** argv){
	if(!(argc % 2 == 1))	return false;
	if(argc < 2)			return false;	
	
	//skip first argument (name of the func)
	//check if there is "-d", "-o" in args
	//check only odds args
	bool exist_inputfile = 	false;
	bool exist_outputfile = false;
	for(int i = 1; i <= (argc-1); i += 2){
		if(strcmp(argv[i], "-d") == 0)  	exist_inputfile = true;
		else if(strcmp(argv[i], "-o") == 0)	exist_outputfile = true;
	}
	
	return exist_inputfile && exist_outputfile;
}

void get_args(
				int argc,
				char** argv,
				char*& inputfile,
				char*& outputfile
			){
	for(int i = 1; i <= (argc-1); i += 2){
		if(strcmp(argv[i], "-d") == 0)  	inputfile 	= argv[i+1];
		else if(strcmp(argv[i], "-o") == 0)	outputfile 	= argv[i+1];
	}
}

int main(int argc, char** argv){
	
	char* out_file;
	char* filename;
	
	assert(check_arguments(argc, argv));
	get_args(argc, argv, filename, out_file);
	
	
	RandomGenerator Gen;
	
	//open ouput file
	ofstream out(out_file); assert(out.is_open());
	
	/*
	 ****************
	 * Get Lexicons *
	 ****************
	 */
	
	Sentiment Sent(vader_lexicon);
	ItemsDictionary Items(coins_queries);
	
	/*
	 *************
	 * Get input *
	 *************
	 */
	
	file2data get_input(Sent, Items, filename);
	list<PreferenceVector*> Users;
	list<PreferenceVector*> Tweets;
	get_input(Users, Tweets);
	
	//keep valids
	keep_valid_PVs(Users);
	keep_valid_PVs(Tweets);
	
	
	//normalize				
	normalize_PVs(Users);  for(auto u : Users)  u->scalar_product(1000.0);  //multiplying by 1000 gives more percise results
	normalize_PVs(Tweets);
	
	
	
	out << "**************************" << endl;
	out << "** Recommendation by ui **" << endl;
	out << "**************************" << endl;
		
	//LSH get neighbors
	const int k					= ceil(log2(Users.size()));
	const int hashtable_size	= pow(2, k);
	LSHCreateNeighborhoods LSH_get_neighbors(
											L,
											k,
											Items.get_n_items(),
											hashtable_size,
											Gen,
											(list<Point*>&) Users,
											P
										);
	
	list<set<PreferenceVector*>> LSH_Neighbors; LSH_get_neighbors((list<Point*>&) Users, (list< set<Point*> >&) LSH_Neighbors);
	
	
	//LSH rating
	list<PreferenceVector*> LSH_Rating;
	rate(Users, LSH_Neighbors, LSH_Rating);
	
	
	//printing
	out << endl;
	out << "**********************" << endl;
	out << "* LSH Recommendation *" << endl;
	out << "**********************" << endl;
	for(auto l : LSH_Rating){
		
		//get recommendations
		int RecommendedItems[n_to_recommend_users];
		recommend_top_items(
							*l,
							n_to_recommend_users,
							RecommendedItems,
							n_to_recommend_users
						);
		
		
		//print recommendations
		out << "[" << l->get_id() << "]" << " ";
		for(int i = 0; i <= n_to_recommend_users-1; ++i) out << Items.get_items_name(RecommendedItems[i]) << " ";
		out << endl;
	}
	
	for(auto l : LSH_Rating) delete l;
	
	
	
	//Clustering get neighbors
	ClusteringAlgorithm CA(
							set_rand_centroids,
							loyds_assignment,
							k_means,
							
							Gen,
							
							n_iterations
						);
	
	ClustreringCreateNeighborhoods Clustering_get_neighbors(
																n_clusters,
																Items.get_n_items(),
																euclidean_dist,
																(list<Point*>&) Users,
																CA,
																P
															);
															
	list< set<PreferenceVector*> > Clustering_Neighbors; Clustering_get_neighbors((list<Point*>&) Users, (list< set<Point*> >&) Clustering_Neighbors);
	
	//Clustering rating
	list<PreferenceVector*> Clustering_Rating;
	rate(Users, Clustering_Neighbors, Clustering_Rating);

	
	//printing
	out << endl;
	out << "*****************************" << endl;
	out << "* Clustering Recommendation *" << endl;
	out << "*****************************" << endl;
	for(auto l : Clustering_Rating){
		
		//get recommendations
		int RecommendedItems[n_to_recommend_users];
		recommend_top_items(
							*l,
							n_to_recommend_users,
							RecommendedItems,
							n_to_recommend_users
						);
		
		
		//print recommendations
		out << "[" << l->get_id() << "]" << " ";
		for(int i = 0; i <= n_to_recommend_users-1; ++i) out << Items.get_items_name(RecommendedItems[i]) << " ";
		out << endl;
	}
	
	for(auto l : Clustering_Rating) delete l;
	
	
	
	out << "**************************" << endl;
	out << "** Recommendation by ti **" << endl;
	out << "**************************" << endl;
	
	
	
	
	for(auto u : Users) delete u;
	for(auto t : Tweets) delete t;
	
	out.close();
	
	return 0;
}
