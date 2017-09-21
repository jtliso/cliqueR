// John Eblen
// July 17, 2006
// Base class for Clique Test functional classes plus some derived classes
// that implement specific tests.

// Very simple base class - derived classes need only implement the
// procedure call ( () operator) with the appropriate parameters.

#ifndef _CLIQUE_TEST_H
#define _CLIQUE_TEST_H 1

#include <map>
#include "Graph2.h"
#include "color2.h"
using namespace std;

class Clique_Test
{
  protected:
  const bool subgraph_needed;

  public:
  Clique_Test(bool b) :subgraph_needed(b) {}
  virtual bool operator() (const Graph::Vertices &, int, int node = -1)
                                                                  const = 0;
  bool need_subgraph() const {return subgraph_needed;}
  virtual ~Clique_Test() {}
};

class Size_Test : public Clique_Test
{
  public:
  Size_Test() :Clique_Test(0) {}

  bool operator() (const Graph::Vertices &v, int min_size, int node) const
  {
    if (node != -1) return v.degree(node) < min_size - 1;
    return v.size() < min_size;
  }
};

class Edge_Test : public Clique_Test
{
  public:
  Edge_Test() :Clique_Test(1) {}

  bool operator() (const Graph::Vertices &v, int min_size, int node) const;
};

class Color_Test : public Clique_Test
{
  public:
  Color_Test() :Clique_Test(1) {}

  bool operator() (const Graph::Vertices &v, int min_size, int node) const
  {
    if (node != -1)
    {
      cerr << "Error - color test called with specific node." << endl;
      exit(EXIT_FAILURE);
    }

    return color_subgraph(v, 0, 0) < min_size;
  }
};

#endif
