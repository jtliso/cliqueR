/* API for Bit-Based Adjacency Matrix for undirected graphs */
/* Graph Theory Team, Computer Science Department */ 
/* University of Tennessee, Knoxville */
/* Yun Zhang, yzhang@cs.utk.edu, December 12, 2004 */

#ifndef __GRAPH_MAXIMAL_H
#define __GRAPH_MAXIMAL_H

#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

typedef unsigned short vid_t;

/** Data structure **/

/* data structure of a graph */
typedef struct graph_t {
  unsigned int _num_vertices;  /* total number of vertices in graph */
  unsigned int _num_active_vertices; /* number of activer vertices */
  unsigned int _num_edges;     /* number of active edges */
  unsigned int _num_bytes;     /* number of bytes in bit-based vector */
  unsigned int **_neighbor;      /* bit-based adjacency matrix */
  unsigned int *_active;       /* bit-based vector: 1-active */
  unsigned short *_degree;     /* number of edges each vertex have (<65536) */
  char **_label;               /* labels of vertices */
} Graph;


/* Macros for Graph */

/* returns the total number of vertices in graph */
#define num_vertices(g)  (g->_num_vertices)

/* returns the number of active vertices in graph */
#define num_active_vertex(g)  (g->_num_active_vertices)

/* returns the number of active edges in graph */
#define num_edges(g)  (g->_num_edges)

/* check to see if an edge exists or not */
#define edge_exists(g, u, v)  (IS_SET(g->_neighbor[u], v))

/* check to see if a vertex is active or not */
#define vertex_exists(g, u)  (IS_SET(g->_active, u))

/* return the degree of a vertex */
#define degree(g, u)  (g->_degree[u])

/* return the pointer to the bit-based neighbors of vertex u */
#define neighbors(g, u)  (g->_neighbor[u])

/* add an edge to graph */
#define add_edge(g, u, v)  { \
  if (!IS_SET(g->_neighbor[u], v)) {\
    SET_BIT(g->_neighbor[u], v); SET_BIT(g->_neighbor[v], u); \
    g->_num_edges++; \
    g->_degree[u]++; g->_degree[v]++; }\
}	

/* delete an edge from graph */
#define delete_edge(g, u, v) { \
  if (IS_SET(g->_neighbor[u], v)) {\
    DEL_BIT(g->_neighbor[u], v); DEL_BIT(g->_neighbor[v], u); \
    g->_degree[u]--; g->_degree[v]--; \
    g->_num_edges--; } \
}

/* delete a vertex, making it inactive */
#define delete_vertex(g, u) { \
  int my_i; \
  if (IS_SET(g->_active, u)) { \
    g->_num_active_vertices--; \
    for (my_i = 0; my_i < num_vertices(g); my_i++) {\
      if (IS_SET(g->_neighbor[u], my_i)) { delete_edge(g, u, my_i); } \
    }\
    DEL_BIT(g->_active, u);\
  }\
}

/* iteratively deletes all vertices with degree < k */
#define delete_lower_degree_vertices(g, k) {\
  int my_j, my_deleted=1; \
  while (my_deleted) { \
    my_deleted = 0; \
    for (my_j = 0; my_j < num_vertices(g); my_j++) \
      if (vertex_exists(g, my_j)) \
        if (degree(g, my_j) < k) { \
          delete_vertex(g, my_j); my_deleted++; \
        } \
  }\
}


/** Function prototypes **/

/* Malloc a graph, initialize it and returns a pointer to it */
Graph *graph_make(unsigned int num_vertices);

/* Free the memory of a graph */
void graph_free(Graph *G);

/* Read in a graph from an unweighted edge-list format file */
Graph *UW_EdgeList_in(FILE *fp);
Graph *graph_edgelist_in(FILE *fp);

/* Write out a graph to an unweighted edge-list format file */
void UW_EdgeList_out(FILE *fp, Graph *G);

/* Write out a graph to an adjacency matrix format file */
void AdjMatrix_out(FILE *fp, Graph *G);

/* Write out the degree of each vertex in a graph to a file pointer */
void DegreeList_out(FILE *fp, Graph *G);

/* returns the highest degree */
unsigned short highest_degree(Graph *G); 

/* returns the lowest degree */
unsigned short lowest_degree(Graph *G);

/* returns one of the vertices with highest degree */
unsigned int highest_degree_vertex(Graph *G);

/* returns one of the vertices with lowest degree */
unsigned int lowest_degree_vertex(Graph *G);

/* returns one of the vertices with degree equal to k */
int equal_degree_vertex(Graph *G, unsigned short k);

/* returns one of the vertices with degree higher than k */
int higher_degree_vertex(Graph *G, unsigned short k);

/* returns one of the vertices with degree lower than k */
int lower_degree_vertex(Graph *G, unsigned short k);


#endif  /* __GRAPH_H */

