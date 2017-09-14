#include "Clique_Test.h"

bool Edge_Test::operator() (const Graph::Vertices &v, int min_size, int node)
                                                                        const
{
  if (node != -1)
  {
    cerr << "Error - edge test called with specific node." << endl;
    exit(EXIT_FAILURE);
  }

  int degree_sum = 0;
  const int degree_sum_needed = min_size * (min_size - 1);
  Graph::Vertices::Vex_ptr vp(v);

  for (; !vp.end(); ++vp)
  {
    degree_sum += v.degree(*vp);
    if (degree_sum >= degree_sum_needed) return 0;
  }

  return 1;
}

int Lazy_Color_Test::num_colors(const Graph::Vertices &v, int node) const
{
  int i;
  Graph *base_graph = v.graph();
  bitarray colors_found(base_graph->max_size());
  Graph::Vertices node_nbrs(v);
  vector<int> node_nbr_list;

  node_nbrs.remove_nnbrs(node);
  node_nbrs.insert(node);
  node_nbrs.sorted_nodes_vector(node_nbr_list);

  for (i=0; i<(int)node_nbr_list.size(); ++i)
  {
    colors_found.set(node2color[node_nbr_list[i]]);
  }

  return colors_found.count();
}

int Lazy_Match_Test::compute_anti_match_size(const Graph::Vertices &v) const
{
  int i;
  int num_matches = 0;

  for (i=0; i<(int)matches.size(); i += 2)
  {
    if (!(v.in_set(matches[i]))) continue;
    if (!(v.in_set(matches[i+1]))) continue;
    ++num_matches;
  }

  return num_matches;
}
