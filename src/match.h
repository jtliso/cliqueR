// John Eblen
// June 20, 2008
// A collection of functions for computing matchings.  There is a close
// relationship between matchings and cliques, allowing matching to be used for
// approximation heuristics and as a clique test.  More precisely, clique is
// associated with "anti-matching," matching of non-edges, as opposed to edges
// as done in more traditional matching.

#ifndef _MATCH_H
#define _MATCH_H 1

#include "Graph.h"
using namespace std;

// This function attempts to find a large anti-match - not necessarily maximum.
// Useful for building a Clique_Test, which can be used in preprocessing.
int find_large_anti_match(const Graph::Vertices &, vector<int> *matches = 0);
#endif
