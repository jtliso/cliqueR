// John Eblen
// July 14, 2006
// The Recursive General Preprocessing Algorithm (RGPA) and a class to
// encapsulate a strategy for using it.  Note that the RGPA is NOT part of
// the class (it can be used separately).  However, conceptually, the
// Preprocessor class allows you to build your own custom preprocessor.
// You can create a list of algorithms to run, set the stopping criteria, and
// use it as a repository to store the results (the node sets eliminated).

#ifndef _PREPROCESSOR_H
#define _PREPROCESSOR_H 1

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <sys/time.h>
#include "Graph2.h"
#include "Basic_tree2.h"
#include "Clique_Test2.h"
#include "Candidate_Manager2.h"
using namespace std;

bool prep(Graph::Vertices &mygraph, int min_clique_size, int level,
          const Clique_Test &, const Candidate_Manager *, basic_tree *);

class Preprocessor
{
  // Set of preprocessing algorithms
  // Each is described by these three components.
  typedef struct prep_alg_struct
  {
    const Clique_Test *test;
    Candidate_Manager *cand_man;
    int level;

    prep_alg_struct(const Clique_Test *ct, Candidate_Manager *cm, int l)
    :test(ct), cand_man(cm), level(l) {}
  } prep_alg;
  vector<prep_alg> alg;

  // Stopping criterion can be based on number of nodes or number of edges
  // and is measured against a certain ratio.
  // NOTE:  NOT fully flexible.  Stop criterion must be the same for all
  // algorithms.
  enum {NODE, EDGE};
  bool stop_criterion;
  double stop_ratio;

  // Results of operation - a record of sets ruled out of being in the clique.
  // NOTE:  Reset each time the preprocessor is called (using () )
  basic_tree eliminated_sets;

  public:
  Preprocessor() {stop_criterion = NODE; stop_ratio = 0.9;}

  // Basic Access Functions
  basic_tree *get_results() {return &eliminated_sets;}
  void stop_on_edges() {stop_criterion = EDGE;}
  void stop_on_nodes() {stop_criterion = NODE;}
  void set_stop_ratio(double r) {stop_ratio = r;}
  void add_test(const Clique_Test *ct, Candidate_Manager *cm, int l)
                                       {alg.push_back(prep_alg(ct, cm, l));}

  // Main Operation
  bool operator() (Graph &mygraph, int min_clique_size);
  bool operator() (Graph::Vertices &mygraph, int min_clique_size);
};

#endif
