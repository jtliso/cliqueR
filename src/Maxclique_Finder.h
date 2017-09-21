// John Eblen
// Main process to find a maximum clique.
// Used by clique.cpp and paracl_cp.cpp
// Input is a graph and output is a
// maximum clique from that graph.  This function was "main" before DC version
// 4.1.1.  It was changed to accommodate paraclique and other methods that find
// multiple MCs during a single run.

#ifndef _MAXCLIQUE_FINDER_H
#define _MAXCLIQUE_FINDER_H 1

#include "Graph.h"
Graph::Vertices *find_mc(Graph *g);
#endif
