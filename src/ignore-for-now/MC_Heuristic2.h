// John Eblen
// July 28, 2006
// Base class for heuristic algorithms for maximum clique plus some specific,
// derived algorithms.  This class encapsulates an algorithm for quickly
// computing a lower bound on the maximum clique size.  These algorithms
// often are the first step of a larger algorithm to solve the exact maximum
// clique problem.  For example, the lower bound can serve as a good minimum
// clique size parameter for preprocessing.  Effective algorithms will vary
// for different types of graphs, but, theoretically, there are no guarantees.

#ifndef _MC_HEURISTIC_H
#define _MC_HEURISTIC_H 1

#include "Graph2.h"
using namespace std;

class MC_Heuristic
{
  public:
  // First three operators simply provide the user with a flexible
  // interface.  The heuristic is actually implemented in the fourth
  // operation.
  int operator() (Graph &g) const
  {
    Graph::Vertices subgraph(g);
    Graph::Vertices clique(g);
    subgraph.fill();
    clique.clear();
    return operator() (subgraph, clique);
  }

  int operator() (Graph &g, Graph::Vertices &clique) const
  {
    Graph::Vertices subgraph(g);
    subgraph.fill();
    clique.clear();
    return operator() (subgraph, clique);
  }
  
  int operator() (Graph::Vertices &subgraph) const
  {
    Graph *base_graph = subgraph.graph();
    Graph::Vertices clique(*base_graph);
    clique.clear();
    return operator() (subgraph, clique);
  }

  virtual int operator() (Graph::Vertices &, Graph::Vertices &) const = 0;
  virtual ~MC_Heuristic() {}
};

class Greedy_Clique : public MC_Heuristic
{
  public:
  using MC_Heuristic::operator();
  int operator() (Graph::Vertices &, Graph::Vertices &) const;
};

#endif
