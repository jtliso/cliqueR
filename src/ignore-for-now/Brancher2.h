// John Eblen
// August 16, 2006
// A class to encapsulate the general branching algorithm for maximum clique,
// along with an interface that allows the user to build a custom branching
// strategy.

#ifndef _BRANCHER_H
#define _BRANCHER_H 1

#include <iostream>
#include "Graph2.h"
#include "Candidate_Manager2.h"
#include "Clique_Test2.h"
#include "Preprocessor2.h"
#include "Degree_Preprocessor2.h"
using namespace std;

class Brancher
{
  // Basic search strategy
  bool search_best_to_worst;

  Candidate_Manager *branch_cand_man;

  // Turn off/on degree preprocessing
  bool branch_dpp;
  bool node_dpp;

  // Interleaved Preprocessors (a null pointer means no preprocessing)
  Preprocessor *branch_ipp;
  Preprocessor *node_ipp;

  // Only for use by derived classes that have parallel implementations
  // (see below)
  protected:
  bool par_imp;  // Is this a parallel implementation?

  public:
  Brancher()
  {
    search_best_to_worst = 0;
    branch_cand_man = new Candidate_Manager;
    branch_dpp = 0;
    node_dpp = 0;
    branch_ipp = 0;
    node_ipp = 0;
    par_imp = 0;
  }

  // Basic Access Functions
  void set_worst_to_best_search() {search_best_to_worst = 0;}
  void set_best_to_worst_search() {search_best_to_worst = 1;}
  void set_cand_man(Candidate_Manager *cm)
  {
    delete branch_cand_man;
    branch_cand_man = cm;
  }
  void branch_dpp_off() {branch_dpp = 0;}
  void node_dpp_off() {node_dpp = 0;}
  void branch_dpp_on() {branch_dpp = 1;}
  void node_dpp_on() {node_dpp = 1;}
  void set_branch_ipp(Preprocessor *p) {branch_ipp = p;}
  void set_node_ipp(Preprocessor *p) {node_ipp = p;}

  // Main Operation
  int operator() (Graph &, Graph::Vertices &) const;
  int operator() (Graph::Vertices &, Graph::Vertices &) const;

  protected:
  // The core branching function
  void find_mcl(Graph::Vertices mygraph, Graph::Vertices &maximum_clique,
                                Graph::Vertices *current_clique = 0) const;

  // Hooks added Nov. 6, 2006 to allow parallel implementations by
  // derived classes.  These are called at the appropriate places
  // during branching (see source).
  enum{NO_RUN_AFTER_INIT, RUN_AFTER_INIT};
  enum{HALT_SEARCH, CONT_SEARCH};
  virtual int par_init(Graph &g, Graph::Vertices &maximum_clique) const
                                                                 {return 0;}
  virtual int par_find_mcl(Graph::Vertices &mygraph,
                           Graph::Vertices &maximum_clique,
                           Graph::Vertices *current_clique, int depth)
                                                              const {return 0;}
  virtual int par_checkin(Graph::Vertices &mygraph,
                          Graph::Vertices &maximum_clique,
                          Graph::Vertices *current_clique, int depth)
                                                    const {return CONT_SEARCH;}

  // Compare maximum clique size with size of current clique and update if
  // latter is larger.  For version 4.1, we do this simple operation frequently,
  // and hence encapsulating it is appropriate.
  private:
  void update_maximum_clique(Graph::Vertices &, Graph::Vertices &) const;
};

#endif
