// John Eblen
// Direct clique using the MCQ strategy of Tomita and Kameda

#include <iostream>
#include <sys/time.h>
#include "Graph.h"
#include "Candidate_Manager.h"
#include "Clique_Test.h"
#include "Preprocessor.h"
#include "Degree_Preprocessor.h"
#include "MC_Heuristic.h"
#include "Brancher.h"

// SVP:  Libraries for parallel version only
#ifdef PARALLEL
#include "PBrancher.h"
#include "parMPI.h"
#endif
using namespace std;

Graph::Vertices *find_mc(Graph *g);

int main(int argc, char **argv)
{
  // SVP:  Parallel initialization
  #ifdef PARALLEL
  par_init(&argc, &argv);
  #endif

  // Process command-line arguments
  if (argc < 2)
  {
    cerr << "Usage:  " << argv[0] << " <DIMACS graph>" << endl;
    // SVP:  For parallel version, allow parallel library to properly exit
    #ifdef PARALLEL
    par_exit();
    #endif
    exit(EXIT_SUCCESS);
  }
  string graph_file(argv[1]);
  Graph *g = new Graph(graph_file);

  // Find maximum clique and print results
  Graph::Vertices *maximum_clique = find_mc(g);
  cerr << "Maximum clique size is:  " << maximum_clique->size() << endl;
  print_vertices(*maximum_clique);
  delete maximum_clique;

  // SVP:  For parallel version, allow parallel library to properly exit
  #ifdef PARALLEL
  par_exit();
  #endif
}

// Main process to find a maximum clique.  Input is a graph and output is a
// maximum clique from that graph.  This function was "main" before DC version
// 4.1.1.  It was changed to accommodate paraclique and other methods that find
// multiple MCs during a single run.

// WARNING:  Input graph is altered!
Graph::Vertices *find_mc(Graph *g)
{
  // SVP:  Process needs rank for parallel version
  #ifdef PARALLEL
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  #endif

  // Variables for timing
  double start_time;
  struct timeval tval;
  struct timezone tzval;
  #define COMPUTE_SYS_TIME ((double)tval.tv_sec + ((double)tval.tv_usec)/1000000.0)

  // SVP:  For parallel version, only rank 0 prints benchmarks
  #ifdef PARALLEL
  #define BENCHMARK gettimeofday(&tval, &tzval); if (rank == 0) cerr << COMPUTE_SYS_TIME - start_time << ":  "
  #else
  #define BENCHMARK gettimeofday(&tval, &tzval); cerr << COMPUTE_SYS_TIME - start_time << ":  "
  #endif

  // Create container for maximum clique
  Graph::Vertices *maximum_clique = new Graph::Vertices(*g);
  maximum_clique->clear();

  // Start timing
  gettimeofday(&tval, &tzval);
  start_time = COMPUTE_SYS_TIME;

  // Run heuristic to find lower bound
  BENCHMARK << "Applying heuristic" << endl;
  Greedy_Clique gc;
  gc(*g, *maximum_clique);
  BENCHMARK << "Heuristic clique size:  " << maximum_clique->size() << endl;
 
  // Run branching
  BENCHMARK << "Branching" << endl;

  // SVP:  Branching class (but not interface) differs for parallel version
  #ifdef PARALLEL
  PBrancher branch(1);
  #else
  Brancher branch;
  #endif

  g->sort_by_degree_asc();
  Lazy_High_Color_Man lhcm;
  branch.set_cand_man(&lhcm);
  branch.set_best_to_worst_search();
  branch(*g, *maximum_clique);
 
  BENCHMARK << "Finish" << endl;
  return maximum_clique;
}
