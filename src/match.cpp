#include "match.h"

int find_large_anti_match(const Graph::Vertices &v, vector<int> *matches)
{
  static int counter = 0;
  ++counter;
  // if (counter % 10 == 0) cerr << counter << endl;

  int i,j;
  int match_size = 0;
  bool FOUND;
  vector<int> nodes;
  Graph::Vertices sg(v);
  Graph *g = v.graph();
  sg.sorted_nodes_vector(nodes);

  while(1)
  {
    FOUND = 0;

    for (i=0; i<(int)nodes.size(); ++i)
    {
      if (nodes[i] < 0) continue;
      for (j=i+1; j<(int)nodes.size(); ++j)
      {
        if (nodes[j] < 0) continue;
        if (!(g->connected(nodes[i], nodes[j])))
        {
          if (matches != 0)
          {
            matches->push_back(nodes[i]);
            matches->push_back(nodes[j]);
          }

          ++match_size;
          nodes[i] = -1;
          nodes[j] = -1;
          FOUND = 1;
          break;
        }
      }

      if (FOUND) break;
    }

    if (!FOUND) break;
  }

  return match_size;
}
