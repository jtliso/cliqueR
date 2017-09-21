#include "Graph2.h"

// Construct graph without a file.  Create an edgeless graph with init_size
// nodes and capacity max_size.
Graph::Graph(int init_size, int max_size) {init(init_size, max_size);}

// Constructor for a graph that is read from a file.  By default, maximum size
// will be the size of the input graph, but the user can override this with the
// second argument (which must be >= to the size of the input graph).
Graph::Graph(string &graph_file, int max_size)
{
  // Read maximum number of nodes from file, if not passed by the user
  if (max_size < 1)
  {
    ifstream input(graph_file.c_str());
    if (!input)
    {
      cerr << "Error - Unable to open file " << graph_file << endl;
      exit(EXIT_FAILURE);
    }
    input >> max_size;
    if (max_size < 1)
    {
      cerr << "Error - invalid value for number of nodes on first line of file " << graph_file << endl;
      input.close();
      exit(EXIT_FAILURE);
    }
    input.close();
  }

  init(0, max_size);
  read_file(*this, graph_file);
}

// Copy Constructor
Graph::Graph(const Graph &g)
:num_nodes(g.num_nodes),
 num_edges(g.num_edges),
 node_order(g.node_order),
 max_nodes(g.max_nodes),
 edges(new bitarray[g.max_nodes]),
 used_nodes(g.used_nodes),
 nodes(g.nodes),
 node2num(g.node2num)
{
  int i;
  for (i=0; i<max_nodes; i++) edges[i] = g.edges[i];
}

// Assignment Operator
Graph &Graph::operator=(const Graph &g)
{
  int i;

  num_nodes = g.num_nodes;
  num_edges = g.num_edges;
  node_order = g.node_order;
  max_nodes = g.max_nodes;
  used_nodes = g.used_nodes;
  nodes = g.nodes;
  node2num = g.node2num;

  delete [] edges;
  edges = new bitarray[g.max_nodes];
  for (i=0; i<max_nodes; i++) edges[i] = g.edges[i];

  return *this;
}

// Function that really constructs the graph
void Graph::init(int init_size, int max_size)
{
  int i;

  if (max_size == 0) max_size = init_size;
  if (init_size < 0 || max_size < max(1,init_size))
  {
    cerr << "Graph initialized with illegal values." << endl;
    exit(EXIT_FAILURE);
  }

  num_nodes = init_size;
  num_edges = 0;
  for (i=0; i<init_size; i++) node_order.push_back(i);

  max_nodes = max_size;

  edges = new bitarray[max_size];
  for (i=0; i<max_size; i++) edges[i].create(max_size);
  used_nodes.create(max_size);
  for (i=0; i<init_size; i++) used_nodes.set(i);

  nodes.resize(max_size);
  node2num.clear();
}

// Method to read a DIMACS file into a graph instance.  The graph instance
// must be big enough to support it.
// Note:  Any previous graph data is ERASED!
void read_file(Graph &g, const string &graph_file)
{
  string node1, node2;

  ifstream input(graph_file.c_str());
  if (!input)
  {
    cerr << "Error - Unable to open file " << graph_file << endl;
    exit(EXIT_FAILURE);
  }

  g.clear();

  // MAIN LOOP
  input >> node1 >> node2;  // Skip header
  input >> node1 >> node2;
  while(input)
  {
    g.connect(g.create_new_node(node1), g.create_new_node(node2));

    input >> node1 >> node2;
  }

  input.close();
}

// This function resets the graph to a clean, pristine state
void Graph::clear()
{
  int i;

  num_nodes = 0;
  num_edges = 0;
  node_order.resize(0);

  for (i=0; i<max_nodes; i++) edges[i].reset();
  used_nodes.reset();

  node2num.clear();
}


// This function returns an array of Vertices class objects
Graph::Vertices *Graph::new_vertices(int num)
{
  int i;
  Graph::Vertices *v;

  if (num < 1)
  {
    cerr << "Error - invalid parameter passed to Graph::new_vertices." << endl;
    exit(EXIT_FAILURE);
  }

  v = new Vertices[num];
  for (i=0; i<num; i++) {v[i].g = this; v[i].v.create(max_nodes);}

  return v;
}

// Functions to insert a new node.  Only at most max_nodes nodes are allowed.
// Note that this only brings the node into existence.  You must use connect
// to then connect it to other nodes.
// This routine currently uses an inefficient linear search to find the next
// available slot.
int Graph::create_new_node() {return create_new_node("");}
int Graph::create_new_node(const string &name)
{
  int i;

  if (node2num.find(name) != node2num.end()) return node2num[name];
  if (num_nodes >= max_nodes)
  {
    cerr << "Error - cannot create new node in Graph - capacity exceeded." << endl;
    exit(EXIT_FAILURE);
  }

  // Find available slot to store node
  for (i=0; i<max_nodes; i++) {if (!used_nodes[i]) break;}
  if (i >= max_nodes)
  {
    cerr << "Internal Graph Error - unable to find available node in Graph::create_new_node" << endl;
    exit(EXIT_FAILURE);
  }

  // Do some bookkeeping
  num_nodes++;
  node_order.push_back(i);
  used_nodes.set(i);
  nodes[i] = name;
  if (name != "") node2num[name] = i;

  return i;
}

void Graph::remove(int num)
{
  int i;

  if (num < 0 || num >= max_nodes || !used_nodes[num]) return;

  num_nodes--;
  // Remove node's edges
  for (i=0; i<max_nodes; i++)
  {
    if (edges[num][i])
    {
      edges[i].reset(num);
      num_edges--;
    }
  }
  (edges[num]).reset();

  // Do some more bookkeeping
  used_nodes.reset(num);
  node2num.erase(nodes[num]);
}

void Graph::remove(const Vertices &v)
{
  int i;

  for (i=0; i<max_nodes; i++) if (v.v[i]) remove(i);
}

void Graph::remove_all_except(const Vertices &v)
{
  int i;

  for (i=0; i<max_nodes; i++) if (!v.v[i]) remove(i);
}

// Functions to add/remove edges.  Note the lack of bounds checking and
// for connect the lack of unused_node checking.  This is a tradeoff of
// efficiency versus safety.
// Note:  However, this class DOES maintain one invariant, edge[x][x] = 0.
// This assumption is used throughout the library, and it is quite easy for
// users to accidentally try and set this bit.
void Graph::connect(int num1, int num2)
{
  if (num1 == num2) return;

  if (!edges[num1][num2])
  {
    edges[num1].set(num2);
    edges[num2].set(num1);
    num_edges++;
  }
}

void Graph::disconnect(int num1, int num2)
{
  if (edges[num1][num2])
  {
    edges[num1].reset(num2);
    edges[num2].reset(num1);
    num_edges--;
  }
}

int Graph::ldvertex() const
{
  int i, n = -1;
  int deg = num_nodes, deg2;

  for (i=0; i<max_nodes; i++)
  {
    if (used_nodes[i])
    {
      deg2 = degree(i);
      if (deg2 <= deg) {deg = deg2; n = i;}
    }
  }

  return n;
}

int Graph::hdvertex() const
{
  int i, n = -1;
  int deg = -1, deg2;

  for (i=0; i<max_nodes; i++)
  {
    if (used_nodes[i])
    {
      deg2 = degree(i);
      if (deg2 >= deg) {deg = deg2; n = i;}
    }
  }

  return n;
}

int Graph::Vertices::ldvertex(int min) const
{
  int i, n = -1;
  int deg = g->num_nodes, deg2;

  for (i=0; i<g->max_nodes; i++)
  {
    if (v[i])
    {
      deg2 = degree(i);
      if (deg2 <= deg) {deg = deg2; n = i;}
      if (deg <= min && n > -1) break;
    }
  }

  return n;
}

int Graph::Vertices::hdvertex() const
{
  int i, n = -1;
  int deg = -1, deg2;

  for (i=0; i<g->max_nodes; i++)
  {
    if (v[i])
    {
      deg2 = degree(i);
      if (deg2 >= deg) {deg = deg2; n = i;}
    }
  }

  return n;
}

int Graph::Vertices::esize() const
{
  int i,j;
  int edge_sum = 0;

  for (i=0; i<g->max_nodes; i++)
  {
    if (!v[i]) continue;
    for (j=i+1; j<g->max_nodes; j++)
    {
      if (v[j] && g->edges[i][j]) edge_sum++;
    }
  }

  return edge_sum;
}

void Graph::complement()
{
  int i;

  for (i=0; i<max_nodes; i++)
  {
    if (!used_nodes[i]) continue;
    edges[i] ^= used_nodes;
    edges[i].reset(i);
  }
  num_edges = (num_nodes * (num_nodes - 1))/2 - num_edges;
}

bool Graph::is_clique() const
{
  if (num_edges == (num_nodes * (num_nodes - 1))/2) return 1;
  return 0;
}

// Sorting facility.  Reinitialize node_order to have all and only valid
// nodes, and then call sort with appropriate function object.
// NOTE:  Would like to make this more general, so that the user can
// provide their own function or function object for sorting.  How can we
// do this?
void Graph::sort_by_degree_asc()
{
  int i;

  node_order.resize(0);
  for (i=0; i<max_nodes; i++) if (used_nodes[i]) node_order.push_back(i);

  std::sort(node_order.begin(), node_order.end(), Graph_deg_sort_asc(*this));
}

void Graph::sort_by_degree_des()
{
  int i;

  node_order.resize(0);
  for (i=0; i<max_nodes; i++) if (used_nodes[i]) node_order.push_back(i);

  std::sort(node_order.begin(), node_order.end(), Graph_deg_sort_des(*this));
}

void Graph::shrink(int maxn)
{
  int i;
  int old1, new1, old2, new2;
  int new_max_nodes;
  bitarray new_used_nodes;
  bitarray *new_edges;
  vector<int> new_node_order;
  map<int, int> old2new;
  map<int, int>::iterator it, jt;

  new_max_nodes = max(1, num_nodes);
  if (maxn > new_max_nodes) new_max_nodes = maxn;

  // Allocate space to hold new graph data
  new_edges = new bitarray[new_max_nodes];
  for (i=0; i<new_max_nodes; ++i) new_edges[i].create(new_max_nodes);
  new_used_nodes.create(new_max_nodes);

  // Create mapping of old node numbers to new node numbers
  int next_id = 0;
  for (i=0; i<max_nodes; ++i) if (used_nodes[i]) old2new[i] = next_id++;
  if (old2new.size() != num_nodes)
  {
    cerr << "Error - internal consistency error in Graph::shrink()" << endl;
    exit(EXIT_FAILURE);
  }

  // Create new graph data and rename nodes
  // NOTE:  Certain commands depend on the fact that both key and
  // value are traversed in numerical order (commands marked)
  for (it = old2new.begin(); it != old2new.end(); ++it)
  {
    old1 = it->first;
    new1 = it->second;

    new_used_nodes.set(new1);
    nodes[new1] = nodes[old1];  // Depends on numerical order
    if (nodes[new1] != "") node2num[nodes[new1]] = new1;

    jt = it;
    ++jt;
    for (; jt != old2new.end(); ++jt)
    {
      old2 = jt->first;
      new2 = jt->second;
      if (edges[old1][old2])
      {
        new_edges[new1].set(new2);
        new_edges[new2].set(new1);
      }
    }
  }

  // Fix node_order to use new node numbers
  for (i=0; i<node_order.size(); ++i)
  {
    if (used_nodes[node_order[i]])
    {
      new_node_order.push_back(old2new[node_order[i]]);
    }
  }

  // Overwrite old graph data
  node_order = new_node_order;
  max_nodes = new_max_nodes;
  delete [] edges;
  edges = new_edges;
  used_nodes = new_used_nodes;
  nodes.resize(new_max_nodes);
}

void Graph::Vertices::node_array(vector<int> &dest) const
{
  int i;
  dest.clear();
  Vex_ptr vp(*this);

  for (; !vp.end(); ++vp) dest.push_back(*vp);
}

void Graph::Vertices::node_list(list<int> &dest) const
{
  int i;
  dest.clear();
  Vex_ptr vp(*this);

  for (; !vp.end(); ++vp) dest.push_back(*vp);
}

bool Graph::Vertices::is_clique() const
{
  int i;
  int csize = v.count();

  for (i=0; i<g->max_nodes; i++)
  {
    if (v[i])
    {
      if ((v & g->edges[i]).count() < csize - 1) return 0;
    }
  }

  return 1;
}

int Graph::Vertices::pack_size() const
{
  int size;

  size = g->max_nodes / (8*sizeof(unsigned));
  if (g->max_nodes % (8*sizeof(unsigned)) != 0) size++;

  return size;
}

void Graph::Vertices::pack(unsigned *packet) const
{
  int i,j;
  int bit_num = 0;
  int array_size = pack_size();

  for (i=0; i<array_size; i++)
  {
    packet[i] = 0;
    for (j=0; j<8*sizeof(unsigned); j++)
    {
      packet[i] <<= 1;
      if (v[bit_num]) packet[i]++;

      bit_num++;
      if (bit_num >= g->max_nodes)
      {
        packet[i] <<= (8*sizeof(unsigned) - j - 1);
        return;
      }
    }
  }

  cerr << "Internal graph library error in Graph::vpack." << endl;
  exit(EXIT_FAILURE);
}

void Graph::Vertices::unpack(const unsigned *packet)
{
  int i,j;
  int array_size = pack_size();
  int bit_num = 0;
  unsigned bit_field;
  unsigned first_bit_tester = 1;

  for (i=1; i<8*sizeof(unsigned); i++) first_bit_tester <<= 1;
  clear();
  for (i=0; i<array_size; i++)
  {
    bit_field = packet[i];
    for (j=0; j<8*sizeof(unsigned); j++)
    {
      if ((bit_field & first_bit_tester) > 0) v.set(bit_num);
      bit_field <<= 1;

      bit_num++;
      if (bit_num >= g->max_nodes) return;
    }
  }

  cerr << "Internal graph library error in Graph::vunpack." << endl;
  exit(EXIT_FAILURE);
}

void get_node_names(const Graph::Vertices &v, vector<string> &names)
{
  Graph *g = v.graph();
  Graph::Vertices::Vex_ptr vp(v);
  names.resize(0);

  for (; !vp.end(); ++vp) names.push_back(g->label(*vp));
}

void insert_nodes_by_name(Graph::Vertices &v, const vector<string> &names)
{
  int i;
  Graph *g = v.graph();

  for (i=0; i<names.size(); ++i) v.insert(g->vertex(names[i]));
}

// Printing facilities
void print_graph(Graph &g)
{
  Graph::Vex_ptr p(g);
  Graph::Vex_ptr q(g);

  cout << g.size() << " " << g.esize() << endl;
  while (!p.end())
  {
    q=p;
    ++q;
    while (!q.end())
    {
      if (g.connected(*p, *q)) cout << g.label(*p) << " " << g.label(*q)
                                                                     << endl;
      ++q;
    }

    ++p;
  }
}

void print_vertices(Graph::Vertices &v)
{
  Graph *g = v.graph();
  Graph::Vertices::Vex_ptr p(v);

  while (!p.end())
  {
    cout << g->label(*p) << endl;
    ++p;
  }
}

void print_vertices_oneline(Graph::Vertices &v)
{
  Graph *g = v.graph();
  Graph::Vertices::Vex_ptr p(v);

  if (v.size() == 0) return;

  cout << g->label(*p);
  ++p;
  while (!p.end())
  {
    cout << "\t" << g->label(*p);
    ++p;
  }
  cout << endl;
}
