#include "color2.h"

int color_subgraph(const Graph::Vertices &sg, vector<int> *node2color,
                                              vector<int> *sorted_nodes)
{
  int i;
  int num_nodes_left = sg.size();
  int current_color = 0;
  Graph::Vertices color_nbrs(sg);

  // Warning:  Makes certain assumptions about static nature of the graph.
  // See graph library for details.
  vector<int> nodes;
  sg.sorted_nodes_vector(nodes);

  // Each iteration applies a single color.  Continue until all nodes are
  // colored.
  while(num_nodes_left > 0)
  {
    color_nbrs.clear();

    // Greedily color nodes, skipping those colored previously or neighbors of
    // nodes with the current color.
    for (i=nodes.size()-1; i >= 0; --i)
    {
      if (nodes[i] == -1) continue;
      if (color_nbrs.in_set(nodes[i])) continue;

      if (node2color) (*node2color)[nodes[i]] = current_color;
      if (sorted_nodes) (*sorted_nodes)[num_nodes_left-1] = nodes[i];

      color_nbrs.insert_nbrs(nodes[i]);
      --num_nodes_left;
      nodes[i] = -1;
    }

    ++current_color;
  }

  return current_color;
}
