// John Eblen
// August 15, 2007
// Serial and Parallel Direct Maximum Clique - version 4.1.0
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

// Serial and parallel versions are nearly identical.  Conditional compilation
// is used only in this file and is clearly marked with SVP comments (Serial
// Versus Parallel comments).

#include <Rcpp.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include "Graph.h"
#include "Candidate_Manager.h"
#include "Clique_Test.h"
#include "Preprocessor.h"
#include "Degree_Preprocessor.h"
#include "MC_Heuristic.h"
#include "Brancher.h"

using namespace std;
using namespace Rcpp;

Graph::Vertices *find_mc(Graph *g);

// [[Rcpp::export]]
StringVector maximum_clique(std::string filename)
{
  
  string graph_file(filename.c_str());
  Graph *g = new Graph(graph_file);
  g->sort_by_degree_asc();

  // Find maximum clique and print results
  Graph::Vertices *maximum_clique = find_mc(g);
  cerr << "Maximum clique size is:  " << maximum_clique->size() << endl;
  StringVector vertices = print_vertices(*maximum_clique);
  delete maximum_clique;

  return vertices;
}

// Main process to find a maximum clique.  Input is a graph and output is a
// maximum clique from that graph.  This function was "main" before DC version
// 4.1.1.  It was changed to accommodate paraclique and other methods that find
// multiple MCs during a single run.

// WARNING:  Input graph is altered!
Graph::Vertices *find_mc(Graph *g)
{
  int mc_size;


  // Variables for timing
  double start_time;
  struct timeval tval;
  struct timezone tzval;
  #define COMPUTE_SYS_TIME ((double)tval.tv_sec + ((double)tval.tv_usec)/1000000.0)


  // Start timing
  gettimeofday(&tval, &tzval);
  start_time = COMPUTE_SYS_TIME;

  // Run heuristic to find lower bound
  //BENCHMARK << "Applying heuristic" << endl;
  Greedy_Clique gc;
  mc_size = gc(*g);
  //BENCHMARK << "Heuristic clique size:  " << mc_size << endl;

  // Setup preprocessing strategy
  //BENCHMARK << "Setting up preprocessing" << endl;
  Preprocessor preproc;
  Color_Test ct;
  Lazy_Low_Degree_Man ldm;
  preproc.add_test(&ct, &ldm, 1);
  //BENCHMARK << "Ready to preprocess" << endl;

  // Preprocess graph
  //BENCHMARK "Preprocessing" << endl;
  preproc(*g, mc_size);
  //BENCHMARK << "Preprocessed to:  " << g->size() << " " << g->esize() << endl;

  // Shrink graph
  //BENCHMARK << "Shrinking graph" << endl;
  g->shrink();
  //BENCHMARK << "Shrinking complete" << endl;

  // Rerun heuristic
  Graph::Vertices *maximum_clique = new Graph::Vertices(*g);
  maximum_clique->clear();
  //BENCHMARK << "Applying heuristic again" << endl;
  gc(*g, *maximum_clique);
  //BENCHMARK << "Heuristic clique size:  " << maximum_clique->size() << endl;

  // Run branching
  //BENCHMARK << "Branching" << endl;
  Brancher branch;
  
  Prep_Low_Degree_Man pldm;
  branch.set_cand_man(&pldm);
  branch.set_best_to_worst_search();
  branch(*g, *maximum_clique);

  //BENCHMARK << "Finish" << endl;
  return maximum_clique;
}
