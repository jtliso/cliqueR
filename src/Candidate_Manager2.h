// John Eblen
// July 17, 2006
// Base class for candidate managers plus some specific, derived managers.
// Candidate managers encapsulate an algorithm for selecting the next
// best candidate node given a graph and a list of candidates from which
// to choose.

#ifndef _CANDIDATE_MANAGER_H
#define _CANDIDATE_MANAGER_H 1

#include <algorithm>
#include <vector>
#include <list>
#include "Graph2.h"
#include "color2.h"
using namespace std;

// Basic Manager - a useful, lazy default
class Candidate_Manager
{
  protected:
  int next_cand;
  vector<int> cand_list;
  
  public:
  Candidate_Manager() :next_cand(0), cand_list(0) {}
  virtual void init(Graph::Vertices *v, int min_clique_size = 0,
                                        Graph::Vertices *clique = 0);
  virtual int next(Graph::Vertices *v, int min_clique_size = 0,
                                        Graph::Vertices *clique = 0);
  virtual Candidate_Manager *create() const {return new Candidate_Manager();}
  virtual ~Candidate_Manager() {}
};

// Manager that sorts from lowest to highest degree, but only in init
// (hence, lazy because it will normally only sort one time)
class Lazy_Low_Degree_Man : public Candidate_Manager
{
  public:
  void init(Graph::Vertices *v, int min_clique_size = 0,
                                               Graph::Vertices *clique = 0);
  Lazy_Low_Degree_Man *create() const {return new Lazy_Low_Degree_Man();}
};

// A high-degree manager that also will preprocess the graph.
class Prep_High_Degree_Man : public Candidate_Manager
{
  public:
  int next(Graph::Vertices *v, int min_clique_size = 0,
                                              Graph::Vertices *clique = 0);
  Prep_High_Degree_Man *create() const {return new Prep_High_Degree_Man();}
};

// A low-degree manager that also will preprocess the graph.
class Prep_Low_Degree_Man : public Candidate_Manager
{
  public:
  int next(Graph::Vertices *v, int min_clique_size = 0,
                                              Graph::Vertices *clique = 0);
  Prep_Low_Degree_Man *create() const {return new Prep_Low_Degree_Man();}
};

// Sort by a greedy graph coloring.  Coloring is lazy (only in init) but
// next contains a stopping criterion based on coloring.
class Lazy_High_Color_Man : public Candidate_Manager
{
  vector<int> node2color;
  vector<int> my_cand_list;
  int next_cand;

  public:
  Lazy_High_Color_Man() :node2color(0), my_cand_list(0) {next_cand = 0;}
  void init(Graph::Vertices *v, int min_clique_size = 0,
                                               Graph::Vertices *clique = 0);
  int next(Graph::Vertices *v, int min_clique_size = 0,
                                               Graph::Vertices *clique = 0);
  Lazy_High_Color_Man *create() const {return new Lazy_High_Color_Man();}
};

#endif
