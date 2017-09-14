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

#include <iostream>
#include <sys/time.h>
#include <Rcpp.h>
#include "Graph.h"
#include "Candidate_Manager.h"
#include "Clique_Test.h"
#include "Preprocessor.h"
#include "Degree_Preprocessor.h"
#include "MC_Heuristic.h"
#include "Brancher.h"

// SVP:  Libraries for parallel version only
/*#ifdef PARALLEL
#include "PBrancher.h"
#include "parMPI.h"
#endif*/

using namespace std;
using namespace Rcpp;

Graph::Vertices *find_mc(Graph *g);
List rlist_from_vertices(Graph::Vertices &v);

// [[Rcpp::export]]
List max_clique_cpp(string graph_file)
{
  // SVP:  Parallel initialization
  /*#ifdef PARALLEL
  par_init(&argc, &argv);
  #endif*/

  // Process command-line arguments
  /*if (argc < 2)
  {
    //cerr << "Usage:  " << argv[0] << " <DIMACS graph>" << endl;
    // SVP:  For parallel version, allow parallel library to properly exit
    #ifdef PARALLEL
    par_exit();
    #endif
    exit(EXIT_SUCCESS);
  }*/
  //string graph_file(argv[1]);
  Graph *g = new Graph(graph_file);
  g->sort_by_degree_asc();

  // Find maximum clique and print results
  Graph::Vertices *maximum_clique = find_mc(g);
  cerr << "Maximum clique size is:  " << maximum_clique->size() << endl;
  //print_vertices(*maximum_clique);
  maximum_clique_rlist = rlist_from_vertices(*maximum_clique);
  delete maximum_clique;
  return maximum_clique_rlist;
	
  // SVP:  For parallel version, allow parallel library to properly exit
  /*#ifdef PARALLEL
  par_exit();
  #endif*/
}

// Main process to find a maximum clique.  Input is a graph and output is a
// maximum clique from that graph.  This function was "main" before DC version
// 4.1.1.  It was changed to accommodate paraclique and other methods that find
// multiple MCs during a single run.

// WARNING:  Input graph is altered!
Graph::Vertices *find_mc(Graph *g)
{
  int mc_size;

  // SVP:  Process needs rank for parallel version
  /*#ifdef PARALLEL
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  #endif*/

  // Variables for timing
  double start_time;
  struct timeval tval;
  struct timezone tzval;
  #define COMPUTE_SYS_TIME ((double)tval.tv_sec + ((double)tval.tv_usec)/1000000.0)

  // SVP:  For parallel version, only rank 0 prints benchmarks
  /*#ifdef PARALLEL
  #define BENCHMARK gettimeofday(&tval, &tzval); if (rank == 0) cerr << COMPUTE_SYS_TIME - start_time << ":  "
  #else
  #define BENCHMARK gettimeofday(&tval, &tzval); cerr << COMPUTE_SYS_TIME - start_time << ":  "
  #endif*/

  // Start timing
  gettimeofday(&tval, &tzval);
  start_time = COMPUTE_SYS_TIME;

  // Run heuristic to find lower bound
  BENCHMARK << "Applying heuristic" << endl;
  Greedy_Clique gc;
  mc_size = gc(*g);
  BENCHMARK << "Heuristic clique size:  " << mc_size << endl;

  // Setup preprocessing strategy
  BENCHMARK << "Setting up preprocessing" << endl;
  Preprocessor preproc;
  Color_Test ct;
  Lazy_Low_Degree_Man ldm;
  preproc.add_test(&ct, &ldm, 1);
  BENCHMARK << "Ready to preprocess" << endl;

  // Preprocess graph
  BENCHMARK "Preprocessing" << endl;
  preproc(*g, mc_size);
  BENCHMARK << "Preprocessed to:  " << g->size() << " " << g->esize() << endl;

  // Shrink graph
  BENCHMARK << "Shrinking graph" << endl;
  g->shrink();
  BENCHMARK << "Shrinking complete" << endl;

  // Rerun heuristic
  Graph::Vertices *maximum_clique = new Graph::Vertices(*g);
  maximum_clique->clear();
  BENCHMARK << "Applying heuristic again" << endl;
  gc(*g, *maximum_clique);
  BENCHMARK << "Heuristic clique size:  " << maximum_clique->size() << endl;

  // Run branching
  BENCHMARK << "Branching" << endl;

  // SVP:  Branching class (but not interface) differs for parallel version
  /*#ifdef PARALLEL
  PBrancher branch(1);
  #else
  Brancher branch;
  #endif*/

  Prep_Low_Degree_Man pldm;
  branch.set_cand_man(&pldm);
  branch.set_best_to_worst_search();
  branch(*g, *maximum_clique);

  BENCHMARK << "Finish" << endl;
  return maximum_clique;
}

List r_list_from_vertices(Graph::Vertices &v)
{
  Graph *g = v.graph();
  Graph::Vertices::Vex_ptr p(v);
  List vert_list(v.size());
  int i = 0;
  while (!p.end())
  {
    vert_list.[i] = g->label(*p);
    ++p;
	i++
  }
  return vert_list;
}

