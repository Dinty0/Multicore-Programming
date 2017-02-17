/*
  floyd-sequential --
  
  C++ implementation of the Floyd-Warshall algorithm.
  http://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm

  Usage:
  floyd-sequential n

  where 'n' is the number of nodes of a random network.

  The program will compute and display the shortest weighted path between node 0 and
  an arbitrary other node in a random graph of size n.

  Implementation using C++11. Compile as:
  g++ -Wall -std=gnu++11 -o floyd-sequential floyd-sequential.cpp
or
  g++ -Wall -std=gnu++0x -o floyd-sequential floyd-sequential.cpp


  University of Nantes, X8II070 "Multicore Programming"
  Laboratory Work 2

  Author: Frédéric Goualard <Frederic.Goualard@univ-nantes.fr>
  v. 1.3, 2017-02-06 
        Corrected a typo in the comments.
  v. 1.2, 2016-02-09
 */
#include <iostream>
#include <limits>
#include <vector>
#include <iterator>
#include <cstdint>
#include <omp.h>
#include "matrix.h"

using namespace std;

// A path is a list of nodes
typedef vector<uint32_t> path_t;

const double inf = numeric_limits<double>::infinity();


void floyd_warshall(const DMatrix& W, 
		    DMatrix& dist, 
		    IMatrix& next)
{
  uint32_t nvertices = W.rows();

  for (uint32_t i = 0; i < nvertices; ++i) {
    for (uint32_t j = 0; j < nvertices; ++j) {
      dist(i,j) = W(i,j);
    }
  }

  for (uint32_t k=0; k < nvertices; ++k) {
    for (uint32_t i=0; i < nvertices; ++i) {
      for (uint32_t j=0; j < nvertices; ++j) {
	double tmp = dist(i,k) + dist(k,j);
	if (tmp < dist(i,j)) {
	  dist(i,j) = tmp;
	  next(i,j) = k;
	}
      }
    }
  }
}

// Return the shortest path between vertices i and j
path_t recover_path(const DMatrix& dist, 
		const IMatrix& next, uint32_t i, uint32_t j)
{
  path_t p;
  if (dist(i,j) == inf) { // No path between i and j
    return p;
  }
  int intermediate = next(i,j);
  if (intermediate == -1) { // Edge i--j is shortest path
    p.insert(p.begin(),i);
    p.insert(p.end(),j);
    return p;
  } else {
    path_t l = recover_path(dist,next,i,intermediate);
    if (l.size() == 0) { // No path found?
      return p;
    }
    path_t r = recover_path(dist,next,intermediate,j);
    if (r.size() == 0) { // No path found?
      return p;
    }
    p.insert(p.begin(),l.begin(),l.end());
    p.insert(p.end(),r.begin()+1,r.end());
    return p;
  }
}


// Generation of a random network with 'sz' nodes
DMatrix generate_matrix(uint32_t sz)
{
  DMatrix W(sz,sz);

  for (uint32_t i = 0; i < sz; ++i) {
    for (uint32_t j = 0; j < sz; ++j) {
      W(i,j) = double(rand() % 100)+1;
      if ((rand()% 10 < 2)) { // Introducing some sparsity
	W(i,j) = inf;
      }
    }
  }
  
  for (uint32_t i = 0; i < sz; ++i) {
    W(i,i) = 0.0;
  }
  return W;
}

int main(int argc, char *argv[])
{
  /*
  DMatrix W { 5, 5, 
      {   0,   4, inf,   2, inf, 
	inf,   0,   3, inf,   1, 
	  5, inf,   0,   1, inf, 
          7, inf, inf,   0,   5,
	  3, inf, inf, inf,   0 } };
  */

  // Checking that the value of n is given
  if (argc != 2) {
    cerr << "floyd-sequential n" << endl;
    exit(1);
  }
  int n = atoi(argv[1]);
  if (n == 0) {
    cerr << argv[1] << " is a wrong parameter" << endl;
    exit(1);
  }
   
  srand(10);

  DMatrix W = generate_matrix(n);

  DMatrix dist(W.rows(),W.cols(),inf);
  IMatrix next(W.rows(),W.cols(),-1);


  double start = omp_get_wtime();
  floyd_warshall(W, dist, next); 


  // Computing the path between node 0 and an arbitrary other node
  path_t p = recover_path(dist,next,0,(rand()%(n-1))+1);
  double end = omp_get_wtime();

  if (p.size() == 0) {
    cout << "No path" << endl;
  } else {
    copy(p.begin(), p.end(), ostream_iterator<int>(cout, " "));
    cout << "\n";
    double cost = 0;
    for (uint32_t i = 0; i < p.size()-1; ++i) {
      cost += dist(p[i],p[i+1]);
    }
    cout << "Cost: " << cost << "\n";
  }
  
  cout << "Elapsed time: " << (end-start) << endl;
}
