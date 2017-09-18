// John Eblen
// September 10, 2007
// Original nonoverlapping paraclique

#include <iostream>
#include <sys/time.h>
#include "Graph2.h"
#include "Candidate_Manager2.h"
#include "Clique_Test2.h"
#include "Preprocessor2.h"
#include "Degree_Preprocessor2.h"
#include "MC_Heuristic2.h"
#include "Brancher2.h"
#include "paracl.h"
#include <cstdlib>

// SVP:  Libraries for parallel version only
#ifdef PARALLEL
#include "PBrancher.h"
#include "parMPI.h"
#endif
using namespace std;

Graph::Vertices *find_mc(Graph *g);

int main(int argc, char **argv)
{
  int i;
  Graph::Vertices *maximum_clique;

  // Paraclique parameters
  double igf;
  int min_mc_size;
  int min_pc_size;
  int max_num_pcs;

  
  // SVP:  Parallel initialization
  #ifdef PARALLEL
  par_init(&argc, &argv);
  #endif

  // Process command-line arguments
  if (argc < 6)
  {
    cerr << "Usage:  " << argv[0] << " <DIMACS graph> <igf> <Min. maximum clique size> <Min. paraclique size> <Max. no. paracliques>" << endl;
    // SVP:  For parallel version, allow parallel library to properly exit
    #ifdef PARALLEL
    par_exit();
    #endif
    exit(EXIT_SUCCESS);
  }
  string graph_file(argv[1]);
  Graph *g = new Graph(graph_file);
  Graph::Vertices paraclique(*g);
  igf = atof(argv[2]); 
  min_mc_size = max(3, atoi(argv[3]));
  min_pc_size = max(3, atoi(argv[4]));
  max_num_pcs = atoi(argv[5]);
  if (max_num_pcs <= 0) max_num_pcs = g->size();

  for (i=0; i<max_num_pcs; ++i)
  {
    // Use copy of graph for finding maximum clique (find_mc is destructive)
    Graph g_copy(*g);
    maximum_clique = find_mc(&g_copy);
    cerr << "Maximum clique size is:  " << maximum_clique->size() << endl;
    if (maximum_clique->size() < min_mc_size) break;
 
    // Extract results
    paraclique.clear();
    Graph::Vertices::Vex_ptr vp(*maximum_clique);
    for (; !vp.end(); ++vp) paraclique.insert(g->vertex(g_copy.label(*vp)));
    delete maximum_clique;

    build_paracl(*g, paraclique, igf);
    if (paraclique.size() < min_pc_size) break;

    print_vertices_oneline(paraclique);
    g->remove(paraclique);
  }

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
  int mc_size;

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
  g->sort_by_degree_asc();
  Preprocessor preproc;
  Size_Test st;
  Color_Test ct;
  Candidate_Manager cm;
  Lazy_Low_Degree_Man ldm;
  // preproc.add_test(&st, &cm, 2);
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
  #ifdef PARALLEL
  PBrancher branch(1);
  #else
  Brancher branch;
  #endif

  Prep_Low_Degree_Man pldm;
  branch.set_cand_man(&pldm);
  // branch.branch_dpp_on();
  // Lazy_High_Degree_Man lhdm;
  // branch.set_cand_man(&lhdm);
  branch.set_best_to_worst_search();
  branch(*g, *maximum_clique);

  BENCHMARK << "Finish" << endl;
  return maximum_clique;
}
