// John Eblen
// August 15, 2007
// Serial Direct Maximum Clique - version 4.1.0
// A modular maximum clique finder, which implements a complete strategy for
// finding a maximum clique in a graph.  The strategy itself is highly
// configurable, which supports research into effective strategies for various
// graph types.  A clique strategy consists of these three basic components:
// 1.  Quick heuristic to find a lower bound for maximum clique
// 2.  Preprocessing
// 3.  Branching
// Classes support all three and can be configured separately.  For example,
// branching can be altered in how nodes are selected during branching, in
// what interleaved preprocessor to use (if any), in whether or not to use
// simple low/high degree preprocessing, and in how often the latter two
// should be applied.


#include <Rcpp.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include "Graph.h"
#include "Maxclique_Finder.h"

using namespace std;
using namespace Rcpp;

Graph::Vertices *find_mc(Graph *g);

// [[Rcpp::export]]
StringVector maximum_clique(std::string filename)
{
  
  string graph_file(filename.c_str());
  Graph *g = new Graph(graph_file);
  g->sort_by_degree_asc();

  // Find maximum clique and return it
  Graph::Vertices *maximum_clique = find_mc(g);
  StringVector vertices = print_vertices(*maximum_clique);
  delete maximum_clique;

  return vertices;
}

