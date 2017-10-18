/* API for Bit-Based Adjacency Matrix for undirected graphs */
/* Implement functions defined in graph.h */
/* Graph Theory Team, Computer Science Department */
/* University of Tennessee, Knoxville */
/* Yun Zhang, yzhang@cs.utk.edu, December 12, 2004 */


#include <string.h>
#include <R.h>

#include "graph_maximal.h"

// trie node
struct TrieNode {
    struct TrieNode *children[256];
    int isLeaf;
};

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void) {
    struct TrieNode *pNode = (struct TrieNode *)R_alloc(1, sizeof(struct TrieNode));

    if (pNode) {
        int i;
        pNode->isLeaf = -1;
        for (i = 0; i < 256; i++)
            pNode->children[i] = NULL;
    }

    return pNode;
}

void insert(struct TrieNode *root, const char *key, int id) {
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++) {
        index = key[level];
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }

    // mark last node as leaf
    pCrawl->isLeaf = id;
}

// Returns id if key presents in trie
int search(struct TrieNode *root, const char *key) {
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++) {
        index = key[level];

        if (!pCrawl->children[index])
            return -1;

        pCrawl = pCrawl->children[index];
    }

    if(pCrawl == NULL) return -1;
    else return pCrawl->isLeaf;
}


/* Malloc and initialize a graph, returns a pointer to it */
Graph *graph_make(unsigned int num_vertices)
{
  Graph *G;
  int i;
  int num_ints = bit_num_ints(num_vertices);
  
  G = (Graph *) malloc(sizeof(Graph));
  if (G == NULL) { perror("malloc"); exit(-1); }
  G->_num_vertices = num_vertices;
  G->_num_active_vertices = num_vertices;
  G->_num_edges = 0;
  G->_num_bytes = num_ints * sizeof(int);

  G->_label = (char **) malloc(num_vertices * sizeof(char *));
  if (G->_label == NULL) { perror("graph_make : malloc label"); exit(-1); }
  
  G->_neighbor = (unsigned int **) malloc(num_vertices * sizeof(unsigned int *));
  if (G->_neighbor == NULL) { perror("malloc"); exit(-1); }
  G->_neighbor[0] = (unsigned int *) malloc(G->_num_bytes * num_vertices);
  if (G->_neighbor[0] == NULL) { perror("malloc"); exit(-1); }
  for (i = 0; i < num_vertices; i++) {
    G->_neighbor[i] = G->_neighbor[0] + i * num_ints;
  }
  
  G->_active = (unsigned int *) malloc(G->_num_bytes);
  if (G->_active == NULL) { perror("malloc"); exit(-1); }
  
  G->_degree = (unsigned short *) malloc(num_vertices * sizeof(unsigned short));
  if (G->_degree == NULL) { perror("malloc"); exit(-1); }
  
  memset(G->_neighbor[0], 0, G->_num_bytes * num_vertices);
  memset(G->_active, 0xffff, G->_num_bytes);
  memset(G->_degree, 0, num_vertices * sizeof(unsigned short));
  
  return G;
}


/* Free the memory of a graph */
void graph_free(Graph *G)
{
  int i;
  if (G != NULL) {
    if (G->_neighbor) {
      if (G->_neighbor[0]) free(G->_neighbor[0]);
      free(G->_neighbor);
    }
    if (G->_active) free(G->_active);
    if (G->_degree) free(G->_degree);
	for (i=0; i<G->_num_vertices; i++) free(G->_label[i]);
    free(G);
  }
}

/** I/O functions for Graph **/

/* Read in an unweighted edge-list file, return a pointer to the graph */
Graph * UW_EdgeList_in(FILE *fp)
{
  unsigned int u, v, n, e, i;
  Graph *G;
  int f = fscanf(fp, "%d %d", &n, &e);

  if(f != 2){
    fprintf(stderr, "Bad file format : n e incorrect\n");
    exit(-1);
  }

  G = graph_make(n);
  for (i = 0; i < e; i++) {
    f = fscanf(fp, "%d", &u);
    f = fscanf(fp, "%d", &v);
    add_edge(G, u, v);
  }
  return G;
}

Graph * graph_edgelist_in(FILE *fp)
{
  unsigned int n, e;
  int u, v;
  int k=0, edges=0, r;
  char word1[100], word2[100];
  Graph *G;
  int id;

  if (fscanf(fp, "%d %d", &n, &e) != 2) {
	fprintf(stderr, "Bad file format : n e incorrect\n");
	exit(-1);
  }

  G = graph_make(n);

  /* create a trie tree to keep track of unqiue vertex labels seen */
  struct TrieNode *root = getNode();
    
  while ((r = fscanf(fp, "%s\t%s", word1, word2)) != EOF) {
	if (r != 2) {
	  fprintf(stderr, "Bad file format : label1 label2 incorrect\n");
	  exit(-1);
	}

	if ((id = search(root, word1)) != -1) {
	  u = id;
	}
	else {
	  u = k;
	  G->_label[k++] = strdup(word1);
	  insert(root, word1, u);
	}
	
	if ((id = search(root, word2)) != -1) {
	  v = id;
	}
	else {
	  v = k;
	  G->_label[k++] = strdup(word2);
	  insert(root, word2, v);
	}
	
	if (k > n) {
	  fprintf(stderr, "Bad file format : too many labels\n");
	  exit(-1);
	}
	
	add_edge(G, u, v);
	edges++;

//	printf("%s (%d)\t%s (%d)\n", word1, u, word2, v);

  }
  
  if (edges != e) { 
	fprintf(stderr, "edgelist_in : # of edges incorrect\n");
	fprintf(stderr, "edgelist_in : %d vertices, %d edges\n", k, edges);
  }
  if (k != n) {
	fprintf(stderr, "edgelist_in : # of vertices incorrect\n");
	fprintf(stderr, "edgelist_in : %d vertices, %d edges\n", k, edges);
	G->_num_vertices = k;
	G->_num_active_vertices = k;
  }
  
  return G;
}


/* Write out a graph as unweighted edge-list to a file pointer */
void UW_EdgeList_out(FILE *fp, Graph *G)
{
  unsigned int i, j, n;
  fprintf(fp, "%d %d\n", num_active_vertex(G), num_edges(G));
  n = num_vertices(G);
  for (i = 0; i < n; i++)
    for (j = i + 1; j < n; j++)
      if (edge_exists(G, i, j))
        fprintf(fp, "%s\t%s\n", G->_label[i], G->_label[j]);
}

/* Write out a graph as an adjacency matrix to a file pointer */
void AdjMatrix_out(FILE *fp, Graph *G)
{
  unsigned int i, j, n;
  n = num_vertices(G);
  fprintf(fp, "%d %d %d\n", n, num_active_vertex(G), num_edges(G));
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++)
      if (edge_exists(G, i, j))
        fprintf(fp, "1 ");
      else fprintf(fp, "0 ");
    fprintf(fp, "\n");
  }
}

/* Write out the degree of each vertex in a graph to a file pointer */
void DegreeList_out(FILE *fp, Graph *G)
{
  unsigned int i, n;
  n = num_vertices(G);
  fprintf(fp, "DegreeList %d %d\n", n, num_active_vertex(G));
  for (i = 0; i < n; i++)
    if (vertex_exists(G, i))
      fprintf(fp, "%s\t%d\n", G->_label[i], degree(G, i));
  return;
}
 

/** Functions to returns a vertex of certain degree **/

/* Returns the highest degree */
unsigned short highest_degree(Graph *G)
{
  unsigned int n, i;
  unsigned int h = 0; 
  n = num_vertices(G); 
  for (i = 0; i < n; i++)
    if (vertex_exists(G, i))
      if (degree(G, i) > h) 
        h = degree(G, i);
  return h;
}


/* Returns the lowest degree */
unsigned short lowest_degree(Graph *G)
{
  unsigned int n, i;
  unsigned int l = 65535; 
  n = num_vertices(G); 
  for (i = 0; i < n; i++)
    if (vertex_exists(G, i))
      if (degree(G, i) < l) 
        l = degree(G, i);
  return l;
}

/* Returns one of the vertices with highest degree */
unsigned int highest_degree_vertex(Graph *G)
{
  unsigned int n, i;
  unsigned int h = 0, highest; 
  n = num_vertices(G); 
  highest = n;
  for (i = 0; i < n; i++)
    if (vertex_exists(G, i))
      if (degree(G, i) > h) {
        h = degree(G, i);
		highest = i;
      }
  return highest;
}

/* Returns one of the vertices with lowest degree */
unsigned int lowest_degree_vertex(Graph *G)
{
  unsigned int n, i;
  unsigned int l = 65535, lowest;; 
  n = num_vertices(G); 
  lowest = n;
  for (i = 0; i < n; i++)
    if (vertex_exists(G, i))
      if (degree(G, i) < l) {
        l = degree(G, i);
		lowest = i;
      }
  return lowest;
}

/* Returns one of the vertices with degree = k */
int equal_degree_vertex(Graph *G, unsigned short k)
{
  unsigned int n, i;
  n = num_vertices(G); 
  for (i = 0; i < n; i++)
    if (vertex_exists(G, i))
      if (degree(G, i) == k)
        return i;
  return -1;
}

/* Returns one of the vertices with degree > k */
int higher_degree_vertex(Graph *G, unsigned short k)
{
  unsigned int n, i;
  n = num_vertices(G); 
  for (i = 0; i < n; i++)
    if (vertex_exists(G, i))
      if (degree(G, i) > k)
        return i;
  return -1;
}

/* Returns one of the vertices with degree < k */
int lower_degree_vertex(Graph *G, unsigned short k)
{
  unsigned int n, i;
  n = num_vertices(G); 
  for (i = 0; i < n; i++)
    if (vertex_exists(G, i))
      if (degree(G, i) < k)
        return i;
  return -1;
}


