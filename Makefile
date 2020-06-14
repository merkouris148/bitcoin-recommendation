#libraries
CONTAINERS 	= ./lib_containers
MATH 		= ./lib_math
IO 			= ./lib_io
CLUSTERING 	= ./lib_clustering
LSH 		= ./lib_LSH

#main macros
MAIN_OBJ = main.o
OUT		 = recommendation

#scources
LIBS = 	$(MATH)/point.o  $(MATH)/random.o  $(MATH)/hypercube_graph.o  $(MATH)/integer.o  $(LSH)/point_list.o 	\
	  	$(LSH)/hashtable_vector.o  $(LSH)/hash_table.o  $(LSH)/euclidean_lsh.o  $(LSH)/cosine_lsh.o  			\
	  	$(LSH)/cube_lsh.o  $(CONTAINERS)/array_operations.o  $(CONTAINERS)/string_operations.o					\
	  	$(CLUSTERING)/clustering_data_structure.o  $(CLUSTERING)/clustering_algorithms.o						\
	  	$(CLUSTERING)/recommendation.o  $(IO)/lexicons.o  $(IO)/preference_vector.o  $(IO)/io_methods.o

#compliler macros

CC		= g++
FLAGS   = -std=c++11 -c -Wno-write-strings -Wno-reorder

#targets
$(OUT): $(MAIN_OBJ) $(OBJS) $(LIBS)
	$(CC) -g $(MAIN_OBJ) $(OBJS)  $(LIBS) -o $@ -lm


#mains 
main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

#IO
$(IO)/io_methods.o: $(IO)/io_methods.cpp
	$(CC) $(FLAGS) $(IO)/io_methods.cpp -o $(IO)/io_methods.o

$(IO/)/lexicons.o: $(IO)/lexicons.cpp
	$(CC) $(FLAGS) $(IO)/lexicons.cpp -o $(IO)/lexicons.o

$(IO)/preference_vector.o: $(IO)/preference_vector.cpp
	$(CC) $(FLAGS) $(IO)/preference_vector.cpp -o $(IO)/preference_vector.o

#Clustering
$(CLUSTERING)/clustering_data_structure.o: $(CLUSTERING)/clustering_data_structure.cpp
	$(CC) $(FLAGS) $(CLUSTERING)/clustering_data_structure.cpp -o $(CLUSTERING)/clustering_data_structure.o

$(CLUSTERING)/clustering_algorithms.o: $(CLUSTERING)/clustering_algorithms.cpp
	$(CC) $(FLAGS) $(CLUSTERING)/clustering_algorithms.cpp -o $(CLUSTERING)/clustering_algorithms.o

$(CLUSTERING)/recommendation.o: $(CLUSTERING)/recommendation.cpp
	$(CC) $(FLAGS) $(CLUSTERING)/recommendation.cpp -o $(CLUSTERING)/recommendation.o


#LSHs
$(LSH)/euclidean_lsh.o: $(LSH)/euclidean_lsh.cpp
	$(CC) $(FLAGS) $(LSH)/euclidean_lsh.cpp -o $(LSH)/euclidean_lsh.o

$(LSH)/cosine_lsh.o: $(LSH)/cosine_lsh.cpp
	$(CC) $(FLAGS) $(LSH)/cosine_lsh.cpp -o $(LSH)/cosine_lsh.o

$(LSH)/cube_lsh.o: $(LSH)/cube_lsh.cpp
	$(CC) $(FLAGS) $(LSH)/cube_lsh.cpp -o $(LSH)/cube_lsh.o

$(LSH)/hash_table.o: $(LSH)/hash_table.cpp
	$(CC) $(FLAGS) $(LSH)/hash_table.cpp -o $(LSH)/hash_table.o

$(LSH)/hashtable_vector.o: $(LSH)/hashtable_vector.cpp
	$(CC) $(FLAGS) $(LSH)/hashtable_vector.cpp -o $(LSH)/hashtable_vector.o

$(LSH)/point_list.o: $(LSH)/point_list.cpp
	$(CC) $(FLAGS) $(LSH)/point_list.cpp -o $(LSH)/point_list.o

#MATH
$(MATH)/hypercube_graph.o: $(MATH)/hypercube_graph.cpp
	$(CC) $(FLAGS) $(MATH)/hypercube_graph.cpp -o $(MATH)/hypercube_graph.o

$(MATH)/integer.o: $(MATH)/integer.cpp
	$(CC) $(FLAGS) $(MATH)/integer.cpp -o $(MATH)/integer.o

$(MATH)/point.o: $(MATH)/point.cpp
	$(CC) $(FLAGS) $(MATH)/point.cpp -o $(MATH)/point.o

$(MATH)/random.o: $(MATH)/random.cpp
	$(CC) $(FLAGS) $(MATH)/random.cpp -o $(MATH)/random.o

#containers
$(CONTAINERS)/array_operations.o: $(CONTAINERS)/array_operations.cpp
	$(CC) $(FLAGS) $(CONTAINERS)/array_operations.cpp -o $(CONTAINERS)/array_operations.o

$(CONTAINERS)/string_operations.o: $(CONTAINERS)/string_operations.cpp
	$(CC) $(FLAGS) $(CONTAINERS)/string_operations.cpp -o $(CONTAINERS)/string_operations.o

# clean house
clean:
	rm -i $(IO)/*.o  $(LSH)/*.o  $(CLUSTERING)/*.o  $(MATH)/*.o  $(CONTAINERS)/*.o  ./*.o  $(OUT)
