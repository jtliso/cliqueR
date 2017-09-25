// John Eblen
// September 10, 2007
// Original nonoverlapping paraclique

#include <Rcpp.h>
#include <iostream>
#include <sys/time.h>
#include "Graph.h"
#include "paracl.h"
#include "Maxclique_Finder.h"
//#include <cstdlib>

// SVP:  Libraries for parallel version only
/*#ifdef PARALLEL
#include "PBrancher.h"
#include "parMPI.h"
#endif*/

using namespace std;
using namespace Rcpp;
using std::vector;

// [[Rcpp::export]]
std::vector<std::vector<std::string> > find_paracliques(std::string filename, double igf, int min_mc_size, int min_pc_size, int max_num_pcs)
{
  int i;
  Graph::Vertices *maximum_clique;
  std::vector< std::vector<string> > paracliques;

 
  // SVP:  Parallel initialization
  /*#ifdef PARALLEL
  par_init(&argc, &argv);
#endif*/


  string graph_file(filename);
  Graph *g = new Graph(graph_file);
  Graph::Vertices paraclique(*g);
  //igf = atof(argv[2]); 
  min_mc_size = max(3, min_mc_size);
  min_pc_size = max(3, min_pc_size);
//  max_num_pcs = atoi(argv[5]);
  if (max_num_pcs <= 0) max_num_pcs = g->size();

  for (i=0; i<max_num_pcs; ++i)
  {
    // Use copy of graph for finding maximum clique (find_mc is destructive)
    Graph g_copy(*g);
    maximum_clique = find_mc(&g_copy);
    //cerr << "Maximum clique size is:  " << maximum_clique->size() << endl;
    if (maximum_clique->size() < min_mc_size) break;
 
    // Extract results
    paraclique.clear();
    Graph::Vertices::Vex_ptr vp(*maximum_clique);
    for (; !vp.end(); ++vp) paraclique.insert(g->vertex(g_copy.label(*vp)));
    delete maximum_clique;

    build_paracl(*g, paraclique, igf);
    if (paraclique.size() < min_pc_size) break;

    paracliques.push_back(print_vertices_oneline(paraclique));
    g->remove(paraclique);
  }
  return paracliques;
  
  /* SVP:  For parallel version, allow parallel library to properly exit
  #ifdef PARALLEL
  par_exit();
  #endif*/
}
