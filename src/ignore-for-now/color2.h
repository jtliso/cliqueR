// John Eblen
// August 4, 2006
// A collection of functions for coloring graphs.  These are often used by
// both clique testing functions and candidate managers.
// At present, there is only one function, which does greedy coloring and
// is used by the Color_Test and color managers.  Coloring, though, is an
// important part of strategies for clique hunting, and so I suspect this
// library will grow.

#ifndef _COLORING_H
#define _COLORING_H 1

#include "Graph2.h"
using namespace std;

// This function greedily colors the given nodes in subgraph sg in REVERSE
// order (nodes should ideally be sorted lowest to highest degree when passed).
// It returns the number of colors needed (an upper bound on the clique size).
// If passed, node2color is a mapping of each node to its color, and
// sorted_nodes is an array of the nodes sorted from high to low color value.
// Warning:  See warnings in source.
int color_subgraph(const Graph::Vertices &sg, vector<int> *node2color = 0,
                                              vector<int> *sorted_nodes = 0);
#endif
