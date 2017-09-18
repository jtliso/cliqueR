#include "MC_Heuristic2.h"

int Greedy_Clique::operator() (Graph::Vertices &subgraph,
                                               Graph::Vertices &clique) const
{
  int hdnode;
  Graph::Vertices candidates(subgraph);

  while (candidates.size() > 0)
  {
    hdnode = candidates.hdvertex();
    candidates.remove_nnbrs(hdnode);
    candidates.remove(hdnode);
    clique.insert(hdnode);
  }

  return clique.size();
}
