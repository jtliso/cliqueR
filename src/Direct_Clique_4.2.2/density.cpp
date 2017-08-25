// John Eblen
// September 11, 2007
// Script to input a graph and a list of induced subgraphs (vertex sets) and
// compute the edge density of each.

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Graph.h"

void read_subgraphs(Graph *, vector<Graph::Vertices *> &, char *);
double compute_density(Graph::Vertices *);
void split_string(string &, vector<string> &);

main(int argc, char **argv)
{
  int i;
  vector<Graph::Vertices *> subgraphs;

  if (argc < 3)
  {
    cerr << "Usage:  " << argv[0] << " <DIMACS graph> <Subgraph list>" << endl;
    exit(EXIT_SUCCESS);
  }
  string graph_file(argv[1]);
  Graph g(graph_file);

  read_subgraphs(&g, subgraphs, argv[2]);
  for (i=0; i<subgraphs.size(); ++i)
              cout << i << ":  " << compute_density(subgraphs[i]) << endl;
}

void read_subgraphs(Graph *g, vector<Graph::Vertices *> &subgraphs,
                                                   char *input_file)
{
  string s;
  vector<string> nodes;
  Graph::Vertices *v;
  ifstream input(input_file);
  subgraphs.resize(0);

  while (getline(input, s))
  {
    split_string(s, nodes);
    v = new Graph::Vertices(*g);
    v->clear();
    insert_nodes_by_name(*v, nodes);
    subgraphs.push_back(v);
  }

  input.close();
}

double compute_density(Graph::Vertices *v)
{
  double degree_sum = 0;
  double max_edges = (v->size() * (v->size() - 1)) / 2;

  Graph::Vertices::Vex_ptr vp(*v);
  while (!vp.end()) {degree_sum += v->degree(*vp); ++vp;}

  return (degree_sum / (2*max_edges));
}

// Useful clone of Perl's split function
void split_string(string &s, vector<string> &v)
{
  string word;
  istringstream input(s);
  v.resize(0);

  while(input)
  {
    input >> word;
    v.push_back(word);
  }
  v.pop_back();
}
