// John Eblen
// February 25, 2004
// Graph library

#ifndef _GRAPH_H
#define _GRAPH_H 1

#include <Rcpp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include "Bitarray.h"
using namespace std;

class Graph
{
  // User View
  int num_nodes;
  int num_edges;
  vector<int> node_order;  // Order in which nodes should be traversed

  // System View
  int max_nodes;  // True capacity (>= num_nodes)

  bitarray *edges;  //  Bit matrix of edges (max_nodes x max_nodes)
  bitarray used_nodes;  // The num_nodes nodes currently being used

  vector<string> nodes;  // Node number to name mapping
  map<string, int> node2num;  // Node name to number mapping


  public:
  class Vertices;

  // Constructors and constructor-related methods
  Graph(int init_size, int max_size = 0);
  Graph(string &graph_file, int max_size = 0);
  Graph(const Graph &);
  Graph &operator=(const Graph &);

  private:
  void init(int init_size, int max_size = 0);
  public:
  void clear();
  Vertices *new_vertices(int num = 1);

  // Destructor
  ~Graph() {delete [] edges;} 

  // Graph building methods
  int create_new_node();
  int create_new_node(const string &);
  void remove(int);
  void remove(const Vertices &);
  void remove_all_except(const Vertices &);
  void connect(int, int);
  void disconnect(int, int);

  // Accessor methods
  int size() const {return num_nodes;}
  int nsize() const {return num_nodes;}
  int esize() const {return num_edges;}
  int max_size() const {return max_nodes;}
  bool connected(int n1, int n2) const {return edges[n1][n2];}
  string label(int n) const {return nodes[n];}
  int vertex(const string &n) const
  {
    map<string, int>::const_iterator i = node2num.find(n);
    if (i == node2num.end()) return -1;
    return i->second;
  }

  // Simple access to ordered nodes only
  // Added July 24, 2007 to bypass the slow Vex_ptr mechanism of traversal.
  // Warning:  May not be up-to-date if recent modifications have been made
  // to the graph.  To be safe, sort nodes first and then call this function.

  // July 27, 2007 - split into two functions, one for vector and another for
  // list.
  void sorted_nodes_vector(vector<int> &v) const
  {
    v.resize(node_order.size());
    v.assign(node_order.begin(), node_order.end());
  }

  void sorted_nodes_list(list<int> &l) const
  {
    l.clear();
    l.insert(l.end(), node_order.begin(), node_order.end());
  }

  // Degree counting functions
  int degree(int n) const {return ((edges[n]).count());}
  int ldvertex() const;
  int hdvertex() const;

  // Common graph operations
  void complement();
  int cmn_nbrs(int n1, int n2) const {return (edges[n1] & edges[n2]).count();}
  bool is_clique() const;

  // Sorting facilities - incorporated in version 2.0
  void sort_by_degree_asc();
  void sort_by_degree_des();

  // Shrinking function, added July 27, 2006, to extend the usefulness of
  // a Graph.  After preprocessing or some other operation that removes
  // several nodes, future operations on the same graph are highly inefficient
  // due to processing extra unused bits.  This function condenses the graph
  // representation to restore the efficiency and extend the useful life of
  // a graph.  Only the internal representation changes, though, and the user
  // only sees a reduction in the graph's maximum capacity (max_nodes).
  // NOTE:  This operation is VERY expensive and should only be called after
  // a dramatic reduction in the size of the original graph and before
  // expensive future operations are performed (such as branching after
  // preprocessing).
  void shrink(int maxn = 0);

  // Nested pointer class for easy traversal in "node_order" order.
  class Vex_ptr
  {
    int i;
    const Graph *g;

    public:
    // Constructor
    Vex_ptr(const Graph &G)
    {
      g = &G;
      goto_first();
    }

    // Basic Operations
    Vex_ptr &operator++()
    {
      for (i=i+1; i<(int)g->node_order.size(); i++)
                             if (g->used_nodes[g->node_order[i]]) break;
      return *this;
    }

    Vex_ptr &operator--()
    {
      for (i=i-1; i>=0; i--) if (g->used_nodes[g->node_order[i]]) break;
      return *this;
    }

    int operator*() const {return g->node_order[i];}

    // Handy functions
    void goto_first() {i=-1; ++(*this);}
    void goto_last() {i=g->node_order.size(); --(*this);}
    bool end() const {return i >= (int)g->node_order.size();}
    bool begin() const {return i < 0;}

    friend class Graph;
  };

  // Nested vertices class to allow users to easily store and alter sets of
  // vertices.  Functions are also provided to analyze the corresponding
  // induced subgraph.  However, the user should remember that these are
  // simply sets of vertices and not full-fledged graphs.  The "edges" of
  // the "subgraph" reflect those of the actual graph itself.
  // NOTE:  Minimal error-checking as these operations are meant for heavy
  // use to take advantage of efficient bit arithmetic.
  class Vertices
  {
    Graph *g;
    bitarray v;

    private:
    // Parameterless constructor to allow graph class to create
    // arrays of Vertices.
    Vertices()
    :g(NULL),
     v()
    {}

    public:
    // Constructor - creates an empty set of vertices in graph G
    Vertices(Graph &G)
    :g(&G),
    v(G.max_size())
    {}
    Vertices(const Vertices &other_v)
    :g(other_v.g),
    v(other_v.v)
    {}

    // Note:  Default copy, assignment, and destructor are fine for this
    // class and all other classes besides Graph.

    // Vertex set methods
    void clear() {v.reset();}
    void fill() {v.reset(); v ^= g->used_nodes;}
    void complement() {v ^= g->used_nodes;}

    void insert(int n) {v.set(n);}
    void remove(int n) {v.reset(n);}

    void insert(const Vertices &v2) {v |= v2.v;}
    void remove(const Vertices &v2) {v ^= (v & v2.v);}

    void insert_nbrs(int n) {v |= g->edges[n];}
    void remove_nbrs(int n) {v ^= (v & g->edges[n]);}

    void insert_nnbrs(int n) {v |= (~(g->edges[n]));}
    void remove_nnbrs(int n) {v &= g->edges[n];}

    bool in_set(int n) const {return v[n];}
    int size() const {return v.count();}
    int esize() const;
    Graph *graph() const {return g;}
    void node_array(vector<int> &) const;
    void node_list(list<int> &) const;
    int degree(int n) const {return (g->edges[n] & v).count();}
    int ldvertex(int min = 0) const;
    int hdvertex() const;
    int cmn_nbrs(int n1, int n2) const
                         {return ((g->edges[n1] & g->edges[n2]) & v).count();}
    bool is_clique() const;
    int overlap(const Vertices &v2) const {return (v & v2.v).count();}

    // July 30, 2007 - Equivalent to functions with same name in Graph, except
    // exclude vertices not in this particular vertex set.
    void sorted_nodes_vector(vector<int> &vec) const
    {
      int i;
      int next_node = 0;

      vec.resize(v.count());
      for (i=0; i<(int)g->node_order.size(); ++i)
      {
        if (v[g->node_order[i]])
        {
          vec[next_node] = g->node_order[i];
          ++next_node;
        }
      }
    }

    void sorted_nodes_list(list<int> &l) const
    {
      int i;
      l.clear();

      for (i=0; i<(int)g->node_order.size(); ++i)
      {
        if (v[g->node_order[i]]) l.push_back(g->node_order[i]);
      }
    }

    // Vertex set packing.  Pack the set as a tangible unsigned int
    // array that can be sent to other processes (for parallel computing)
    int pack_size() const;
    void pack(unsigned *) const;
    void unpack(const unsigned *);

    friend class Graph;

    // Nested pointer class for easy traversal.  Note that this is
    // implemented using Graph::Vex_ptr, so traversal is identical to that
    // of the graph itself except that nodes not in Vertices set are skipped.
    class Vex_ptr
    {
      Graph::Vex_ptr graph_ptr;
      const Graph::Vertices *v;

      public:
      // Constructor and set function often called after construction
      Vex_ptr(const Graph::Vertices &V)
      :graph_ptr(*(V.g)),
       v(&V)
      {goto_first();}

      void set_ptr(const Graph::Vertices::Vex_ptr &vp)
                                                  {graph_ptr = vp.graph_ptr;}
       
      // Basic Operations
      Vex_ptr &operator++()
      {
        ++graph_ptr;
        while (!graph_ptr.end())
        {
          if (v->v[*graph_ptr]) break;
          ++graph_ptr;
        }
        return *this;
      }

      Vex_ptr &operator--()
      {
        --graph_ptr;
        while (!graph_ptr.begin())
        {
          if (v->v[*graph_ptr]) break;
          --graph_ptr;
        }
        return *this;
      }

      int operator*() const {return *graph_ptr;}

      // Handy functions
      void goto_first()
      {
        graph_ptr.goto_first();
        if (!graph_ptr.end() && !v->v[*graph_ptr]) ++(*this);
      }
      void goto_last()
      {
        graph_ptr.goto_last();
        if (!graph_ptr.begin() && !v->v[*graph_ptr]) --(*this);
      }
      bool end() const {return graph_ptr.end();}
      bool begin() const {return graph_ptr.begin();}

      friend class Graph::Vertices;
    };
  };
};

// Non-member helper functions
void read_file(Graph &, const string &);

// Binary predicates for sorting by degree
class Graph_deg_sort_asc
{
  const Graph *g;

  public:
    Graph_deg_sort_asc(const Graph &gr) {g = &gr;}
    bool operator() (int n1, int n2) { return g->degree(n1) < g->degree(n2);}
};

class Graph_deg_sort_des
{
  const Graph *g;

  public:
    Graph_deg_sort_des(const Graph &gr) {g = &gr;}
    bool operator() (int n1, int n2) {return g->degree(n1) > g->degree(n2);}
};

class Graph_vertices_deg_sort_asc
{
  const Graph::Vertices *v;

  public:
    Graph_vertices_deg_sort_asc(const Graph::Vertices &vp) {v = &vp;}
    bool operator() (int n1, int n2) { return v->degree(n1) < v->degree(n2);}
};

class Graph_vertices_deg_sort_des
{
  const Graph::Vertices *v;

  public:
    Graph_vertices_deg_sort_des(const Graph::Vertices &vp) {v = &vp;}
    bool operator() (int n1, int n2) { return v->degree(n1) > v->degree(n2);}
};

// Handy routines to sort by mapped values.  Useful for nodes of a graph
// but generally useful anytime you need to sort integers by mapped values.
class map_sort_asc
{
  map<int, int> *themap;

  public:
    map_sort_asc(map<int, int> &m) {themap = &m;}
    bool operator() (int i, int j) {return ((*themap)[i] < (*themap)[j]);}
};

class map_sort_des
{
  map<int, int> *themap;

  public:
    map_sort_des(map<int, int> &m) {themap = &m;}
    bool operator() (int i, int j) {return ((*themap)[i] > (*themap)[j]);}
};

// Equivalent routines but with vectors
class vector_sort_asc
{
  vector<int> *themap;

  public:
    vector_sort_asc(vector<int> &m) {themap = &m;}
    bool operator() (int i, int j) {return ((*themap)[i] < (*themap)[j]);}
};

class vector_sort_des
{
  vector<int> *themap;

  public:
    vector_sort_des(vector<int> &m) {themap = &m;}
    bool operator() (int i, int j) {return ((*themap)[i] > (*themap)[j]);}
};

// Handy non-member accessor functions for Graph::Vertices sets.  Retrieve a
// list of nodes by name and also insert a list of nodes by name.  Normally,
// this should be done using the member functions (and node index numbers)
// for efficiency.
void get_node_names(const Graph::Vertices &, vector<string> &);
void insert_nodes_by_name(Graph::Vertices &, const vector<string> &);

// Printing Facilities
void print_graph(Graph &);
Rcpp::StringVector print_vertices(Graph::Vertices &);
void print_vertices_oneline(Graph::Vertices &);
#endif
