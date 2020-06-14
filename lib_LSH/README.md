# LSH library

In this library we implement various [Local Sensitive Hashing algorithms](https://en.wikipedia.org/wiki/Locality-sensitive_hashing).

We implemented two variations of LSH using the [euclidean metric](https://en.wikipedia.org/wiki/Euclidean_distance) and the [cosine similarity](https://en.wikipedia.org/wiki/Cosine_similarity)  metric. On top of these two, we implemented a projection to [hypecube](https://en.wikipedia.org/wiki/Hypercube) storage algorithm. The purpose of the data structures and algorithms presented here is to answer [nearest neighbor](https://en.wikipedia.org/wiki/Nearest_neighbor_search#See_also) and [range search](https://en.wikipedia.org/wiki/Range_searching) queries efficiently. In order to overcome the [curse of dimensionality](https://en.wikipedia.org/wiki/Curse_of_dimensionality), all the algorithms presented here are [approximate](https://en.wikipedia.org/wiki/Approximation_algorithm) and [randomized](https://en.wikipedia.org/wiki/Randomized_algorithm). We will explain here in detail the techniques used.

## Hash Tables

We approach the data structures design bottom-up.

### Point List

This data structure implements a list of points in the IR^n vector space. The points are implemented in `lib_math`. The point list supports two methods:

* ```cpp
  double exact_NN_opt(Point& Q, Point& W, double (*metric) (Point&, Point&))
  ```
  
  The `exact_NN_opt` method takes as argument a point in IR^n (`Q`) and a metric function (`metric`) and returns as point `W` the nearest neighbor of `Q` and distance from `P` to `Q`, namely the value `metric(P, Q)`.

* ```cpp
  bool exact_NN_dec(Point& Q, list<Point*>& WitnessList, double (*metric) (Point&, Point&), double radius)
  ```
  
  The `exact_NN_dec` method takes as argument a point in IR^n (`Q`), a metric function (`metric`) and a `radius`. It returns as `WitnessList` the points of the list that in radius `radius` from the query point `Q`, namely all the points `W`, of the list, for which the relation  `metric(Q, W) < radius` holds. The return value is `true` if the `WitnessList` is not empty and `false` otherwise.

### Hash Table

This data structure implements an array  of `point_lists`. The hash table data structure supports the following methods:

* ```cpp
  double get_NN_opt(
                      Point& Q,                                    //Query Point
                      Point& W,                                    //Witness
                      double (*metric) (Point& P1, Point& P2),    //metric function
                      int h                                        //num of bucket
                  )
  ```
  
  This method just applies the `exact_NN_opt` query to the h-th bucket of the hash table.

* ```cpp
  double get_NN_opt(
                      Point& Q,                                    //Query Point
                      Point& W,                                    //Witness
                      double (*metric) (Point& P1, Point& P2),    //metric_func
                      int H[],                                    //array of bucket indexes
                      int array_size
                  )
  ```
  
  This method applies the `exact_NN_opt` query to all the buckets apointed by the indicies in the `H` array. It returns the optimal nearest neighbor as the point `W` and the distance from `Q` to `W`.

* ```cpp
  bool get_NN_dec(
                      Point& Q,                                    //Query Point
                      std::list<Point*>& WitnessList,                //Witness
                      double (*metric) (Point& P1, Point& P2),    //metric_func
                      double radius,                                 //radius
                      int h                                        //num_of_bucket
                  )
  ```
  
  This method just applies the `exact_NN_dec` query to the h-th buck of the hash table.

* ```cpp
  bool get_NN_dec(
                      Point& Q,                                    //Query Point
                      std::list<Point*>& WitnessList,                //Witness
                      double (*metric) (Point& P1, Point& P2),    //metric_func
                      double radius,                                 //radius
                      int H[],                                    //num_of_bucket
                      int array_size
                  )
  ```
  
  This method works simmilarly to the second `get_NN_opt` method mentioned above.

* ```cpp
  void get_k_NN(
                      Point& Q,                            //Query Point
                      std::set<Point*>& Witnesses,        //Witnesses
                      double (*metric)(Point&, Point&),    //metric
                      int k,                                //num of neighbors
                      int h                                //bucket
                  )
  ```
  
  This method applies the `exact_NN_opt` query to the h-th bucket of the hash table and returns as `Witnesses` the top k nearest neighbors.

### Hash Table Vector

In this project we will be using multiple hash table and we will regard the as a single structure. All the input points will be stored in all of the hash tables. The queries will be  routed to all the hash tables. We will use `L` as the parameter determing the quantity of the hash tables. The greater the `L` the more points will be consider to answer the nearest neighbor and range search queries, but this will also make the computation of the queries slower. The `hash table vector` data structure wraps together `L` `hash tables` and routes queries to them. This data structure provides the following methods:

* ```cpp
  double get_NN_opt(
                      Point& Q,                                    //Query Point
                      Point& W,                                    //Witness
                      double (*metric) (Point& P1, Point& P2),    //metric function
                      int l,
                      int h
                  )
  ```
  
  This method applies the `exact_NN_opt` method to the h-th bucket of the l-th hash table.

* ```cpp
  double get_NN_opt(
                      Point& Q,                                    //Query Point
                      Point& W,                                    //Witness
                      double (*metric) (Point& P1, Point& P2),    //metric_func
                      int Ls[],                                    //array of buckets
                      int array_size
                  )
  ```
  
  This method applies the `exact_NN_opt` method to all hash tables of the hash table vector to all the coresponding buckets pointed out by the index array `Ls`.

* ```cpp
  bool get_NN_dec(
                      Point& Q,                                    //Query Point
                      std::list<Point*>& WitnessList,                //Witness
                      double (*metric) (Point& P1, Point& P2),    //metric_func
                      double radius,                                 //radius
                      int l,
                      int h
                  )
  ```
  
  This method applies the `exact_NN_dec` method to the h-th backet of the l-th hash table.

* ```cpp
  bool get_NN_dec(
                      Point& Q,                                    //Query Point
                      std::list<Point*>& WitnessList,                //Witness
                      double (*metric) (Point& P1, Point& P2),    //metric_func
                      double radius,                                 //radius
                      int Ls[],                                    //array of buckets
                      int array_size
                  )
  ```
  
  This method applies the `exact_NN_dec` method to all hash tables of the hash table vector to all the coresponding buckets pointed out by the index array `Ls`.

* ```cpp
  void get_k_NN(
                  Point& Q,                            //Query Point
                  std::set<Point*>& Witnesses,        //Witnesses
                  double (*metric)(Point&, Point&),    //metric
                  int k,                                //num of neighbors
                  int l,
                  int h
              )
  ```
  
  This method applies the `get_k_NN` methodto the h-th bucket of the l-th hash table.

* ```cpp
  void get_k_NN(
                  Point& Q,                            //Query Point
                  std::set<Point*>& Witnesses,        //Witnesses
                  double (*metric)(Point&, Point&),    //metric
                  int k,                                //num of neighbors
                  int Ls[],
                  int array_size
              )
  ```
  
  This method applies the `get_k_NN` method to all hash tables of the hash table vector to all the coresponding buckets pointed out by the index array `Ls`.

## Local Sensitive Hash functions

You may observed that above, despite using the term "hash table", we didn't mention, yet, a hash function. That's because we keep the implementation of the storage data structure apart from the implementation of a hashing method. This makes the use of different hashing methods easier to implement and the code cleaner.

We will describe bellow two families of local sensitive hash functions, one for the euclidean metric and the other for the coseine similarity. We will follow the same aproach in the two families described. Firstly, we will give a function family

`H = {h | h: IR^n --> IN}`

then we choose randomly `k` of these functions and we construct an *amplified hash function*:

`g(p) = [h_1(p), h_2(p), ..., h_k(p)]`

Lastly, from `g(p)` we define `phi: IR^n --> IN mod TableSize`. 

### Euclidean hash functions

We describe, now, a family of euclidean hash functions. Consider a **vector** **v** in IR^n choosen [normally](https://en.wikipedia.org/wiki/Normal_distribution). Let `w` be a **positive integer**. Also, let `t` be a **real number** choosen [uniformly](https://en.wikipedia.org/wiki/Uniform_distribution_(continuous)). For every point **p** in IR^n, we define:

`h(p) = ceil[ (p*v + t) / w]`

Observe that `h(p)` is well defined and is an integer.

Next, we pick `k` hash fuctions, of the function family and define the **amplified hash function** concatinating the values of the `h(p)`:

`g(p) = [h_1(p), h_2(p), ..., h_k(p)]`

So we define `phi` as random combination of the hi ’s as follows:

`phi(p) = r_1*h_1(p) + r_2*h_2(p) + ... +r_k*h_k(p) mod TableSize`

where the `r_i` are integers picked randomly.

### Cosine Similarity LSH

We describe, here, a family of hash functions for the cosine similarity metric. Let **r_i** be a vector in IR^n choosen normally. For every **p** in IR^n, we define:

`h(p) = 1, r_i*p = 0`

`h(p) = 0, otherwise`

Again, we pick `k` hash functions, of the function family described above and define the **amplified hash function** `g(p)` concatinating the values of the `h(p)`. Observe that, since h(p) takes only the values 0,1, the  `g(p)` is a binary integer. Thus, we define `phi(p)` to just be `g(p)`.

### Multiple hash functions

Since we use multiple hash tables (see hash table vector above) the hash function will be using is of the following form:

`phis: IR^n --> IN^L`

that takes a point in IR^n as argument and returns an array of indicies pointing to hash table positions. The `phis` function is just a concatenation of `L` `phi` functions. Namely:

`phis(p) = [phi_1(p), phi_2(p), ..., phi_L(p)]`

We wraped the multiple hash functions in the `LSHsVector` class. This class  is a [template](https://en.wikipedia.org/wiki/Template_(C%2B%2B)), therefore, the `phi` functions *may be parametrized so that implement, euclidean or cosine similarity `phi`  function*. We implemented the `phis` function with the method bellow:

```cpp
void phis(Point& P, int phis_vector[], int array_size, int hash_table_size = 0)
```

## Projection to Hypercube

Above the two methods we described, we implemented the projection to hypercube method for [dimensionality reduction](https://en.wikipedia.org/wiki/Dimensionality_reduction). Let a `g(p)` function as we desrcibed above. Also, let `f: IN --> {0,1}`. Given the value `g(p)` we compute a new function `g'(p)`, where:

`g'(p) = [f_1(h_1(p)), f_2(h_2(p)), ..., f_k(h_k(p))]`,

and the h_i(p) values are the values we got from g(p). This way we projected a point of the IR^n space to the k-dimensional hypercube. Agian, observe that `g'(p)` is a binary integer, and we define `phi(p) = g'(p)`. 

This aproaches uses a **single hash table**. After a point is stored in the hypercube, we may search the neighbor nodes. To keep track of who the neighbor nodes are, we implemented a `HyperCubeGraph` data structure (see `lib_math` directory), while the algorith is implemented by the `cube_lsh` files.
