#ifndef _PARACL_H
#define _PARACL_H 1

#include "Graph.h"

void build_paracl(Graph &g, Graph::Vertices &v, double glom);
void build_acc_paracl(Graph &g, Graph::Vertices &v, int agg_factor);

#endif
